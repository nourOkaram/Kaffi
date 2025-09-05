#pragma once
#include "defines.h"

/**
 * @file test.h
 * @brief Declares a simple test function to verify library exporting.
 *
 * This header is part of the initial project setup and ensures that
 * the engine can be built as a shared library. It also verifies that
 * its functions can be correctly called by a host application (the testbed).
 */

/**
 * @brief Simple test function exported from the engine library.
 *
 * This function verifies that the KAPI export/import mechanism works correctly.
 * The testbed application calls this function to ensure it can successfully
 * link to and invoke functions within the engine DLL/shared object. 
 * The function prints the provided integer to the console.
 *
 * @param i A signed 32-bit integer to print to the console.
 */

KAPI void print_int(i32 i);