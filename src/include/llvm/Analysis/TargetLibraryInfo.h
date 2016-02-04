//===-- TargetLibraryInfo.h - Library information ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_TARGETLIBRARYINFO_H
#define LLVM_ANALYSIS_TARGETLIBRARYINFO_H

#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/Triple.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

namespace llvm {
/// VecDesc - Describes a possible vectorization of a function.
/// Function 'VectorFnName' is equivalent to 'ScalarFnName' vectorized
/// by a factor 'VectorizationFactor'.
struct VecDesc {
  const char *ScalarFnName;
  const char *VectorFnName;
  unsigned VectorizationFactor;
};
class PreservedAnalyses;

  namespace LibFunc {
    enum Func {
      /// int _IO_getc(_IO_FILE * __fp);
      under_IO_getc,
      /// int _IO_putc(int __c, _IO_FILE * __fp);
      under_IO_putc,
      /// void operator delete[](void*);
      ZdaPv,
      /// void operator delete[](void*, nothrow);
      ZdaPvRKSt9nothrow_t,
      /// void operator delete[](void*, unsigned int);
      ZdaPvj,
      /// void operator delete[](void*, unsigned long);
      ZdaPvm,
      /// void operator delete(void*);
      ZdlPv,
      /// void operator delete(void*, nothrow);
      ZdlPvRKSt9nothrow_t,
      /// void operator delete(void*, unsigned int);
      ZdlPvj,
      /// void operator delete(void*, unsigned long);
      ZdlPvm,
      /// void *new[](unsigned int);
      Znaj,
      /// void *new[](unsigned int, nothrow);
      ZnajRKSt9nothrow_t,
      /// void *new[](unsigned long);
      Znam,
      /// void *new[](unsigned long, nothrow);
      ZnamRKSt9nothrow_t,
      /// void *new(unsigned int);
      Znwj,
      /// void *new(unsigned int, nothrow);
      ZnwjRKSt9nothrow_t,
      /// void *new(unsigned long);
      Znwm,
      /// void *new(unsigned long, nothrow);
      ZnwmRKSt9nothrow_t,
      /// double __cospi(double x);
      cospi,
      /// float __cospif(float x);
      cospif,
      /// int __cxa_atexit(void (*f)(void *), void *p, void *d);
      cxa_atexit,
      /// void __cxa_guard_abort(guard_t *guard);
      /// guard_t is int64_t in Itanium ABI or int32_t on ARM eabi.
      cxa_guard_abort,
      /// int __cxa_guard_acquire(guard_t *guard);
      cxa_guard_acquire,
      /// void __cxa_guard_release(guard_t *guard);
      cxa_guard_release,
      /// int __isoc99_scanf (const char *format, ...)
      dunder_isoc99_scanf,
      /// int __isoc99_sscanf(const char *s, const char *format, ...)
      dunder_isoc99_sscanf,
      /// void *__memcpy_chk(void *s1, const void *s2, size_t n, size_t s1size);
      memcpy_chk,
      /// void *__memmove_chk(void *s1, const void *s2, size_t n,
      ///                     size_t s1size);
      memmove_chk,
      /// void *__memset_chk(void *s, char v, size_t n, size_t s1size);
      memset_chk,
      /// double __sincospi_stret(double x);
      sincospi_stret,
      /// float __sincospif_stret(float x);
      sincospif_stret,
      /// double __sinpi(double x);
      sinpi,
      /// float __sinpif(float x);
      sinpif,
      /// double __sqrt_finite(double x);
      sqrt_finite,
      /// float __sqrt_finite(float x);
      sqrtf_finite,
      /// long double __sqrt_finite(long double x);
      sqrtl_finite,
      /// char *__stpcpy_chk(char *s1, const char *s2, size_t s1size);
      stpcpy_chk,
      /// char *__stpncpy_chk(char *s1, const char *s2, size_t n,
      ///                     size_t s1size);
      stpncpy_chk,
      /// char *__strcpy_chk(char *s1, const char *s2, size_t s1size);
      strcpy_chk,
      /// char * __strdup(const char *s);
      dunder_strdup,
      /// char *__strncpy_chk(char *s1, const char *s2, size_t n,
      ///                     size_t s1size);
      strncpy_chk,
      /// char *__strndup(const char *s, size_t n);
      dunder_strndup,
      /// char * __strtok_r(char *s, const char *delim, char **save_ptr);
      dunder_strtok_r,
      /// int abs(int j);
      abs,
      /// int access(const char *path, int amode);
      access,
      /// double acos(double x);
      acos,
      /// float acosf(float x);
      acosf,
      /// double acosh(double x);
      acosh,
      /// float acoshf(float x);
      acoshf,
      /// long double acoshl(long double x);
      acoshl,
      /// long double acosl(long double x);
      acosl,
      /// double asin(double x);
      asin,
      /// float asinf(float x);
      asinf,
      /// double asinh(double x);
      asinh,
      /// float asinhf(float x);
      asinhf,
      /// long double asinhl(long double x);
      asinhl,
      /// long double asinl(long double x);
      asinl,
      /// double atan(double x);
      atan,
      /// double atan2(double y, double x);
      atan2,
      /// float atan2f(float y, float x);
      atan2f,
      /// long double atan2l(long double y, long double x);
      atan2l,
      /// float atanf(float x);
      atanf,
      /// double atanh(double x);
      atanh,
      /// float atanhf(float x);
      atanhf,
      /// long double atanhl(long double x);
      atanhl,
      /// long double atanl(long double x);
      atanl,
      /// double atof(const char *str);
      atof,
      /// int atoi(const char *str);
      atoi,
      /// long atol(const char *str);
      atol,
      /// long long atoll(const char *nptr);
      atoll,
      /// int bcmp(const void *s1, const void *s2, size_t n);
      bcmp,
      /// void bcopy(const void *s1, void *s2, size_t n);
      bcopy,
      /// void bzero(void *s, size_t n);
      bzero,
      /// void *calloc(size_t count, size_t size);
      calloc,
      /// double cbrt(double x);
      cbrt,
      /// float cbrtf(float x);
      cbrtf,
      /// long double cbrtl(long double x);
      cbrtl,
      /// double ceil(double x);
      ceil,
      /// float ceilf(float x);
      ceilf,
      /// long double ceill(long double x);
      ceill,
      /// int chmod(const char *path, mode_t mode);
      chmod,
      /// int chown(const char *path, uid_t owner, gid_t group);
      chown,
      /// void clearerr(FILE *stream);
      clearerr,
      /// int closedir(DIR *dirp);
      closedir,
      /// double copysign(double x, double y);
      copysign,
      /// float copysignf(float x, float y);
      copysignf,
      /// long double copysignl(long double x, long double y);
      copysignl,
      /// double cos(double x);
      cos,
      /// float cosf(float x);
      cosf,
      /// double cosh(double x);
      cosh,
      /// float coshf(float x);
      coshf,
      /// long double coshl(long double x);
      coshl,
      /// long double cosl(long double x);
      cosl,
      /// char *ctermid(char *s);
      ctermid,
      /// double exp(double x);
      exp,
      /// double exp10(double x);
      exp10,
      /// float exp10f(float x);
      exp10f,
      /// long double exp10l(long double x);
      exp10l,
      /// double exp2(double x);
      exp2,
      /// float exp2f(float x);
      exp2f,
      /// long double exp2l(long double x);
      exp2l,
      /// float expf(float x);
      expf,
      /// long double expl(long double x);
      expl,
      /// double expm1(double x);
      expm1,
      /// float expm1f(float x);
      expm1f,
      /// long double expm1l(long double x);
      expm1l,
      /// double fabs(double x);
      fabs,
      /// float fabsf(float x);
      fabsf,
      /// long double fabsl(long double x);
      fabsl,
      /// int fclose(FILE *stream);
      fclose,
      /// FILE *fdopen(int fildes, const char *mode);
      fdopen,
      /// int feof(FILE *stream);
      feof,
      /// int ferror(FILE *stream);
      ferror,
      /// int fflush(FILE *stream);
      fflush,
      /// int ffs(int i);
      ffs,
      /// int ffsl(long int i);
      ffsl,
      /// int ffsll(long long int i);
      ffsll,
      /// int fgetc(FILE *stream);
      fgetc,
      /// int fgetpos(FILE *stream, fpos_t *pos);
      fgetpos,
      /// char *fgets(char *s, int n, FILE *stream);
      fgets,
      /// int fileno(FILE *stream);
      fileno,
      /// int fiprintf(FILE *stream, const char *format, ...);
      fiprintf,
      /// void flockfile(FILE *file);
      flockfile,
      /// double floor(double x);
      floor,
      /// float floorf(float x);
      floorf,
      /// long double floorl(long double x);
      floorl,
      /// double fmax(double x, double y);
      fmax,
      /// float fmaxf(float x, float y);
      fmaxf,
      /// long double fmaxl(long double x, long double y);
      fmaxl,
      /// double fmin(double x, double y);
      fmin,
      /// float fminf(float x, float y);
      fminf,
      /// long double fminl(long double x, long double y);
      fminl,
      /// double fmod(double x, double y);
      fmod,
      /// float fmodf(float x, float y);
      fmodf,
      /// long double fmodl(long double x, long double y);
      fmodl,
      /// FILE *fopen(const char *filename, const char *mode);
      fopen,
      /// FILE *fopen64(const char *filename, const char *opentype)
      fopen64,
      /// int fprintf(FILE *stream, const char *format, ...);
      fprintf,
      /// int fputc(int c, FILE *stream);
      fputc,
      /// int fputs(const char *s, FILE *stream);
      fputs,
      /// size_t fread(void *ptr, size_t size, size_t nitems, FILE *stream);
      fread,
      /// void free(void *ptr);
      free,
      /// double frexp(double num, int *exp);
      frexp,
      /// float frexpf(float num, int *exp);
      frexpf,
      /// long double frexpl(long double num, int *exp);
      frexpl,
      /// int fscanf(FILE *stream, const char *format, ... );
      fscanf,
      /// int fseek(FILE *stream, long offset, int whence);
      fseek,
      /// int fseeko(FILE *stream, off_t offset, int whence);
      fseeko,
      /// int fseeko64(FILE *stream, off64_t offset, int whence)
      fseeko64,
      /// int fsetpos(FILE *stream, const fpos_t *pos);
      fsetpos,
      /// int fstat(int fildes, struct stat *buf);
      fstat,
      /// int fstat64(int filedes, struct stat64 *buf)
      fstat64,
      /// int fstatvfs(int fildes, struct statvfs *buf);
      fstatvfs,
      /// int fstatvfs64(int fildes, struct statvfs64 *buf);
      fstatvfs64,
      /// long ftell(FILE *stream);
      ftell,
      /// off_t ftello(FILE *stream);
      ftello,
      /// off64_t ftello64(FILE *stream)
      ftello64,
      /// int ftrylockfile(FILE *file);
      ftrylockfile,
      /// void funlockfile(FILE *file);
      funlockfile,
      /// size_t fwrite(const void *ptr, size_t size, size_t nitems,
      /// FILE *stream);
      fwrite,
      /// int getc(FILE *stream);
      getc,
      /// int getc_unlocked(FILE *stream);
      getc_unlocked,
      /// int getchar(void);
      getchar,
      /// char *getenv(const char *name);
      getenv,
      /// int getitimer(int which, struct itimerval *value);
      getitimer,
      /// int getlogin_r(char *name, size_t namesize);
      getlogin_r,
      /// struct passwd *getpwnam(const char *name);
      getpwnam,
      /// char *gets(char *s);
      gets,
      /// int gettimeofday(struct timeval *tp, void *tzp);
      gettimeofday,
      /// uint32_t htonl(uint32_t hostlong);
      htonl,
      /// uint16_t htons(uint16_t hostshort);
      htons,
      /// int iprintf(const char *format, ...);
      iprintf,
      /// int isascii(int c);
      isascii,
      /// int isdigit(int c);
      isdigit,
      /// long int labs(long int j);
      labs,
      /// int lchown(const char *path, uid_t owner, gid_t group);
      lchown,
      /// double ldexp(double x, int n);
      ldexp,
      /// float ldexpf(float x, int n);
      ldexpf,
      /// long double ldexpl(long double x, int n);
      ldexpl,
      /// long long int llabs(long long int j);
      llabs,
      /// double log(double x);
      log,
      /// double log10(double x);
      log10,
      /// float log10f(float x);
      log10f,
      /// long double log10l(long double x);
      log10l,
      /// double log1p(double x);
      log1p,
      /// float log1pf(float x);
      log1pf,
      /// long double log1pl(long double x);
      log1pl,
      /// double log2(double x);
      log2,
      /// float log2f(float x);
      log2f,
      /// double long double log2l(long double x);
      log2l,
      /// double logb(double x);
      logb,
      /// float logbf(float x);
      logbf,
      /// long double logbl(long double x);
      logbl,
      /// float logf(float x);
      logf,
      /// long double logl(long double x);
      logl,
      /// int lstat(const char *path, struct stat *buf);
      lstat,
      /// int lstat64(const char *path, struct stat64 *buf);
      lstat64,
      /// void *malloc(size_t size);
      malloc,
      /// void *memalign(size_t boundary, size_t size);
      memalign,
      /// void *memccpy(void *s1, const void *s2, int c, size_t n);
      memccpy,
      /// void *memchr(const void *s, int c, size_t n);
      memchr,
      /// int memcmp(const void *s1, const void *s2, size_t n);
      memcmp,
      /// void *memcpy(void *s1, const void *s2, size_t n);
      memcpy,
      /// void *memmove(void *s1, const void *s2, size_t n);
      memmove,
      // void *memrchr(const void *s, int c, size_t n);
      memrchr,
      /// void *memset(void *b, int c, size_t len);
      memset,
      /// void memset_pattern16(void *b, const void *pattern16, size_t len);
      memset_pattern16,
      /// int mkdir(const char *path, mode_t mode);
      mkdir,
      /// time_t mktime(struct tm *timeptr);
      mktime,
      /// double modf(double x, double *iptr);
      modf,
      /// float modff(float, float *iptr);
      modff,
      /// long double modfl(long double value, long double *iptr);
      modfl,
      /// double nearbyint(double x);
      nearbyint,
      /// float nearbyintf(float x);
      nearbyintf,
      /// long double nearbyintl(long double x);
      nearbyintl,
      /// uint32_t ntohl(uint32_t netlong);
      ntohl,
      /// uint16_t ntohs(uint16_t netshort);
      ntohs,
      /// int open(const char *path, int oflag, ... );
      open,
      /// int open64(const char *filename, int flags[, mode_t mode])
      open64,
      /// DIR *opendir(const char *dirname);
      opendir,
      /// int pclose(FILE *stream);
      pclose,
      /// void perror(const char *s);
      perror,
      /// FILE *popen(const char *command, const char *mode);
      popen,
      /// int posix_memalign(void **memptr, size_t alignment, size_t size);
      posix_memalign,
      /// double pow(double x, double y);
      pow,
      /// float powf(float x, float y);
      powf,
      /// long double powl(long double x, long double y);
      powl,
      /// ssize_t pread(int fildes, void *buf, size_t nbyte, off_t offset);
      pread,
      /// int printf(const char *format, ...);
      printf,
      /// int putc(int c, FILE *stream);
      putc,
      /// int putchar(int c);
      putchar,
      /// int puts(const char *s);
      puts,
      /// ssize_t pwrite(int fildes, const void *buf, size_t nbyte,
      ///                off_t offset);
      pwrite,
      /// void qsort(void *base, size_t nel, size_t width,
      ///            int (*compar)(const void *, const void *));
      qsort,
      /// ssize_t read(int fildes, void *buf, size_t nbyte);
      read,
      /// ssize_t readlink(const char *path, char *buf, size_t bufsize);
      readlink,
      /// void *realloc(void *ptr, size_t size);
      realloc,
      /// void *reallocf(void *ptr, size_t size);
      reallocf,
      /// char *realpath(const char *file_name, char *resolved_name);
      realpath,
      /// int remove(const char *path);
      remove,
      /// int rename(const char *old, const char *new);
      rename,
      /// void rewind(FILE *stream);
      rewind,
      /// double rint(double x);
      rint,
      /// float rintf(float x);
      rintf,
      /// long double rintl(long double x);
      rintl,
      /// int rmdir(const char *path);
      rmdir,
      /// double round(double x);
      round,
      /// float roundf(float x);
      roundf,
      /// long double roundl(long double x);
      roundl,
      /// int scanf(const char *restrict format, ... );
      scanf,
      /// void setbuf(FILE *stream, char *buf);
      setbuf,
      /// int setitimer(int which, const struct itimerval *value,
      ///               struct itimerval *ovalue);
      setitimer,
      /// int setvbuf(FILE *stream, char *buf, int type, size_t size);
      setvbuf,
      /// double sin(double x);
      sin,
      /// float sinf(float x);
      sinf,
      /// double sinh(double x);
      sinh,
      /// float sinhf(float x);
      sinhf,
      /// long double sinhl(long double x);
      sinhl,
      /// long double sinl(long double x);
      sinl,
      /// int siprintf(char *str, const char *format, ...);
      siprintf,
      /// int snprintf(char *s, size_t n, const char *format, ...);
      snprintf,
      /// int sprintf(char *str, const char *format, ...);
      sprintf,
      /// double sqrt(double x);
      sqrt,
      /// float sqrtf(float x);
      sqrtf,
      /// long double sqrtl(long double x);
      sqrtl,
      /// int sscanf(const char *s, const char *format, ... );
      sscanf,
      /// int stat(const char *path, struct stat *buf);
      stat,
      /// int stat64(const char *path, struct stat64 *buf);
      stat64,
      /// int statvfs(const char *path, struct statvfs *buf);
      statvfs,
      /// int statvfs64(const char *path, struct statvfs64 *buf)
      statvfs64,
      /// char *stpcpy(char *s1, const char *s2);
      stpcpy,
      /// char *stpncpy(char *s1, const char *s2, size_t n);
      stpncpy,
      /// int strcasecmp(const char *s1, const char *s2);
      strcasecmp,
      /// char *strcat(char *s1, const char *s2);
      strcat,
      /// char *strchr(const char *s, int c);
      strchr,
      /// int strcmp(const char *s1, const char *s2);
      strcmp,
      /// int strcoll(const char *s1, const char *s2);
      strcoll,
      /// char *strcpy(char *s1, const char *s2);
      strcpy,
      /// size_t strcspn(const char *s1, const char *s2);
      strcspn,
      /// char *strdup(const char *s1);
      strdup,
      /// size_t strlen(const char *s);
      strlen,
      /// int strncasecmp(const char *s1, const char *s2, size_t n);
      strncasecmp,
      /// char *strncat(char *s1, const char *s2, size_t n);
      strncat,
      /// int strncmp(const char *s1, const char *s2, size_t n);
      strncmp,
      /// char *strncpy(char *s1, const char *s2, size_t n);
      strncpy,
      /// char *strndup(const char *s1, size_t n);
      strndup,
      /// size_t strnlen(const char *s, size_t maxlen);
      strnlen,
      /// char *strpbrk(const char *s1, const char *s2);
      strpbrk,
      /// char *strrchr(const char *s, int c);
      strrchr,
      /// size_t strspn(const char *s1, const char *s2);
      strspn,
      /// char *strstr(const char *s1, const char *s2);
      strstr,
      /// double strtod(const char *nptr, char **endptr);
      strtod,
      /// float strtof(const char *nptr, char **endptr);
      strtof,
      // char *strtok(char *s1, const char *s2);
      strtok,
      // char *strtok_r(char *s, const char *sep, char **lasts);
      strtok_r,
      /// long int strtol(const char *nptr, char **endptr, int base);
      strtol,
      /// long double strtold(const char *nptr, char **endptr);
      strtold,
      /// long long int strtoll(const char *nptr, char **endptr, int base);
      strtoll,
      /// unsigned long int strtoul(const char *nptr, char **endptr, int base);
      strtoul,
      /// unsigned long long int strtoull(const char *nptr, char **endptr,
      ///                                 int base);
      strtoull,
      /// size_t strxfrm(char *s1, const char *s2, size_t n);
      strxfrm,
      /// int system(const char *command);
      system,
      /// double tan(double x);
      tan,
      /// float tanf(float x);
      tanf,
      /// double tanh(double x);
      tanh,
      /// float tanhf(float x);
      tanhf,
      /// long double tanhl(long double x);
      tanhl,
      /// long double tanl(long double x);
      tanl,
      /// clock_t times(struct tms *buffer);
      times,
      /// FILE *tmpfile(void);
      tmpfile,
      /// FILE *tmpfile64(void)
      tmpfile64,
      /// int toascii(int c);
      toascii,
      /// double trunc(double x);
      trunc,
      /// float truncf(float x);
      truncf,
      /// long double truncl(long double x);
      truncl,
      /// int uname(struct utsname *name);
      uname,
      /// int ungetc(int c, FILE *stream);
      ungetc,
      /// int unlink(const char *path);
      unlink,
      /// int unsetenv(const char *name);
      unsetenv,
      /// int utime(const char *path, const struct utimbuf *times);
      utime,
      /// int utimes(const char *path, const struct timeval times[2]);
      utimes,
      /// void *valloc(size_t size);
      valloc,
      /// int vfprintf(FILE *stream, const char *format, va_list ap);
      vfprintf,
      /// int vfscanf(FILE *stream, const char *format, va_list arg);
      vfscanf,
      /// int vprintf(const char *restrict format, va_list ap);
      vprintf,
      /// int vscanf(const char *format, va_list arg);
      vscanf,
      /// int vsnprintf(char *s, size_t n, const char *format, va_list ap);
      vsnprintf,
      /// int vsprintf(char *s, const char *format, va_list ap);
      vsprintf,
      /// int vsscanf(const char *s, const char *format, va_list arg);
      vsscanf,
      /// ssize_t write(int fildes, const void *buf, size_t nbyte);
      write,

