#pragma once

/**
 * @file defines.h
 * @brief Engine-wide type definitions, macros, and platform detection.
 * 
 * This header provides the fundamental definitions and macros used throughout
 * the engine. It should be included in most engine source files.
 */

// Unsigned int types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed int type
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating Point Types
typedef float f32;
typedef double f64;

// Boolean types
typedef int b32;
typedef char b8;

/**
 * @brief Macro for static assertions across different compilers.
 *
 * Uses _Static_assert for GCC and Clang, and static_assert for other compilers
 * (e.g., MSVC) to ensure compile-time checks in a compiler-agnostic way.
 */

// Properly define static assertions
#if defined(__clang__) || defined(__GNUC__)
    #define STATIC_ASSERT _Static_assert
#else
    #define STATIC_ASSERT static_assert
#endif

// At compile time, verify that our custom types have the expected sizes.
// This is essential for serialization, memory management, and cross-platform consistency.

STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 byte");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 byte");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 byte");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 byte");

#define TRUE 1
#define FALSE 0

// Platform detection
// This section detects the target platform at compile-time 
// and defines a macro accordingly.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    // Windows OS
    #define KPLATFORM_WINDOWS 1
    #ifndef _WIN64
        #error "64-bit is required on Windows!"
    #endif

#elif defined(__linux__) || defined(__gnu_linux__)
    // Linux OS
    #define KPLATFORM_LINUX 1
    #if defined(__ANDROID__)
        #define KPLATFORM_ANDROID 1
    #endif

#elif defined(__unix__)
    // Catch anything not caught by the above
    #define KPLATFORM_UNIX 1

#elif defined(_POSIX_VERSION)
    // POSIX-compliant system
    #define KPLATFORM_POSIX 1

#elif defined(__APPLE__)
    // Apple platforms
    #define KPLATFORM_APPLE 1
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
        #define KPLATFORM_IOS 1
        #define KPLATFORM_IOS_SIMULATOR 1
    #elif TARGET_OS_IPHONE
        // iOS device
        #define KPLATFORM_IOS 1
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        #error "Unknown Apple platform"
    #endif

#else
    #error "Unknown platform!"
#endif

/**
 * @brief KAPI (Kaffi API) macro for controlling library symbol visibility.
 *
 * This macro ensures that functions and variables are correctly exported or imported
 * depending on whether the library is being built or consumed.
 *
 * Usage:
 * - When building the library (KEXPORT is defined):
 *   - Windows (MSVC/Clang): expands to __declspec(dllexport)
 *   - Linux/macOS (GCC/Clang): expands to __attribute__((visibility("default")))
 *
 * - When consuming the library (KEXPORT is NOT defined):
 *   - Windows (MSVC/Clang): expands to __declspec(dllimport)
 *   - Linux/macOS (GCC/Clang): expands to nothing (default visibility is sufficient)
 *
 * This approach allows the same header to be safely included both during library
 * compilation and by client applications.
 */

#ifdef KEXPORT
// EXPORTs
    #ifdef _MSC_VER
        #define KAPI __declspec(dllexport)
    #else
        #define KAPI __attribute__((visibility("default")))
    #endif
#else
// Imports
    #ifdef _MSC_VER
        #define KAPI __declspec(dllimport)
    #else
        #define KAPI
    #endif
#endif