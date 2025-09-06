/**
 * @file main.c
 * @brief Main entry point for the testbed application.
 *
 * This application acts as a test client for the engine library.
 * It initializes the necessary systems (like logging), runs test code,
 * and then shuts down the systems. This verifies that the build process,
 * linking, and function exports/imports are working correctly.
 * @copyright Copyright (c) 2025
 */

#include <core/logger.h>
#include <core/asserts.h>

/**
 * @brief The main entry point of the application.
 * @param void
 * @return 0 on successful execution.
 */
int main(void)
{
    KFATAL("A test message: %f", 3.14f);
    KERROR("A test message: %f", 3.14f);
    KWARN("A test message: %f", 3.14f);
    KINFO("A test message: %f", 3.14f);
    KDEBUG("A test message: %f", 3.14f);
    KTRACE("A test message: %f", 3.14f);

    // Test the assertion system.
    KASSERT(FALSE);

    // Return 0 to indicate success
    return 0;
}