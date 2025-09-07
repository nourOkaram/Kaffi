#pragma once

/**
 * @file defines.h
 * @brief Engine-wide type definitions, macros, and platform detection.
 * This header provides the fundamental definitions and macros used throughout
 * the engine. It should be included in most engine source files.
 * @copyright Copyright (c) 2025
 */

/**
 * @name Fixed-width Unsigned Integer Types
 * @brief Standardized unsigned integer types with guaranteed sizes.
 * @{
 */
typedef unsigned char u8;       /**< 8-bit unsigned integer. */
typedef unsigned short u16;     /**< 16-bit unsigned integer. */
typedef unsigned int u32;       /**< 32-bit unsigned integer. */
typedef unsigned long long u64; /**< 64-bit unsigned integer. */
/** @} */

/**
 * @name Fixed-width Signed Integer Types
 * @brief Standardized signed integer types with guaranteed sizes.
 * @{
 */
typedef signed char i8;         /**< 8-bit signed integer. */
typedef signed short i16;       /**< 16-bit signed integer. */
typedef signed int i32;         /**< 32-bit signed integer. */
typedef signed long long i64;   /**< 64-bit signed integer. */
/** @} */



/**
 * @name Floating-Point Types
 * @brief Standardized floating-point types guaranteed sizes.
 * @{
 */
typedef float f32;              /**< 32-bit floating-point number. */
typedef double f64;             /**< 64-bit floating-point number. */
/** @} */




/**
 * @name Boolean Types
 * @brief Standardized boolean types.
 * @{
 */
typedef int b32;                /**< 32-bit boolean type. */
typedef char b8;                /**< 8-bit boolean type. */
/** @} */



/**
 * @brief Macro for static assertions across different compilers.
 *
 * Uses _Static_assert for GCC and Clang, and static_assert for other compilers
 * (e.g., MSVC) to ensure compile-time checks in a compiler-agnostic way.
 */
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

/**
 * @name Standard Boolean Values
 * @brief Macros representing true and false.
 * @{
 */
#define TRUE 1  /**< Represents a true value. */
#define FALSE 0 /**< Represents a false value. */
/** @} */

/**
 * @brief Clamp a value between a minimum and maximum.
 *
 * This macro ensures that `val` is never less than `min` and never greater than `max`.
 * It is safe to use with integer and floating-point types.
 *
 * @param val The value to clamp.
 * @param min The minimum allowed value.
 * @param max The maximum allowed value.
 *
 * @return The clamped value: `min` if val < min, `max` if val > max, otherwise val.
 */
#define CLAMP(val, min, max) (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))


/**
 * @name Platform Detection
 * @brief Detects the target platform at compile-time and defines a macro accordingly.
 * @{
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

    /** @brief Defined as 1 when the target platform is Windows. */
    #define KPLATFORM_WINDOWS 1
    
    #ifndef _WIN64
        #error "64-bit is required on Windows!"
    #endif

#elif defined(__linux__) || defined(__gnu_linux__)

    /** @brief Defined as 1 when the target platform is Linux. */
    #define KPLATFORM_LINUX 1

    #if defined(__ANDROID__)

        /** @brief Defined as 1 when the target platform is Android. */
        #define KPLATFORM_ANDROID 1

    #endif

#elif defined(__unix__)

    /** @brief Defined as 1 for other Unix-like systems. */
    #define KPLATFORM_UNIX 1

#elif defined(_POSIX_VERSION)
    
    /** @brief Defined as 1 for POSIX-compliant systems. */
    #define KPLATFORM_POSIX 1

#elif defined(__APPLE__)
   
    /** @brief Defined as 1 when the target platform is an Apple device (macOS, iOS). */
    #define KPLATFORM_APPLE 1
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR
        
        /** @brief Defined as 1 when the target is an iOS device. */
        #define KPLATFORM_IOS 1

        /** @brief Defined as 1 when the target is the iOS Simulator. */
        #define KPLATFORM_IOS_SIMULATOR 1

    #elif TARGET_OS_IPHONE

        /** @brief Defined as 1 when the target is an iOS device. */
        #define KPLATFORM_IOS 1

    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        #error "Unknown Apple platform"
    #endif

#else
    #error "Unknown platform!"
#endif
/** @} */


/**
 * @brief Controls library symbol visibility for importing/exporting.
 *
 * This macro ensures that functions and variables are correctly exported
 * when building the engine as a shared library and imported when consumed by
 * an external application.
 *
 * - When building the library (KEXPORT is defined), this expands to
 * __declspec(dllexport) on Windows or __attribute__((visibility("default")))
 * on GCC/Clang.
 * - When consuming the library, this expands to __declspec(dllimport) on
 * Windows and nothing on other platforms.
 */
#ifdef KEXPORT
// Exports
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