      NumLibFuncs
    };
  }

/// \brief Implementation of the target library information.
///
/// This class constructs tables that hold the target library information and
/// make it available. However, it is somewhat expensive to compute and only
/// depends on the triple. So users typicaly interact with the \c
/// TargetLibraryInfo wrapper below.
class TargetLibraryInfoImpl {
  friend class TargetLibraryInfo;

  unsigned char AvailableArray[(LibFunc::NumLibFuncs+3)/4];
  llvm::DenseMap<unsigned, std::string> CustomNames;
  static const char* StandardNames[LibFunc::NumLibFuncs];

  enum AvailabilityState {
    StandardName = 3, // (memset to all ones)
    CustomName = 1,
    Unavailable = 0  // (memset to all zeros)
  };
  void setState(LibFunc::Func F, AvailabilityState State) {
    AvailableArray[F/4] &= ~(3 << 2*(F&3));
    AvailableArray[F/4] |= State << 2*(F&3);
  }
  AvailabilityState getState(LibFunc::Func F) const {
    return static_cast<AvailabilityState>((AvailableArray[F/4] >> 2*(F&3)) & 3);
  }

  /// Vectorization descriptors - sorted by ScalarFnName.
  std::vector<VecDesc> VectorDescs;
  /// Scalarization descriptors - same content as VectorDescs but sorted based
  /// on VectorFnName rather than ScalarFnName.
  std::vector<VecDesc> ScalarDescs;

public:
  /// \brief  List of known vector-functions libraries.
  ///
  /// The vector-functions library defines, which functions are vectorizable
  /// and with which factor. The library can be specified by either frontend,
  /// or a commandline option, and then used by
  /// addVectorizableFunctionsFromVecLib for filling up the tables of
  /// vectorizable functions.
  enum VectorLibrary {
    NoLibrary, // Don't use any vector library.
    Accelerate // Use Accelerate framework.
  };

