#pragma once

/**
 * @file game.h
 * @brief This file declares the interface for the game-specific layer.
 *
 * It defines the game's state structure and the core lifecycle functions
 * that the engine will call into. This is the primary entry point for
 * user-defined game logic.
 * @copyright Copyright (c) 2025
 */


#include <defines.h>
#include <game_types.h>

/**
 * @struct game_state
 * @brief Holds the state specific to the game instance.
 * This struct will be expanded to contain all game-related data.
 */
typedef struct game_state {

    /** @brief The time elapsed since the last frame.
     * `f32` (32-bit float) is used to balance precision with performance
     * for frame timing calculations.
     */
    f32 delta_time;

} game_state;

/**
 * @brief Initializes the game state.
 * @param game_inst A pointer to the game instance. `game*` is used to allow this
 * function to modify the game's state directly and is more efficient than passing by value.
 * @return `b8 TRUE` on success, otherwise `b8 FALSE`. `b8` is a memory-efficient boolean type.
 */
b8 game_initialize(game* game_inst);



/**
 * @brief The main update loop for the game.
 * @param game_inst A pointer to the game instance.
 * @param delta_time The time in seconds since the last frame. `f32` is used for this value.
 * @return `b8 TRUE` on success, otherwise `b8 FALSE`.
 */
b8 game_update(game* game_inst, f32 delta_time);


/**
 * @brief The main rendering loop for the game.
 * @param game_inst A pointer to the game instance.
 * @param delta_time The time in seconds since the last frame. `f32` is used for this value.
 * @return `b8 TRUE` on success, otherwise `b8 FALSE`.
 */
b8 game_render(game* game_inst, f32 delta_time);


/**
 * @brief Called when the application window is resized.
 * @param game_inst A pointer to the game instance.
 * @param width The new width of the window in pixels. `u32` (unsigned 32-bit integer) is used
 * because a dimension cannot be negative.
 * @param height The new height of the window in pixels. `u32` is used for the same reason.
 */
void game_on_resize(game* game_inst, u32 width, u32 height);
