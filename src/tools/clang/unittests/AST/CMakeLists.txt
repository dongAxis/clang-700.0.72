set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_unittest(ASTTests
  ASTContextParentMapTest.cpp
  ASTTypeTraitsTest.cpp
  ASTVectorTest.cpp
  CommentLexer.cpp
  CommentParser.cpp
  DeclPrinterTest.cpp
  DeclTest.cpp
  EvaluateAsRValueTest.cpp
  ExternalASTSourceTest.cpp
  NamedDeclPrinterTest.cpp
  SourceLocationTest.cpp
  StmtPrinterTest.cpp
  )

target_link_libraries(ASTTests
  clangAST
  clangASTMatchers
  clangBasic
  clangCodeGen
  clangFrontend
  clangTooling
  )