  TargetLibraryInfoImpl();
  explicit TargetLibraryInfoImpl(const Triple &T);

  // Provide value semantics.
  TargetLibraryInfoImpl(const TargetLibraryInfoImpl &TLI);
  TargetLibraryInfoImpl(TargetLibraryInfoImpl &&TLI);
  TargetLibraryInfoImpl &operator=(const TargetLibraryInfoImpl &TLI);
  TargetLibraryInfoImpl &operator=(TargetLibraryInfoImpl &&TLI);

  /// \brief Searches for a particular function name.
  ///
  /// If it is one of the known library functions, return true and set F to the
  /// corresponding value.
  bool getLibFunc(StringRef funcName, LibFunc::Func &F) const;

  /// \brief Forces a function to be marked as unavailable.
  void setUnavailable(LibFunc::Func F) {
    setState(F, Unavailable);
  }

  /// \brief Forces a function to be marked as available.
  void setAvailable(LibFunc::Func F) {
    setState(F, StandardName);
  }

  /// \brief Forces a function to be marked as available and provide an
  /// alternate name that must be used.
  void setAvailableWithName(LibFunc::Func F, StringRef Name) {
    if (StandardNames[F] != Name) {
      setState(F, CustomName);
      CustomNames[F] = Name;
      assert(CustomNames.find(F) != CustomNames.end());
    } else {
      setState(F, StandardName);
    }
  }

