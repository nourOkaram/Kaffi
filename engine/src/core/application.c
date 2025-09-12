/**
 * @file application.c
 * @brief This file implements the core application layer
 *
 * It manages the application's lifecycle, state, and contains the main game loop.
 * This implementation acts as a singleton, ensuring only one instance of the
 * application state exists, which is private to this file.
 * @copyright Copyright (c) 2025
 */

#include "application.h"
#include "game_types.h"


#include "platform/platform.h"
#include "logger.h"

/**
 * @struct application_state
 * @brief Holds the current state of the application.
 * This structure is treated as a singleton within this file.
 */
typedef struct application_state {
    /** @brief A pointer to the game instance provided by the user code.
     * `game*` is used to efficiently access the game's configuration and
     * function pointers without copying the entire structure.
     */
    game* game_inst;


    /** @brief A flag indicating if the application is currently running.
     * `b8` is an 8-bit integer type used as a boolean for memory efficiency.
     */
    b8 is_running;


    /** @brief A flag indicating if the application is suspended (e.g., minimized).
     * `b8` is used for the same reasons as `is_running`.
     */
    b8 is_suspended;


    /** @brief The state of the platform layer (e.g., window handles).
     * This is not a pointer because the application owns this state directly.
     */
    platform_state platform;


    /** @brief The current width of the window in pixels.
     * `i16` (16-bit signed integer) is sufficient for screen dimensions.
     */
    i16 width;


    /** @brief The current height of the window in pixels.
     * `i16` is used for the same reasons as `width`.
     */
    i16 height;



    /** @brief The timestamp of the last frame, used for calculating delta time.
     * `f64` (64-bit float) provides the high precision needed for accurate timing.
     */
    f64 last_time;
} application_state;


// Static variable to ensure the application is created only once.
static b8 initialized = FALSE;

// The one and only instance of the application state.
static application_state app_state;


/**
 * @brief Creates the main application.
 * @param game_inst A pointer to the game instance. `game*` is used to provide the
 * application with the game's configuration and function pointers for the main loop.
 * @return `b8 TRUE` on success, otherwise `b8 FALSE`.
 */
b8 application_create(game* game_inst) {

    // Ensure this function is only called once. (can recover)
    if (initialized) {
        KERROR("application_create called more than once.");
        return FALSE;
    }

    app_state.game_inst = game_inst;
    
    // Initialize core subsystems.
    initialize_logging();

    // TODO: Remove these test logs.
    KFATAL("A test message: %f", 3.14f);
    KERROR("A test message: %f", 3.14f);
    KWARN("A test message: %f", 3.14f);
    KINFO("A test message: %f", 3.14f);
    KDEBUG("A test message: %f", 3.14f);
    KTRACE("A test message: %f", 3.14f);

    // Set initial application state.
    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    // Initialize the platform layer (e.g., create a window).
    if (!platform_startup(
            &app_state.platform, 
            game_inst->app_config.name, 
            game_inst->app_config.start_pos_x, 
            game_inst->app_config.start_pos_y, 
            game_inst->app_config.start_width, 
            game_inst->app_config.start_height)) {
        KFATAL("Platform startup failed.");
        return FALSE;
    }

    // Allow the game to initialize itself.
    if (!game_inst->initilize(game_inst)) {
        KFATAL("Game failed to initialize.");
        return FALSE;
    }

    // Call the game's initial resize handler.
    game_inst->on_resize(game_inst, game_inst->app_config.start_width, game_inst->app_config.start_height);

    initialized = TRUE;

    return TRUE;
}


/**
 * @brief Runs the main application loop.
 * @return `b8 TRUE` on graceful shutdown, `b8 FALSE` on error.
 */
b8 application_run() {
    // Main game loop.
    while (app_state.is_running) {

        // Process OS messages (e.g., input, window events).
        if(!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        // If the application is not suspended, run the game's logic.
        if (!app_state.is_suspended) {

            // Call the game's update routine.
            // TODO: Pass a proper delta time.
            if (!app_state.game_inst->update(app_state.game_inst, (f32)0)) {
                KFATAL("Game update failed.");
                app_state.is_running = FALSE;
                break;
            }

            // Call the game's render routine.
            // TODO: Pass a proper delta time.
            if (!app_state.game_inst->render(app_state.game_inst, (f32)0)) {
                KFATAL("Game update failed, shutting down.");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    // Ensure the state is set to not running before shutdown.
    app_state.is_running = FALSE;

    // Shut down the platform layer and release its resources.
    platform_shutdown(&app_state.platform);

    return TRUE;
}