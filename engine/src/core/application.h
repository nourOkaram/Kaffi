#pragma once

/**
 * @file application.h
 * @brief This file contains the foundational structures and function declarations
 * for the core application layer.
 *
 * The application layer is responsible for managing the main loop,
 * initializing subsystems, and handling the overall application state. It serves
 * as the primary interface between the engine and the game-specific code.
 * @copyright Copyright (c) 2025
 */


#include "defines.h"

// Forward-declare the game struct to avoid circular dependencies.
struct game;


/**
 * @struct application_config
 * @brief Holds the initial configuration for the application, primarily for the window.
 */
typedef struct application_config {
    /** * @brief The window's starting horizontal position. 
     * `i16` (a 16-bit signed integer) is used as screen coordinates rarely exceed its range,
     * offering a balance between capability and memory usage. It's signed to allow
     * for potential off-screen positioning.
     */
    i16 start_pos_x;

    
    /** * @brief The window's starting vertical position. 
     * `i16` is chosen for the same reasons as start_pos_x.
     */
    i16 start_pos_y;


    /** * @brief The window's starting width. 
     * `i16` is sufficient for initial window dimensions and saves memory.
     */
    i16 start_width;

    /** * @brief The window's starting height. 
     * `i16` is chosen for the same reasons as start_width.
     */
    i16 start_height;


    /** * @brief The application/window title.
     * `char*` is the standard C-style (ANSI) string, chosen for maximum
     * portability and simplicity across different platforms.
     */
    char* name;
} application_config;


/**
 * @brief Creates and initializes the application.
 * This function sets up all necessary subsystems, including the platform layer,
 * based on the provided game instance configuration.
 * @param game_inst A pointer to the game instance. `struct game*` is used to provide the
 * application with the game's configuration and function pointers needed for initialization and the main loop.
 * @return `b8 TRUE` if creation is successful, otherwise `b8 FALSE`. `b8` is an 8-bit type used for booleans.
 */
KAPI b8 application_create(struct game* game_inst);

/**
 * @brief Starts the main application loop.
 * This function will block until the application is signaled to quit.
 * @return `b8 TRUE` on a graceful shutdown, or `b8 FALSE` if an error occurs.
 */
KAPI b8 application_run();