  /// \brief Disables all builtins.
  ///
  /// This can be used for options like -fno-builtin.
  void disableAllFunctions();

  /// addVectorizableFunctions - Add a set of scalar -> vector mappings,
  /// queryable via getVectorizedFunction and getScalarizedFunction.
  void addVectorizableFunctions(ArrayRef<VecDesc> Fns);

  /// Calls addVectorizableFunctions with a known preset of functions for the
  /// given vector library.
  void addVectorizableFunctionsFromVecLib(enum VectorLibrary VecLib);

  /// isFunctionVectorizable - Return true if the function F has a
  /// vector equivalent with vectorization factor VF.
  bool isFunctionVectorizable(StringRef F, unsigned VF) const {
    return !getVectorizedFunction(F, VF).empty();
  }

  /// isFunctionVectorizable - Return true if the function F has a
  /// vector equivalent with any vectorization factor.
  bool isFunctionVectorizable(StringRef F) const;

  /// getVectorizedFunction - Return the name of the equivalent of
  /// F, vectorized with factor VF. If no such mapping exists,
  /// return the empty string.
  StringRef getVectorizedFunction(StringRef F, unsigned VF) const;

  /// isFunctionScalarizable - Return true if the function F has a
  /// scalar equivalent, and set VF to be the vectorization factor.
  bool isFunctionScalarizable(StringRef F, unsigned &VF) const {
    return !getScalarizedFunction(F, VF).empty();
  }

