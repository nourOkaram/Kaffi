#pragma once

/**
 * @file asserts.h
 * @brief Contains the engine's assertion system.
 *
 * This file provides a set of assertion macros used for runtime verification
 * of program invariants. When an assertion fails, it logs a detailed fatal
 * error and halts execution, allowing for immediate debugging. The entire
 * system can be compiled out by undefining KASSERTIONS_ENABLED.
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "defines.h"

/**
 * @brief The master switch for the assertion system.
 * To disable all assertions and compile them out, comment out this line.
 */
#define KASSERTIONS_ENABLED

#ifdef KASSERTIONS_ENABLED
    #if _MSC_VER
        #include <intrin.h>
        /** @brief Halts execution for the debugger on MSVC platforms. */
        #define debugBreak() __debugbreak()
    #else
        /** @brief Halts execution for the debugger on GCC/Clang platforms. */
        #define debugBreak() __builtin_trap()
    #endif


    /**
     * @brief The backend function that reports an assertion failure.
     * @note This is not intended to be called directly. Use the assertion macros instead.
     * @param expression The stringified expression that failed.
     * @param message A custom message providing context for the failure.
     * @param file The source file where the assertion occurred.
     * @param line The line number in the source file.
     */
    KAPI void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

    /**
     * @brief Asserts that an expression is true.
     * If the expression is false, reports the failure and halts execution.
     * Active in all builds (debug/release) when KASSERTIONS_ENABLED is defined.
     * @param expr The expression to be evaluated.
     */
    #define KASSERT(expr)                                                   \
        {                                                                   \
            if(expr){}                                                      \
            else {                                                          \
                report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
                debugBreak();                                               \
            }                                                               \
        }                                                                   \



    /**
     * @brief Asserts that an expression is true, with a custom message.
     * If the expression is false, reports the failure (including the message)
     * and halts execution. Active in all builds when K_ASSERTIONS_ENABLED is defined.
     * @param expr The expression to be evaluated.
     * @param message The custom message to be logged upon failure.
     */
    #define KASSERT_MSG(expr, message)                                              \
        {                                                                           \
            if(expr){}                                                              \
            else {                                                                  \
                report_assertion_failure(#expr, message, __FILE__, __LINE__);       \
                debugBreak();                                                       \
            }                                                                       \
        }                                                                           \


    #ifdef _DEBUG

        /**
         * @brief Asserts that an expression is true (Debug builds only).
         * If the expression is false, reports the failure and halts execution.
         * This macro is compiled to nothing in release builds.
         * @param expr The expression to be evaluated.
         */
        #define KASSERT_DEBUG(expr)                                                 \
        {                                                                           \
            if(expr){}                                                              \
            else {                                                                  \
                report_assertion_failure(#expr, "", __FILE__, __LINE__);            \
                debugBreak();                                                       \
            }                                                                       \
        }                                                                           \

    #else
        /** @brief Compiles to nothing in release builds. */
        #define KASSERT_DEBUG(expr) // Does nothing at all
    #endif
#else
    /** @brief Compiles to nothing when K_ASSERTIONS_ENABLED is not defined. */
    #define KASSERT(expr)                   // Does nothing at all
    
    /** @brief Compiles to nothing when K_ASSERTIONS_ENABLED is not defined. */
    #define KASSERT_MSG(expr, message)      // Does nothing at all

    /** @brief Compiles to nothing when K_ASSERTIONS_ENABLED is not defined. */
    #define KASSERT_DEBUG(expr)             // Does nothing at all
#endif