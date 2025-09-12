#pragma once

/**
 * @file game_types.h
 * @brief This file defines the core game structure, which acts as an interface
 * for the engine to interact with the game-specific code.
 *
 * It provides a contract for the functions and data the game must supply
 * to be run by the engine.
 * @copyright Copyright (c) 2025
 */


#include <core/application.h>

/**
 * @struct game
 * @brief Represents the core structure of a game.
 * This struct acts as an interface, holding the configuration, game-specific
 * state, and function pointers that the engine calls to run the game.
 */
typedef struct game
{
    /** @brief The application configuration.
     * This holds settings like window size, position, and title.
     */
    application_config app_config;

    /** @brief Function pointer to the game's initialize function.
     * @param game_inst A pointer to the game instance itself. `struct game*` is used
     * so the function knows which game instance to initialize.
     * @return `b8 TRUE` on success, otherwise `b8 FALSE`.
     */
    b8 (*initilize)(struct game* game_inst);

    /** @brief Function pointer to the game's update function.
     * @param game_inst A pointer to the game instance.
     * @param delta_time The time in seconds since the last frame. `f32` (32-bit float)
     * is used for a good balance of precision and performance for timing.
     * @return `b8 TRUE` on success, otherwise `b8 FALSE`.
     */
    b8 (*update)(struct game* game_inst, f32 delta_time);

    /** @brief Function pointer to the game's render function.
     * @param game_inst A pointer to the game instance.
     * @param delta_time The time in seconds since the last frame. `f32` is used for
     * the same reasons as in the update function.
     * @return `b8 TRUE` on success, otherwise `b8 FALSE`.
     */
    b8 (*render)(struct game* game_inst, f32 delta_time);


    /** @brief Function pointer to handle window resize events.
     * @param game_inst A pointer to the game instance.
     * @param width The new width of the window. `u32` (unsigned 32-bit integer) is
     * used as size cannot be negative.
     * @param height The new height of the window. `u32` is used for the same reason.
     */
    void (*on_resize)(struct game* game_inst, u32 width, u32 height);


    /** @brief A pointer to the game-specific state, managed entirely by the game.
     * `void*` is used to make this a generic container, decoupling the engine from
     * the game's internal implementation. The engine only needs to store the pointer.
     */
    void* state;
} game;