  /// getScalarizedFunction - Return the name of the equivalent of
  /// F, scalarized. If no such mapping exists, return the empty string.
  ///
  /// Set VF to the vectorization factor.
  StringRef getScalarizedFunction(StringRef F, unsigned &VF) const;
};

/// \brief Provides information about what library functions are available for
/// the current target.
///
/// This both allows optimizations to handle them specially and frontends to
/// disable such optimizations through -fno-builtin etc.
class TargetLibraryInfo {
  friend class TargetLibraryAnalysis;
  friend class TargetLibraryInfoWrapperPass;

  const TargetLibraryInfoImpl *Impl;

public:
  explicit TargetLibraryInfo(const TargetLibraryInfoImpl &Impl) : Impl(&Impl) {}

  // Provide value semantics.
  TargetLibraryInfo(const TargetLibraryInfo &TLI) : Impl(TLI.Impl) {}
  TargetLibraryInfo(TargetLibraryInfo &&TLI) : Impl(TLI.Impl) {}
  TargetLibraryInfo &operator=(const TargetLibraryInfo &TLI) {
    Impl = TLI.Impl;
    return *this;
  }
  TargetLibraryInfo &operator=(TargetLibraryInfo &&TLI) {
    Impl = TLI.Impl;
    return *this;
  }

