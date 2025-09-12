#pragma once

/**
 * @file kmemory.h
 * @brief This file contains the declarations for the engine's memory subsystem.
 *
 * @details The memory subsystem provides a unified interface for memory operations.
 * It allows for tracking memory allocations via tags, which is crucial for
 * debugging and performance monitoring. All engine and game-level code
 * should use this interface instead of standard library functions like malloc/free.
 */

#include "defines.h"


/**
 * @enum memory_tag
 * @brief Enumeration of memory allocation tags for tracking purposes.
 *
 * @details These tags are used to categorize every memory allocation within the
 * engine. This helps in tracking how much memory is used by different systems
 * (e.g., rendering, physics, game logic).
 */
typedef enum memory_tag {
    // For temporary use. Should be assigned a proper tag.
    MEMORY_TAG_UNKOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTTY_NODE,
    MEMORY_TAG_SCENE,

    // The maximum number of tags. This should always be the last entry.
    MEMORY_TAG_MAX_TAGS
} memory_tag;

/**
 * @brief Initializes the memory subsystem.
 * @details This must be called before any other memory function. It sets up the internal
 * state for tracking allocations.
 */
void initialize_memory();


/**
 * @brief Shuts down the memory subsystem.
 * @details Performs cleanup and logs final memory usage statistics.
 */
void shutdown_memory();


/**
 * @brief Allocates a block of memory.
 * @param size The size of the block to allocate in bytes. Type `u64` is used to support large allocations on 64-bit systems.
 * @param tag The `memory_tag` to classify this allocation for tracking.
 * @return A `void*` pointer to the allocated memory block. This generic pointer can be cast to any data type.
 */
KAPI void* kallocate(u64 size, memory_tag tag);


/**
 * @brief Frees a previously allocated block of memory.
 * @param block A pointer to the memory block to free. `void*` is used to accept any pointer type.
 * @param size The size of the block being freed. `u64` is used to match the size provided during allocation for accurate tracking.
 * @param tag The `memory_tag` that was used when the block was allocated.
 */
KAPI void kfree(void* block, u64 size, memory_tag tag);


/**
 * @brief Zeros out a block of memory.
 * @param block A pointer to the memory block. `void*` makes the function generic.
 * @param size The number of bytes to set to zero. `u64` supports large memory blocks.
 * @return A pointer to the memory block (`block`).
 */
KAPI void* kzero_memory(void* block, u64 size);


/**
 * @brief Copies memory from a source to a destination.
 * @param dest A pointer to the destination memory block.
 * @param src A pointer to the source memory block. `const void*` ensures the source data is not modified.
 * @param size The number of bytes to copy. `u64` supports large memory blocks.
 * @return A pointer to the destination memory block (`dest`).
 */
KAPI void* kcopy_memory(void* dest, const void* src, u64 size);


/**
 * @brief Sets a block of memory to a specific value.
 * @param dest A pointer to the destination memory block.
 * @param value The value to set each byte to. `i32` is a standard integer type, though only its lowest 8 bits are used for each byte.
 * @param size The number of bytes to set. `u64` supports large memory blocks.
 * @return A pointer to the destination memory block (`dest`).
 */
KAPI void* kset_memory(void* dest, i32 value, u64 size);


/**
 * @brief Generates and returns a string with detailed memory usage statistics.
 * @details This is a debug utility. The returned string is dynamically allocated and must be freed by the caller.
 * @return A `char*` pointer to a new string containing formatted memory usage info.
 */
KAPI char* get_memory_usage_str();