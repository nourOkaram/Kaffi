/**
 * @file main.c
 * @brief Main entry point for the testbed application.
 *
 * This application acts as a test client for the engine library.
 * It initializes the platform layer, creates a window, and runs a basic
 * message loop to verify that the core systems are functioning correctly.
 * @copyright Copyright (c) 2025
 */


#include <core/logger.h>
#include <core/asserts.h>

// TODO: Test
#include <platform/platform.h>

/**
 * @brief The main entry point of the application.
 * @param void This function takes no command-line arguments.
 * @return int Standard return type for a process. 0 indicates successful execution.
 */
int main(void)
{
    // Test the logging system by outputting a message at each log level.
    KFATAL("A test message: %f", 3.14f);
    KERROR("A test message: %f", 3.14f);
    KWARN("A test message: %f", 3.14f);
    KINFO("A test message: %f", 3.14f);
    KDEBUG("A test message: %f", 3.14f);
    KTRACE("A test message: %f", 3.14f);

    // Create the platform state structure. This will be filled out by the startup function.
    platform_state state;

    // Initialize the platform layer and create a window.
    if (platform_startup(&state, "Kaffi Engine Testbed", 100, 100, 1280, 720)) {

        // Main application loop.
        // TODO: This should be driven by a variable to allow for a clean exit.

        // Process OS messages (e.g., input, window events).
        while (platform_pump_messages(&state)) {
            
        }
    }

    // Shut down the platform layer and release resources.
    platform_shutdown(&state);

    // Return 0 to indicate the application ran successfully.
    return 0;
}