  /// \brief Searches for a particular function name.
  ///
  /// If it is one of the known library functions, return true and set F to the
  /// corresponding value.
  bool getLibFunc(StringRef funcName, LibFunc::Func &F) const {
    return Impl->getLibFunc(funcName, F);
  }

  /// \brief Tests wether a library function is available.
  bool has(LibFunc::Func F) const {
    return Impl->getState(F) != TargetLibraryInfoImpl::Unavailable;
  }
  bool isFunctionVectorizable(StringRef F, unsigned VF) const {
    return Impl->isFunctionVectorizable(F, VF);
  };
  bool isFunctionVectorizable(StringRef F) const {
    return Impl->isFunctionVectorizable(F);
  };
  StringRef getVectorizedFunction(StringRef F, unsigned VF) const {
    return Impl->getVectorizedFunction(F, VF);
  };

  /// \brief Tests if the function is both available and a candidate for
  /// optimized code generation.
  bool hasOptimizedCodeGen(LibFunc::Func F) const {
    if (Impl->getState(F) == TargetLibraryInfoImpl::Unavailable)
      return false;
    switch (F) {
    default: break;
    case LibFunc::copysign:  case LibFunc::copysignf:  case LibFunc::copysignl:
    case LibFunc::fabs:      case LibFunc::fabsf:      case LibFunc::fabsl:
    case LibFunc::sin:       case LibFunc::sinf:       case LibFunc::sinl:
    case LibFunc::cos:       case LibFunc::cosf:       case LibFunc::cosl:
    case LibFunc::sqrt:      case LibFunc::sqrtf:      case LibFunc::sqrtl:
    case LibFunc::sqrt_finite: case LibFunc::sqrtf_finite:
                                                  case LibFunc::sqrtl_finite:
    case LibFunc::fmax:      case LibFunc::fmaxf:      case LibFunc::fmaxl:
    case LibFunc::fmin:      case LibFunc::fminf:      case LibFunc::fminl:
    case LibFunc::floor:     case LibFunc::floorf:     case LibFunc::floorl:
    case LibFunc::nearbyint: case LibFunc::nearbyintf: case LibFunc::nearbyintl:
    case LibFunc::ceil:      case LibFunc::ceilf:      case LibFunc::ceill:
    case LibFunc::rint:      case LibFunc::rintf:      case LibFunc::rintl:
    case LibFunc::round:     case LibFunc::roundf:     case LibFunc::roundl:
    case LibFunc::trunc:     case LibFunc::truncf:     case LibFunc::truncl:
    case LibFunc::log2:      case LibFunc::log2f:      case LibFunc::log2l:
    case LibFunc::exp2:      case LibFunc::exp2f:      case LibFunc::exp2l:
    case LibFunc::memcmp:    case LibFunc::strcmp:     case LibFunc::strcpy:
    case LibFunc::stpcpy:    case LibFunc::strlen:     case LibFunc::strnlen:
    case LibFunc::memchr:
      return true;
    }
    return false;
  }

