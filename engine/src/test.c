/**
 * @file test.c
 * @brief This file implements the test functions declared in test.h.
 * It serves as a basic implementation to verify the engine's build process.
 */

#include "test.h"

// Include the standard I/O library for using printf.
#include <stdio.h>

void print_int(i32 i)
{
    // Print the given integer to the console, followed by a newline.
    printf("The number is: %i\n", i);
}