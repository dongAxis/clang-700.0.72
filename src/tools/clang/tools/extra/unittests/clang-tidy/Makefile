##===- unittests/clang-tidy/Makefile -----------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

CLANG_LEVEL = ../../../..
include $(CLANG_LEVEL)/../../Makefile.config

TESTNAME = ClangTidy
LINK_COMPONENTS := $(TARGETS_TO_BUILD) asmparser bitreader bitwriter \
                   instrumentation ipo support mc objcarcopts option
USEDLIBS = clangTidy.a clangTidyLLVMModule.a clangTidyGoogleModule.a \
	   clangTidyMiscModule.a clangTidyReadability.a clangTidy.a \
	   clangTidyUtils.a \
	   clangStaticAnalyzerFrontend.a clangStaticAnalyzerCheckers.a \
	   clangStaticAnalyzerCore.a \
	   clangFormat.a clangTooling.a clangToolingCore.a \
	   clangFrontend.a clangCodeGen.a clangIndex.a clangSerialization.a \
	   clangDriver.a clangRewriteFrontend.a clangRewrite.a \
	   clangParse.a clangSema.a clangAnalysis.a clangAST.a \
	   clangASTMatchers.a clangEdit.a clangLex.a clangAPINotes.a clangBasic.a \
           clangIndex.a

include $(CLANG_LEVEL)/Makefile
MAKEFILE_UNITTEST_NO_INCLUDE_COMMON := 1
CPP.Flags += -I$(PROJ_SRC_DIR)/../../clang-tidy
include $(LLVM_SRC_ROOT)/unittests/Makefile.unittest
