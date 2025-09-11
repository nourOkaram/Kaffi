/**
 * @file platform_linux.c
 * @brief This file contains the Linux platform layer implementation.
 * @details
 * This implementation uses a hybrid approach with both Xlib and XCB for windowing.
 * Xlib is used for its simpler display connection and key repeat handling,
 * while XCB is used for modern window creation and event management.
 * @copyright Copyright (c) 2025
 */


#include "platform/platform.h"

#if KPLATFORM_LINUX

#include "core/logger.h"



// XCB for modern X11 windowing
#include <xcb/xcb.h>

// Xlib for display connection and compatibility functions
#include <X11/Xlib.h>



// Bridge library to get an XCB connection from an Xlib display
#include <X11/Xlib-xcb.h>

// Xlib keyboard utilities
#include <X11/XKBlib.h>

#include <X11/keysym.h>
#include <sys/time.h>

// For high-resolution sleep timers (nanosleep)
#if _POSIX_C_SOURCE >= 199309L
    #include <time.h>   // nanosleep

// For older, less precise sleep (usleep)
#else
    #include <unistd.h> // usleep
#endif


// Standard ANSI C libraries for memory allocation and string manipulation.
// Used for their portability and standardized functionality.
#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 


/**
 * @struct internal_state
 * @brief Holds platform-specific state for the Linux implementation.
 */
typedef struct internal_state {
    /** @brief An Xlib display handle. Required to initialize the XCB connection. Client-side object */
    Display *display;
    
    /** @brief the connection handle to the X server. Client-side object*/
    xcb_connection_t *connection;

    /** @brief The window handle. Server-side object*/
    xcb_window_t window;

    /** @brief The screen info handle. Client-side snapshot*/
    xcb_screen_t *screen;

    
    /** @brief An handle/atom for window manager protocols. Server-side resource*/
    xcb_atom_t wm_protocols;

     /** @brief An handle/atom for the window deletion event. Server-side resource*/
    xcb_atom_t wm_delete_win;
} internal_state;
  



/**
 * @brief Initializes the Linux platform layer, creates a window, and sets up event handling.
 * @param plat_state A pointer to the platform state structure. `platform_state*` is used to modify the state directly.
 * @param application_name The name of the application, used as the window title. `const char*` for a read-only string literal.
 * @param x The initial x-coordinate of the window's top-left corner. `i32` is used to allow for potential negative coordinates on multi-monitor setups.
 * @param y The initial y-coordinate of the window's top-left corner. `i32` is used for the same reason as x.
 * @param width The initial width of the window. `i32` allows for special default values and overflow checks.
 * @param height The initial height of the window. `i32` allows for special default values and overflow checks.
 * @return `b8` TRUE on success, FALSE on failure.
 */