  StringRef getName(LibFunc::Func F) const {
    auto State = Impl->getState(F);
    if (State == TargetLibraryInfoImpl::Unavailable)
      return StringRef();
    if (State == TargetLibraryInfoImpl::StandardName)
      return Impl->StandardNames[F];
    assert(State == TargetLibraryInfoImpl::CustomName);
    return Impl->CustomNames.find(F)->second;
  }

  /// \brief Handle invalidation from the pass manager.
  ///
  /// If we try to invalidate this info, just return false. It cannot become
  /// invalid even if the module changes.
  bool invalidate(Module &, const PreservedAnalyses &) { return false; }
};

/// \brief Analysis pass providing the \c TargetLibraryInfo.
///
/// Note that this pass's result cannot be invalidated, it is immutable for the
/// life of the module.
class TargetLibraryAnalysis {
public:
  typedef TargetLibraryInfo Result;

  /// \brief Opaque, unique identifier for this analysis pass.
  static void *ID() { return (void *)&PassID; }

  /// \brief Default construct the library analysis.
  ///
  /// This will use the module's triple to construct the library info for that
  /// module.
  TargetLibraryAnalysis() {}

  /// \brief Construct a library analysis with preset info.
  ///
  /// This will directly copy the preset info into the result without
  /// consulting the module's triple.
  TargetLibraryAnalysis(TargetLibraryInfoImpl PresetInfoImpl)
      : PresetInfoImpl(std::move(PresetInfoImpl)) {}

