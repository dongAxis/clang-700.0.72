//===--- UniqueptrResetRelease.h - clang-tidy -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_UNIQUEPTR_RESET_RELEASE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_UNIQUEPTR_RESET_RELEASE_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {

/// \brief Find and replace unique_ptr::reset(release()) with std::move
///
/// Example:
///   std::unique_ptr<Foo> x, y;
///   x.reset(y.release()); -> x = std::move(y);
///
/// If "y" is already rvalue, std::move is not added.
/// "x" and "y" can also be std::unique_ptr<Foo>*.
class UniqueptrResetRelease : public ClangTidyCheck {
public:
  UniqueptrResetRelease(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_UNIQUEPTR_RESET_RELEASE_H
