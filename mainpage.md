# Kaffi Engine

Welcome to **Kaffi Engine**, a high-performance game engine written entirely in **C**. Kaffi is meticulously designed to provide a deep dive into the core technologies of modern game development. It features full 3D capabilities from inception, is entirely open-source, and built with clean abstractions for seamless multi-platform compatibility.

---

## Engine Overview

Kaffi is structured to give developers a solid foundation for building games efficiently and effectively. It includes:

- **Core Language:** C
- **Primary Compiler:** Clang (Visual Studio's compiler is not supported)
- **Graphics API:** Vulkan (OpenGL and DirectX support planned)
- **Platform Support:** Windows (primary), Linux, macOS (future)

The engine ecosystem consists of several distinct components:

1. **Engine Library (DLL/SO):** Core engine logic as a dynamic library.
2. **Game Application Executable:** Your actual game referencing the engine library.
3. **Test Bed Executable:** Dedicated for testing engine features in isolation.
4. **Editor (Separate):** Optional, modular editor not bundled with core engine.

---

## Requirements

To use and contribute to Kaffi, the following are recommended:

- **C Language Proficiency:** Understanding of C programming is essential.
- **Clang Compiler:** Ensures cross-platform compatibility.
- **Game Dev Concepts:** Basic knowledge of graphics APIs (OpenGL/Vulkan) and game loops is beneficial.
- **Introduction to C:** For beginners, a primer series will be available.

---

## Core Features

Kaffi includes a robust initial feature set:

1. **Build System:** Lightweight and cross-platform.
2. **Low-Level Utilities:** Dynamic arrays, binary trees, and string handling.
3. **Platform Abstraction:** Seamless Windows/Linux support.
4. **Comprehensive Logger:** Console and file logging.
5. **Flexible File I/O:** Efficient handling for textures, models, and maps.
6. **Application Layer:** Core game loop and engine subsystems.
7. **Advanced Renderer:** Vulkan support; OpenGL/DirectX planned.
8. **Memory Management:** Custom allocators for performance.
9. **Scene Graph & ECS:** Hierarchical scene management with ECS.
10. **Profiling & Debugging:** Built-in utilities.
11. **Hot-Reloadable Code:** Rapid iteration for scripts/game logic.
12. **Integrated Physics:** Basic physics system for realistic interactions.

---

## Engine Architecture

Kaffi is organized into modular, interdependent layers:

- **Platform Layer:** OS abstraction for windowing, input, file access, and console output.
- **Low-level Utilities:** Memory management, file I/O, and essential data structures.
- **Renderer Layer:** Unified interface for graphics APIs, starting with Vulkan.
- **Application Layer:** Central game loop, physics simulation, rendering orchestration.
- **Game Layer:** Top layer for game-specific logic, hot-reload support, and test bed integration.

---

## Getting Started

To compile the project, run the master build script located in the root directory:
-   On **Windows**, run: `build-all.bat`
-   On **Linux**, run: `build-all.sh`

This will build the engine library first, followed by the testbed executable, placing all binaries in the `bin/` directory.

---

Kaffi aims to be a **developer-friendly, cross-platform, and open-source** game engine, empowering creators to focus on building immersive experiences with minimal friction.