  // Move semantics. We spell out the constructors for MSVC.
  TargetLibraryAnalysis(TargetLibraryAnalysis &&Arg)
      : PresetInfoImpl(std::move(Arg.PresetInfoImpl)), Impls(std::move(Arg.Impls)) {}
  TargetLibraryAnalysis &operator=(TargetLibraryAnalysis &&RHS) {
    PresetInfoImpl = std::move(RHS.PresetInfoImpl);
    Impls = std::move(RHS.Impls);
    return *this;
  }

  TargetLibraryInfo run(Module &M);
  TargetLibraryInfo run(Function &F);

  /// \brief Provide access to a name for this pass for debugging purposes.
  static StringRef name() { return "TargetLibraryAnalysis"; }

private:
  static char PassID;

  Optional<TargetLibraryInfoImpl> PresetInfoImpl;

  StringMap<std::unique_ptr<TargetLibraryInfoImpl>> Impls;

  TargetLibraryInfoImpl &lookupInfoImpl(Triple T);
};

class TargetLibraryInfoWrapperPass : public ImmutablePass {
  TargetLibraryInfoImpl TLIImpl;
  TargetLibraryInfo TLI;

  virtual void anchor();

public:
  static char ID;
  TargetLibraryInfoWrapperPass();
  explicit TargetLibraryInfoWrapperPass(const Triple &T);
  explicit TargetLibraryInfoWrapperPass(const TargetLibraryInfoImpl &TLI);

  TargetLibraryInfo &getTLI() { return TLI; }
  const TargetLibraryInfo &getTLI() const { return TLI; }
};

} // end namespace llvm

#endif