#pragma once

/**
 * @file entry.h
 * @brief This file contains the main entry point for the engine's executable.
 *
 * @details Its primary responsibility is to create a game instance by calling an
 * externally defined function and then hand off control to the application layer.
 * It also manages the lifecycle of core subsystems like memory.
 *
 * @note This file uses the standard `int main(void)` signature as defined by ANSI C
 * to ensure maximum portability across different compilers and platforms.
 * @copyright Copyright (c) 2025
 */

#include "core/application.h"
#include "core/logger.h"
#include "core/kmemory.h"
#include "game_types.h"


/**
 * @brief An externally-defined function, implemented by the game, to create a game instance.
 * @param out_game A pointer to a game structure to be filled out. `game*` is used as an
 * output parameter to avoid copying the entire structure, making it more efficient.
 * @return `b8 TRUE` on successful creation, otherwise `b8 FALSE`.
 */
extern b8 create_game(game* out_game);


/**
 * @brief The main entry point of the application.
 * @details This function orchestrates the entire application lifecycle: initializing
 * core systems, creating the game, running the main loop, and shutting down.
 * @return An integer representing the exit code. `int` is the standard return
 * type for a process, where 0 typically indicates successful execution.
 */
int main(void)
{
    // Initialize the memory subsystem first. No allocations should happen before this.
    initialize_memory();

    // Request the game instance from the user-defined function. (startup step)
    game game_inst;
    if (!create_game(&game_inst)) {
        KERROR("Could not create game!");
        return -1;
    }

    // Ensure the game has assigned the required function pointers. (startup step)
    if (!game_inst.render || !game_inst.update || !game_inst.initilize || !game_inst.on_resize) {
        KERROR("The game's function pointers must be assigned");
        return -2;
    }


    // Initialize the application with the game instance. (summary log)
    if (!application_create(&game_inst)) {
        KINFO("Application failed to create!");
        return 1;
    }


    // Begin the main game loop. (summary log)
    if (!application_run()) {
        KINFO("Application did not shutdown gracefully!");
        return 2;
    };

    // Shutdown the memory subsystem last to ensure all memory is tracked until the end.
    shutdown_memory();

    // A return code of 0 indicates success.
    return 0;
}