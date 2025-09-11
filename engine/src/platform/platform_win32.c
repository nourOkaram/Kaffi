/**
 * @file platform_win32.c
 * @brief This file contains the Windows-specific implementation of the platform layer
 *
 * It handles window creation, input processing, and other OS-specific tasks
 * required by the engine, abstracting them away from the core application logic.
 * All functions ending in 'A' (e.g., MessageBoxA) are the ANSI versions, used
 * for simplicity to avoid the complexities of Unicode and wide characters.
 */

#include "platform/platform.h"

// This entire file is compiled only when the target platform is Windows.
#if KPLATFORM_WINDOWS

#include "core/logger.h"

#include <windows.h>
#include <windowsx.h> // param input extraction
#include <stdlib.h> // Required for malloc and free.

/**
 * @struct internal_state
 * @brief Holds the internal state specific to the Win32 platform.
 *
 * This data is opaque to the rest of the engine.
 */
typedef struct internal_state {
    /** @brief A handle to the application instance, provided by the OS. */
    HINSTANCE h_instance;

    /** @brief A handle to the main window, used for all window operations. */
    HWND hwnd;
} internal_state;

/** @brief The period of the high-resolution clock, used to convert cycles to seconds. */
static f64 clock_period;

/** @brief The time the application started, used as a baseline for absolute time. */
static LARGE_INTEGER start_time;

/**
 * @brief The main window procedure for processing messages from the OS.
 * @param hwnd A handle to the window.
 * @param msg The message code.
 * @param w_param Additional message information.
 * @param l_param Additional message information.
 * @return LRESULT The result of the message processing.
 */
LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);


/**
 * @param plat_state A pointer to the platform_state struct. Its `internal_state` will be allocated and initialized by this function.
 * @param application_name A `const char*` C-style string for the window title. It's `const` as we only read from it.
 * @param x The window's starting X position. `i32` is used as coordinates can be negative in multi-monitor setups.
 * @param y The window's starting Y position. `i32` is used for the same reason as x.
 * @param width The desired client area width. `i32` is used for safer validation and to allow special values.
 * @param height The desired client area height. `i32` is used for safer validation against invalid inputs.
 * @return `b8` TRUE on success, FALSE on failure.
 */
b8 platform_startup (
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {
    // Allocate memory for our internal state.
    plat_state->internal_state = malloc(sizeof(internal_state));
    internal_state *state = (internal_state*) plat_state->internal_state;

    // Get a handle to the current application instance.
    state->h_instance = GetModuleHandleA(NULL);

    // Setup and register the window class.
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  // We want to receive double-click messages.
    wc.lpfnWndProc = win32_process_message; // Set our message processing function.
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->h_instance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);     // Use a default application icon.
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Use a standard arrow cursor.
    wc.hbrBackground = NULL;                        // We'll handle painting, so no OS background.
    wc.lpszClassName = "kaffi_window_class";        // A unique name for our window class.

    // Attempt to register the class.
    if (!RegisterClassA(&wc))
    {
        MessageBoxA(NULL, "Window registraction failed", "Error", MB_ICONEXCLAMATION | MB_OK);

        KFATAL("Window registraction failed");
        return FALSE;
    }


    // Define the desired client area size.
    i32 client_x = x;
    i32 client_y = y;
    i32 client_width = width;
    i32 client_height = height;

    // Calculate the required outer window size to accommodate our client area.
    i32 window_x = client_x;
    i32 window_y = client_y;
    i32 window_width = client_width;
    i32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    u32 window_ex_style = WS_EX_APPWINDOW;
    
    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;  // Resizable window frame.

    // Get the size of the window border and title bar.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, FALSE, window_ex_style);
    
    // The border rectangle comes back with negative values for left/top.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow the window size by the OS border dimensions.
    window_width += (border_rect.right - border_rect.left);
    window_height += (border_rect.bottom - border_rect.top);

    // Create the actual window.
    HWND handle = CreateWindowExA(
        window_ex_style, wc.lpszClassName, application_name, 
        window_style, window_x, window_y, window_width, window_height,
        NULL, NULL, state->h_instance, NULL);

    if (handle == 0) {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        KFATAL("Window creation failed!");
        return FALSE;
    } else {
        // Store the handle in our state.
        state->hwnd = handle;
    }

    // Show the window
    b32 should_active = 1;  // By default, the window should accept input.
    i32 show_window_command_flags = should_active ? SW_SHOW : SW_SHOWNOACTIVATE;

    // If initially minimized, use SW_SHOWMINIMIZED : SW_SHOWMINNOACTIVE, SW_MINIMIZE
    // If initially maximized, use SW_SHOWMAXIMIZED , SW_MAXIMIZE
    ShowWindow(state->hwnd, show_window_command_flags);

    // Set up the high-resolution clock.
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_period = 1.0 / (f64) frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return TRUE;
}


