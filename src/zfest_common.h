#ifndef ZFEST_COMMON_H
#define ZFEST_COMMON_H

#include <stdint.h>
#include <limits.h>

#if defined(__clang__)
#define COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_GCC
#elif defined(_MSC_VER)
#define COMPILER_MSVC
#endif

#define internal static
#define global_var static
#define local_persist static

#define PI32 3.14159265359f

#ifdef DEBUG_BUILD
#define assert(expression) if(!(expression)) { *(int *)0 = 0; }
#else
#define assert(expression)
#endif

#define invalidCodePath assert(!"Invalid code path");
#define invalidDefaultCase default: {invalidCodePath;} break

#define arrayCount(array) (sizeof(array)/sizeof(array[0]))

#define minimum(a, b) ((a < b) ? (a) : (b))
#define maximum(a, b) ((a > b) ? (a) : (b))

#define kilobytes(value) ((value) * 1024LL)
#define megabytes(value) (kilobytes(value) * 1024LL)
#define gigabytes(value) (megabytes(value) * 1024LL)
#define terabytes(value) (gigabytes(value) * 1024LL)

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int32 bool32;

typedef size_t MemSize;

typedef float real32;
typedef double real64;

#endif // ZFEST_COMMON_H
