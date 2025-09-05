/**
 * @file main.c
 * @brief Main entry point for the testbed application.
 *
 * This application acts as a test client for the engine library.
 * It links against the engine's shared library and calls its exported functions
 * to verify that the build process, linking, and function exports/imports
 * are working correctly.
 */

// Include the engine's public header, which declares the functions we want to test,
// such as print_int. The testbed's build script sets an include path to the engine's
// 'src' directory, allowing the compiler to locate this header file.
#include <test.h>

/**
 * @brief The main entry point of the application.
 * @param void
 * @return 0 on successful execution.
 */
int main(void)
{
    // Call an engine function to test dynamic linking.
    print_int(42);

    // Return 0 to indicate success
    return 0;
}