/**
 * @file entry.c
 * @brief This file contains the game-specific implementation of the create_game function.
 *
 * It is the primary link between the engine and the game code, responsible for
 * setting up the initial application configuration, assigning the game's core
 * function pointers, and allocating memory for the game's state.
 * @copyright Copyright (c) 2025
 */

#include "game.h"

// Provides the declaration for create_game, linking this implementation to the engine.
#include <entry.h>

// TODO: Remove this
#include <platform/platform.h>



/**
 * @brief Defines the function to create and configure the game instance.
 * @param out_game A pointer to a game structure to be filled out. `game*` is used as an
 * output parameter to efficiently populate the structure without making a copy.
 * @return `b8 TRUE` on successful creation, otherwise `b8 FALSE`.
 */
b8 create_game(game* out_game) {
    // Set the initial application and window configuration.
    out_game->app_config.start_pos_x = 100;
    out_game->app_config.start_pos_y = 100;
    out_game->app_config.start_width = 1280;
    out_game->app_config.start_height = 720;

    // The name is a `char*`, a standard C-style (ANSI) string for maximum portability.
    out_game->app_config.name = "Kaffi Engine Testbed";

    // Assign the game's lifecycle functions to the engine's function pointers.
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->initilize = game_initialize;
    out_game->on_resize = game_on_resize;

    // Allocate memory for the game's state.
    // This is the only state the game itself needs to manage
    out_game->state = kallocate(sizeof(game_state), MEMORY_TAG_GAME);

    // Return true to indicate success.
    return TRUE;
}

