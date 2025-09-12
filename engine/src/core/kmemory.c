/**
 * @file kmemory.c
 * @brief This file contains the implementation of the engine's memory subsystem.
 *
 * @details This implementation handles the actual memory operations by calling the
 * platform layer and maintains statistics about memory usage.
 */


#include "kmemory.h"

#include "core/logger.h"
#include "platform/platform.h"

// TODO: Replace with a custom string library
#include <string.h>
#include <stdio.h>


/**
 * @struct memory_stats
 * @brief A private structure to hold all memory allocation statistics.
 */
struct memory_stats {
    /** @brief The total number of bytes allocated across the entire application. */
    u64 total_allocated;

    /** @brief An array holding the number of bytes allocated per tag. */
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

// A static array of strings providing human-readable names for each memory tag.
static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      "};


// Holds the global state for the memory subsystem. Static to keep it private to this file.
static struct memory_stats stats;

void initialize_memory() {
    // Zero out the entire stats structure to ensure a clean state at startup.
    platform_zero_memory(&stats, sizeof(stats));
}

void shutdown_memory() {
    // This is a placeholder for any future cleanup logic.
}

void* kallocate(u64 size, memory_tag tag) {
    // Warn if the 'UNKNOWN' tag is used, as all allocations should be properly classified.
    if (tag == MEMORY_TAG_UNKOWN) {
        KWARN("kallocate called using MEMORY_TAG_UNKOWN. Re-class this allocation.");
    }

    // Increment the total and tagged allocation counters.
    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    // TODO: Implement memory alignment.
    // Call the platform layer to perform the actual allocation.
    void* block = platform_allocate(size, FALSE);

    // Zero the memory before returning it to prevent using uninitialized data.
    platform_zero_memory(block, size);

    return block;
}

void kfree(void* block, u64 size, memory_tag tag) {

    // Warn if the 'UNKNOWN' tag is used.
    if (tag == MEMORY_TAG_UNKOWN) {
        KWARN("kfree called using MEMORY_TAG_UNKOWN. Re-class this allocation.");
    }

    // Decrement the total and tagged allocation counters.
    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    // TODO: Implement memory alignment.
    // Call the platform layer to free the memory.
    platform_free(block, FALSE);
}

void* kzero_memory(void* block, u64 size) {
    // A simple pass-through to the platform-specific implementation.
    return platform_zero_memory(block, size);    
}

void* kcopy_memory(void* dest, const void* src, u64 size) {
    // A simple pass-through to the platform-specific implementation.
    return platform_copy_memory(dest, src, size);
}


void* kset_memory(void* dest, i32 value, u64 size) {
    // A simple pass-through to the platform-specific implementation.
    return platform_set_memory(dest, value, size);
}

char* get_memory_usage_str() {
    // Define constants for converting bytes to human-readable units.
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    enum { msg_length = 8000 };
    char buffer[msg_length] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);

    // Iterate through each memory tag and format its usage information.
    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB"; // A template for the memory unit (e.g., KiB, MiB).
        float amount = 1.0f;
        
        // Determine the most appropriate unit for the current size.
        if (stats.tagged_allocations[i] >= gib) {
            unit[0] = 'G';
            amount = stats.tagged_allocations[i] / (float) gib;
        } else if (stats.tagged_allocations[i] >= mib) {
            unit[0] = 'M';
            amount = stats.tagged_allocations[i] / (float) mib;
        } else if (stats.tagged_allocations[i] >= kib) {
            unit[0] = 'K';
            amount = stats.tagged_allocations[i] / (float) kib;
        } else {
            unit[0] = 'B';
            unit[1] =  0; // Null-terminate to just show 'B' for bytes.
            amount = (float) stats.tagged_allocations[i];
        }

        // Write the formatted string for the current tag into the buffer.
        i32 length = snprintf(buffer + offset, msg_length - offset, "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);

        // Clamp the length to prevent buffer overflow issues.
        length = CLAMP(length, 0, msg_length - offset - 1);
        offset += length;
    }

    // Duplicate the stack-allocated buffer to a new heap-allocated string.
    // The caller is responsible for freeing this memory.
    char* out_string = _strdup(buffer);
    return out_string;
}