set(LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  AsmPrinter
  DebugInfoDWARF
  MC
  Object
  Support
  )

IF(APPLE)
set(BUNDLE_COMPONENTS
  CFBundle.cpp CFString.cpp CFStringUtils.c)
ENDIF(APPLE)

add_llvm_tool(llvm-dsymutil
  dsymutil.cpp
  BinaryHolder.cpp
  DebugMap.cpp
  DwarfLinker.cpp
  MachODebugMapParser.cpp
  MachOUtils.cpp
  ${BUNDLE_COMPONENTS}
  )

IF(APPLE)
  target_link_libraries(llvm-dsymutil "-framework CoreFoundation")
ENDIF(APPLE)