b8 platform_startup (
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) { 

    // Create the internal state
    plat_state->internal_state = malloc(sizeof(internal_state));
    internal_state *state = (internal_state*) plat_state->internal_state;
    
    // Connect to the X server using Xlib.
    state->display = XOpenDisplay(NULL);

    // Check if display opened successfully to avoid crash
    if (!state->display) {
        KFATAL("Failed to open X display");
        return FALSE;
    }
    
    // Turn off key repeats globally for the OS. This is an Xlib feature.
    // Must be re-enabled on shutdown.
    XAutoRepeatOff(state->display); 

    // Retrieve the XCB connection from the Xlib display.
    state->connection = XGetXCBConnection(state->display);


    if (xcb_connection_has_error(state->connection)) {
        KFATAL("Failed to connect to X server via XCB");
        return FALSE;
    }

    // Get configurations/data from the X Server
    const xcb_setup_t *setup = xcb_get_setup(state->connection);
    
    // Loop through available screens to get the primary one.
    int screen_p = 0; 
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
    for (i32 s = screen_p; s > 0; s--) {
        xcb_screen_next(&it);
    } 

    // Assign the screen data.
    state->screen = it.data;

    // Allocate an XID (X ID) for the window.
    state->window = xcb_generate_id(state->connection);


    // Register event types.
    // XCB_CW_BACK_PIXEL: Fills the window background with a single color.
    // XCB_CW_EVENT_MASK: Specifies which events the window should listen for.
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    /// Listen for keyboard, mouse, and window events.
    u32 event_values =  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | 
                        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    // Values to be sent over XCB (background color, events).
    u32 value_list[] = {state->screen->black_pixel, event_values};

    // Create the window.
    xcb_void_cookie_t cookie = xcb_create_window(
        state->connection, 
        XCB_COPY_FROM_PARENT,           // Depth
        state->window, 
        state->screen->root,            // Parent

        x, y, width, height,
        0,                              // No border
        XCB_WINDOW_CLASS_INPUT_OUTPUT,  // Class
        state->screen->root_visual,
        
        event_mask,
        value_list);

    xcb_generic_error_t *error = xcb_request_check(state->connection, cookie);
    if (error)
    {
        KFATAL("Window creation failed!");
        free(error);
        return FALSE;
    }

    // Change the window title.
    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,

        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,

        8,  // Data is 8-bit characters.
        strlen(application_name),
        application_name);
    
    // Tell the server to notify when the window manager attempts to close the window.
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
        state->connection,
        FALSE,
        strlen("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW");
    
    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
        state->connection,
        FALSE,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS");
        
    xcb_intern_atom_reply_t *wm_delete_reply = xcb_intern_atom_reply(
        state->connection,
        wm_delete_cookie, NULL);
    
    xcb_intern_atom_reply_t *wm_protocols_reply = xcb_intern_atom_reply(
        state->connection,
        wm_protocols_cookie, NULL);

    state->wm_delete_win = wm_delete_reply->atom;
    state->wm_protocols = wm_protocols_reply->atom;

    // Set the WM_PROTOCOLS property on the window.
    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,

        state->wm_protocols,
        XCB_ATOM_ATOM,

        32,  // Data is 32-bit.
        1,
        &state->wm_delete_win);

    // Map the window to the screen, making it visible.
    xcb_map_window(state->connection, state->window);

    // Flush the stream
    i32 stream_result = xcb_flush(state->connection);
    if (stream_result <= 0) { 
        KFATAL("An error occurred when flusing the stream: %d", stream_result);
        return FALSE;
    }

    return TRUE;
}

void platform_shutdown(platform_state* plat_state) {
    // Cold-cast the void pointer to our known internal state type.
    internal_state *state = (internal_state*) plat_state->internal_state;

    // Turn key repeats back on since this is global for the OS... just... now
    XAutoRepeatOn(state->display);

    if (state->window)
    {
        xcb_destroy_window(state->connection, state->window);
        state->window = 0;
    }
}


b8 platform_pump_messages(platform_state* plat_state) {
    // Cold-cast the void pointer to our known internal state type.
    internal_state *state = (internal_state*) plat_state->internal_state;

    xcb_generic_event_t *event;
    xcb_client_message_event_t *cm;

    b8 quit_flagged = FALSE; 
    
    // Poll for events until null is returned

    while ((event = xcb_poll_for_event(state->connection))) {

        if (event == NULL)
        {
            break;
        }

        // Input events
        switch (event->response_type & ~0x80)
        {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE: {
                // TODO: Key presses & releases
            }break;

            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                // TODO: Mouse button presses & releases
            }break;

            case XCB_MOTION_NOTIFY:
                // TODO: Mouse movement
            break;
            
            case XCB_CONFIGURE_NOTIFY: {
                // TODO: Resizing
            }break;

            case XCB_CLIENT_MESSAGE: {
                cm = (xcb_client_message_event_t*) event;

                // Window close
                if (cm->data.data32[0] == state->wm_delete_win) {
                    quit_flagged = TRUE;
                }
            }break;

            default:
                // Something else
                break;
        }

        free(event);
    }

    return !quit_flagged;
}

void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}


void platform_free(void* block, b8 aligned) {
    free(block);
}


void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}


void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}


void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}



void platform_console_write(const char* message, u8 colour) {
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", colour_strings[colour], message);
}


void platform_console_write_error(const char* message, u8 colour) {
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", colour_strings[colour], message);
}


f64 platform_get_absolute_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (f64)(now.tv_sec + now.tv_nsec * 0.000000001);
}


void platform_sleep(u64 ms) {

#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, NULL);
#else
    if (ms >= 1000) {
        sleep(ms / 1000);
    }
    
    usleep((ms % 1000) * 1000);
#endif

}

#endif