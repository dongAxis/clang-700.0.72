//===-- BinaryHolder.cpp --------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This program is a utility that aims to be a dropin replacement for
// Darwin's dsymutil.
//
//===----------------------------------------------------------------------===//

#include "BinaryHolder.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Object/Error.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
namespace dsymutil {

static std::vector<MemoryBufferRef>
getMachOFatMemoryBuffers(StringRef Filename, MemoryBuffer &Mem,
                         object::MachOUniversalBinary &Fat) {
  std::vector<MemoryBufferRef> Buffers;
  StringRef FatData = Fat.getData();
  for (auto It = Fat.begin_objects(), End = Fat.end_objects(); It != End;
       ++It) {
    StringRef ObjData = FatData.substr(It->getOffset(), It->getSize());
    Buffers.emplace_back(ObjData, Filename);
  }
  return std::move(Buffers);
}

void
BinaryHolder::changeBackingMemoryBuffer(std::unique_ptr<MemoryBuffer> &&Buf) {
  CurrentArchives.clear();
  CurrentObjectFiles.clear();
  CurrentFatBinary.reset();

  CurrentMemoryBuffer = std::move(Buf);
}

ErrorOr<std::vector<MemoryBufferRef>>
BinaryHolder::GetMemoryBuffersForFile(StringRef Filename,
                                      sys::TimeValue Timestamp) {
  if (Verbose)
    outs() << "trying to open '" << Filename << "'\n";

  // Try that first as it doesn't involve any filesystem access.
  if (auto ErrOrArchiveMembers = GetArchiveMemberBuffers(Filename, Timestamp))
    return *ErrOrArchiveMembers;

  // If the name ends with a closing paren, there is a huge chance
  // it is an archive member specification.
  if (Filename.endswith(")"))
    if (auto ErrOrArchiveMember =
        MapArchiveAndGetMemberBuffers(Filename, Timestamp))
      return *ErrOrArchiveMember;

  // Otherwise, just try opening a standard file. If this is an
  // archive member specifiaction and any of the above didn't handle it
  // (either because the archive is not there anymore, or because the
  // archive doesn't contain the requested member), this will still
  // provide a sensible error message.
  auto ErrOrFile = MemoryBuffer::getFileOrSTDIN(Filename);
  if (auto Err = ErrOrFile.getError())
    return Err;

  changeBackingMemoryBuffer(std::move(*ErrOrFile));
  if (Verbose)
    outs() << "\tloaded file.\n";

  auto ErrOrFat = object::MachOUniversalBinary::create(
      CurrentMemoryBuffer->getMemBufferRef());
  if (ErrOrFat.getError()) {
    // Not a fat binary must be a standard one.
    return std::vector<MemoryBufferRef>(1,
                                        CurrentMemoryBuffer->getMemBufferRef());
  }

  CurrentFatBinary = std::move(*ErrOrFat);
  return getMachOFatMemoryBuffers(Filename, *CurrentMemoryBuffer,
                                  *CurrentFatBinary);
}

ErrorOr<std::vector<MemoryBufferRef>>
BinaryHolder::GetArchiveMemberBuffers(StringRef Filename,
                                      sys::TimeValue Timestamp) {
  if (CurrentArchives.empty())
    return make_error_code(errc::no_such_file_or_directory);

  StringRef CurArchiveName = CurrentArchives.front()->getFileName();
  if (!Filename.startswith(Twine(CurArchiveName, "(").str()))
    return make_error_code(errc::no_such_file_or_directory);

  // Remove the archive name and the parens around the archive member name.
  Filename = Filename.substr(CurArchiveName.size() + 1).drop_back();

  std::vector<MemoryBufferRef> Buffers;
  Buffers.reserve(CurrentArchives.size());

  for (const auto &Archive : CurrentArchives) {
    for (const auto &Child : Archive->children()) {
      if (auto NameOrErr = Child.getName()) {
        if (auto Err = NameOrErr.getError())
          return Err;
        if (*NameOrErr == Filename) {
          if (Timestamp != sys::TimeValue::MinTime() &&
              Timestamp != Child.getLastModified()) {
            if (Verbose)
              outs() << "\ttimestamp mismatch.\n";
          } else {
            if (Verbose)
              outs() << "\tfound member in current archive.\n";
            auto ErrOrMem = Child.getMemoryBufferRef();
            if (auto Err = ErrOrMem.getError())
              return Err;
            Buffers.push_back(*ErrOrMem);
          }
        }
      }
    }
  }

  if (Buffers.empty())
    return make_error_code(errc::no_such_file_or_directory);

  return Buffers;
}

ErrorOr<std::vector<MemoryBufferRef>>
BinaryHolder::MapArchiveAndGetMemberBuffers(StringRef Filename,
                                            sys::TimeValue Timestamp) {
  StringRef ArchiveFilename = Filename.substr(0, Filename.find('('));

  auto ErrOrBuff = MemoryBuffer::getFileOrSTDIN(ArchiveFilename);
  if (auto Err = ErrOrBuff.getError())
    return Err;

  if (Verbose)
    outs() << "\topened new archive '" << ArchiveFilename << "'\n";

  changeBackingMemoryBuffer(std::move(*ErrOrBuff));
  std::vector<MemoryBufferRef> ArchiveBuffers;
  auto ErrOrFat = object::MachOUniversalBinary::create(
      CurrentMemoryBuffer->getMemBufferRef());
  if (ErrOrFat.getError()) {
    // Not a fat binary must be a standard one.
    ArchiveBuffers.push_back(CurrentMemoryBuffer->getMemBufferRef());
  } else {
    CurrentFatBinary = std::move(*ErrOrFat);
    ArchiveBuffers = getMachOFatMemoryBuffers(
        ArchiveFilename, *CurrentMemoryBuffer, *CurrentFatBinary);
  }

  for (auto MemRef : ArchiveBuffers) {
    auto ErrOrArchive = object::Archive::create(MemRef);
    if (auto Err = ErrOrArchive.getError())
      return Err;
    CurrentArchives.push_back(std::move(*ErrOrArchive));
  }
  return GetArchiveMemberBuffers(Filename, Timestamp);
}

ErrorOr<const object::ObjectFile &>
BinaryHolder::getObjfileForArch(const Triple &T) {
  for (const auto &Obj : CurrentObjectFiles) {
    if (const auto *MachO = dyn_cast<object::MachOObjectFile>(Obj.get())) {
      // FIXME: getArch will return bad triples for the processors
      // that can only do Thumb. Use the thumb triple when one is
      // returned, we don't really care about the arch details anyway.
      // Maybe we should change getArch to return only valid triples?
      Triple ThumbTriple;
      Triple MachOTriple = MachO->getArch(nullptr, &ThumbTriple);
      if (ThumbTriple.getArch())
        MachOTriple = ThumbTriple;
      if (MachOTriple.str() == T.str())
        return *MachO;
    } else if (Obj->getArch() == T.getArch())
      return *Obj;
  }

  return make_error_code(object::object_error::arch_not_found);
}

// ErrorOr<const object::MachOObjectFile *>
// BinaryHolder::GetNextObjectFile() {
//   if (!CurrentFatBinary)
//     return make_error_code(object::object_error::invalid_file_type);

//   ++CurrentFatPos;
//   if (CurrentFatPos == CurrentFatBinary->end_objects())
//     return nullptr;

//   auto ErrOrMacho = CurrentFatPos->getAsObjectFile();
//   if (auto Err = ErrOrMacho.getError())
//     return Err;

//   auto *MachO = (*ErrOrMacho).get();
//   CurrentObjectFile = std::move(*ErrOrMacho);
//   return MachO;
// }

ErrorOr<std::vector<const object::ObjectFile *>>
BinaryHolder::GetObjectFiles(StringRef Filename, sys::TimeValue Timestamp) {
  auto ErrOrMemBufferRefs = GetMemoryBuffersForFile(Filename, Timestamp);
  if (auto Err = ErrOrMemBufferRefs.getError())
    return Err;

  std::vector<const object::ObjectFile *> Objects;
  Objects.reserve(ErrOrMemBufferRefs->size());

  CurrentObjectFiles.clear();
  for (auto MemBuf : *ErrOrMemBufferRefs) {
    auto ErrOrObjectFile = object::ObjectFile::createObjectFile(MemBuf);
    if (auto Err = ErrOrObjectFile.getError())
      return Err;

    Objects.push_back(ErrOrObjectFile->get());
    CurrentObjectFiles.push_back(std::move(*ErrOrObjectFile));
  }

  return std::move(Objects);
}
}
}
