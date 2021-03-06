// RUN: clang-tidy -checks='-*,google-explicit-constructor' -header-filter='' %s -- -I %S/Inputs/file-filter -isystem %S/Inputs/file-filter/system 2>&1 | FileCheck %s
// RUN: clang-tidy -checks='-*,google-explicit-constructor' -header-filter='.*' %s -- -I %S/Inputs/file-filter -isystem %S/Inputs/file-filter/system 2>&1 | FileCheck --check-prefix=CHECK2 %s
// RUN: clang-tidy -checks='-*,google-explicit-constructor' -header-filter='header2\.h' %s -- -I %S/Inputs/file-filter -isystem %S/Inputs/file-filter/system 2>&1 | FileCheck --check-prefix=CHECK3 %s
// RUN: clang-tidy -checks='-*,google-explicit-constructor' -header-filter='.*' -system-headers %s -- -I %S/Inputs/file-filter -isystem %S/Inputs/file-filter/system 2>&1 | FileCheck --check-prefix=CHECK4 %s

#include "header1.h"
// CHECK-NOT: warning:
// CHECK2: header1.h:1:12: warning: single-argument constructors must be explicit [google-explicit-constructor]
// CHECK3-NOT: warning:
// CHECK4: header1.h:1:12: warning: single-argument constructors

#include "header2.h"
// CHECK-NOT: warning:
// CHECK2: header2.h:1:12: warning: single-argument constructors
// CHECK3: header2.h:1:12: warning: single-argument constructors
// CHECK4: header2.h:1:12: warning: single-argument constructors

#include <system-header.h>
// CHECK-NOT: warning:
// CHECK2-NOT: warning:
// CHECK3-NOT: warning:
// CHECK4: system-header.h:1:12: warning: single-argument constructors

class A { A(int); };
// CHECK: :[[@LINE-1]]:11: warning: single-argument constructors
// CHECK2: :[[@LINE-2]]:11: warning: single-argument constructors
// CHECK3: :[[@LINE-3]]:11: warning: single-argument constructors
// CHECK4: :[[@LINE-4]]:11: warning: single-argument constructors

// CHECK-NOT: warning:
// CHECK2-NOT: warning:
// CHECK3-NOT: warning:
// CHECK4-NOT: warning:

// CHECK: Suppressed 3 warnings (3 in non-user code)
// CHECK: Use -header-filter='.*' to display errors from all non-system headers.
// CHECK2: Suppressed 1 warnings (1 in non-user code)
// CHECK2: Use -header-filter='.*' {{.*}}
// CHECK3: Suppressed 2 warnings (2 in non-user code)
// CHECK3: Use -header-filter='.*' {{.*}}
// CHECK4-NOT: Suppressed {{.*}} warnings
// CHECK4-NOT: Use -header-filter='.*' {{.*}}

// FIXME: It doesn't pass on win32. Investigating.
// REQUIRES: shell