/**
 * @param plat_state A pointer to the platform_state struct containing the internal state to be shut down.
 */
void platform_shutdown(platform_state* plat_state) {
    // Cold-cast the void pointer to our known internal state type.
    internal_state *state = (internal_state*) plat_state->internal_state;

    if (state->hwnd)
    {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }
}


/**
 * @param plat_state A pointer to the platform_state struct. Not directly used here but required by the interface.
 */
b8 platform_pump_messages(platform_state* plat_state) {
    MSG message;
    // Process all messages in the queue for this window.
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message); // Sends the message to our win32_process_message function.
    }
    return TRUE;
}


/**
 * @param size The size in bytes to allocate. `u64` allows for large allocations on 64-bit systems.
 * @param aligned A `b8` flag indicating if the memory should be aligned (currently unused).
 */
void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}


/**
 * @param block A `void*` generic pointer to the memory to be freed.
 * @param aligned A `b8` flag indicating if the memory was aligned (currently unused).
 */
void platform_free(void* block, b8 aligned) {
    free(block);
}


/**
 * @param block A `void*` generic pointer to the memory block.
 * @param size The `u64` size in bytes of the block to be zeroed.
 */
void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}


/**
 * @param dest A `void*` pointer to the destination memory block.
 * @param source A `const void*` pointer to the source; `const` because we don't modify it.
 * @param size The `u64` number of bytes to copy.
 */
void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}


/**
 * @param dest A `void*` pointer to the destination memory block.
 * @param value The `i32` value to set each byte to (will be truncated to a char).
 * @param size The `u64` number of bytes to set.
 */
void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}



/**
 * @param message The `const char*` message string to write.
 * @param colour A `u8` index representing the desired color level.
 */
void platform_console_write(const char* message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // Color attributes for FATAL, ERROR, WARN, INFO, DEBUG, TRACE levels.
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);    // Also write to the VS debug output window.
    u64 length = strlen(message);
    LPDWORD number_written = NULL;
    WriteConsoleA(console_handle, message, (DWORD)length, number_written, NULL);
}


/**
 * @param message The `const char*` error message string to write.
 * @param colour A `u8` index representing the desired color level.
 */
void platform_console_write_error(const char* message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // Color attributes for FATAL, ERROR, WARN, INFO, DEBUG, TRACE levels.
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);    // Also write to the VS debug output window.
    u64 length = strlen(message);
    LPDWORD number_written = NULL;
    WriteConsoleA(console_handle, message, (DWORD)length, number_written, NULL);
}


/**
 * @return Returns the absolute time in seconds as a high-precision `f64`.
 */
f64 platform_get_absolute_time() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64) now_time.QuadPart * clock_period;
}


/**
 * @param ms The number of milliseconds to sleep. `u64` allows for very long durations if needed.
 */
void platform_sleep(u64 ms) {
    Sleep(ms);
}


/**
 * @param hwnd A `HWND` handle to the window that received the message.
 * @param msg The `u32` message identifier (e.g., WM_CLOSE).
 * @param w_param A `WPARAM` (Windows-specific type) containing extra message data.
 * @param l_param An `LPARAM` (Windows-specific type) containing extra message data.
 */
LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param)
{
    switch(msg) {
        case WM_ERASEBKGND:
            // Notify the OS that we will handle erasing the background to prevent flicker.
            return 1;
        case WM_CLOSE:
            // TODO: Fire an event for the application to quit gracefully.
            return 0;
        case WM_DESTROY:
            // This message is sent when the window is being destroyed.
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // A resize event occurred.

            // Get the updated size
            // RECT r;
            // GetClientRect(hwnd, &r);
            // u32 width = r.right - r.left;
            // u32 height = r.bottom - r.top;

            // TODO: Fire an event for the renderer to handle the new window size.
        }break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // Keyboard input event.

            // Key pressed/released
            // b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);

            // TODO: Process keyboard input and fire events.
        }break;
        case WM_MOUSEMOVE: {
            // Mouse movement event.

            //i32 x_position = GET_X_LPARAM(l_param);
            //i32 y_position = GET_Y_LPARAM(l_param);
            
            // TODO: Process mouse movement and fire events.
        }break;
        case WM_MOUSEWHEEL: {
            // Mouse wheel scroll event.
            
            // i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            // if (z_delta != 0) {
            //     // Flatten the input to an OS-independent (-1, 1)
            //     z_delta = (z_delta < 0) ? -1 : 1;
            //     // TODO: Process mouse wheel input and fire events.
            // } 
        }break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            // Mouse button event.

            // b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);

            // TODO: Process mouse button input and fire events.
        }break;
    }

    // Pass any unhandled messages to the default window procedure.
    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

#endif  // KPLATFORM_WINDOWS