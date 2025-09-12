#pragma once

/**
 * @file platform.h
 * @brief This file defines the platform abstraction layer
 *
 * It provides a platform-agnostic interface for windowing, memory management,
 * console I/O, and other OS-specific functionalities. The engine should only
 * interact with this interface, never directly with platform-specific code.
 */

#include "defines.h"

/**
 * @struct platform_state
 * @brief Holds the internal, opaque state for the platform layer.
 *
 * The engine does not need to know the details of the internal state,
 * which is managed entirely by the platform-specific implementation.
 */
typedef struct platform_state {
    /** @brief A void pointer to the platform-specific internal state. */
    void* internal_state;
} platform_state;


/**
 * @brief Initializes the platform layer.
 *
 * This function sets up the platform, including creating a window on desktop systems.
 * It must be called before any other platform functions.
 *
 * @param plat_state A pointer to the platform_state structure to be initialized.
 * @param application_name The name of the application, used for the window title.
 * @param x The initial x position of the window.
 * @param y The initial y position of the window.
 * @param width The initial width of the window's client area.
 * @param height The initial height of the window's client area.
 * @return b8 Returns TRUE on success, FALSE on failure.
 */
b8 platform_startup (
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height);

/**
 * @brief Shuts down the platform layer and releases all resources.
 * @param plat_state A pointer to the platform_state structure.
 */
void platform_shutdown(platform_state* plat_state);

/**
 * @brief Processes OS-specific messages and events.
 *
 * This must be called continuously in the main application loop to keep
 * the window responsive and handle input.
 *
 * @param plat_state A pointer to the platform_state structure.
 * @return b8 Returns TRUE if the application should continue running, FALSE otherwise.
 */
b8 platform_pump_messages(platform_state* plat_state);

/*
==================================
      MEMORY MANAGEMENT
==================================
*/

/**
 * @brief Allocates a block of memory.
 * @param size The size of the block to allocate.
 * @param aligned Indicates if the allocation should be aligned.
 * @return A pointer to the allocated memory block.
 */
void* platform_allocate(u64 size, b8 aligned);

/**
 * @brief Frees a previously allocated block of memory.
 * @param block A pointer to the memory block to free.
 * @param aligned Indicates if the allocation was aligned.
 */
void platform_free(void* block, b8 aligned);

/**
 * @brief Zeros out a block of memory.
 * @param block A pointer to the memory block.
 * @param size The size of the block to zero.
 * @return A pointer to the memory block.
 */
void* platform_zero_memory(void* block, u64 size);


/**
 * @brief Copies a block of memory from a source to a destination.
 * @param dest A pointer to the destination memory block.
 * @param source A pointer to the source memory block.
 * @param size The number of bytes to copy.
 * @return A pointer to the destination memory block.
 */
void* platform_copy_memory(void* dest, const void* source, u64 size);


/**
 * @brief Sets a block of memory to a specific value.
 * @param dest A pointer to the destination memory block.
 * @param value The value to set each byte to.
 * @param size The number of bytes to set.
 * @return A pointer to the destination memory block.
 */
void* platform_set_memory(void* dest, i32 value, u64 size);


/*
==================================
      CONSOLE I/O
==================================
*/


/**
 * @brief Writes a message to the console with a specific color.
 * @param message The message to write.
 * @param colour The color level to use for the output.
 */
void platform_console_write(const char* message, u8 colour);


/**
 * @brief Writes an error message to the console with a specific color.
 * @param message The error message to write.
 * @param colour The color level to use for the output.
 */
void platform_console_write_error(const char* message, u8 colour);


/*
==================================
      TIMING
==================================
*/


/**
 * @brief Gets the absolute time since the application started.
 * @return The absolute time in seconds as a 64-bit float.
 */
f64 platform_get_absolute_time();


/**
 * @brief Sleeps on the current thread for the provided milliseconds.
 *
 * This blocks the main thread. It should only be used for giving time
 * back to the OS when the application loop has spare time, to avoid
 * maxing out the CPU.
 *
 * @param ms The number of milliseconds to sleep.
 */
void platform_sleep(u64 ms);