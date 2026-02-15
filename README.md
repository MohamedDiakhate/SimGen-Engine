# SimGen++ Physics Engine 🪐

**A Deterministic, Node-Based Physics & Procedural Generation Engine in C++.**

SimGen++ is a custom-built simulation engine designed to generate synthetic data for computer vision tasks and physics experimentation. It features a graph-based execution pipeline, rigid body physics, and a modular architecture for procedural generation.



---

## 🧭 Reviewer's Guide / Architecture Overview

This project is structured around a **Dependency Graph** architecture rather than a monolithic loop. If you are reviewing the code, here is a roadmap of how the system is organized and the reasoning behind key design decisions.

### 1. The Core Philosophy: "Graph & Nodes"
The simulation logic is split into discrete stages: `Setup` -> `Randomize` -> `Physics` -> `Render` -> `Output`.
* **The Pipeline (`src/core/graph.cpp`):** Manages the lifecycle and execution of the simulation. It handles the deterministic seed cascade to ensure 100% reproducibility.
* **The Nodes (`src/nodes/`):** Each logical task (moving objects, calculating gravity, rendering pixels) is an isolated `Node` that can be plugged into any stage.

### 2. Key Modules
| Module | Responsibility |
| :--- | :--- |
| **`src/core/`** | **The Brain.** Contains the Graph execution logic and the Node base classes. |
| **`fabric/scene_database.h`** | **The Data.** Functions as a lightweight ECS (Entity Component System). Stores entities in a flat memory layout for cache efficiency. |
| **`nodes/physics/`** | **The Law.** A rigid body dynamics solver using Semi-Implicit Euler integration for stability. |
| **`nodes/render/`** | **The Eyes.** A custom multi-threaded Ray Tracer. |

---

## 🧠 Design Decisions & Trade-offs

### 🧵 Threading Model: Custom Pool vs. TBB
You will notice **Intel TBB (Threading Building Blocks)** is included in `CMakeLists.txt`, but I am currently using a **custom Thread Pool implementation**.
* **Current State:** Multi-threading is currently active **only** in the `RayTraceNode` (Rendering).
* **Reasoning:** The Physics and Randomization loops operate on < 100 entities for this demo. The overhead of launching threads for such a small workload outweighs the benefits.
* **Future:** As entity counts scale to 10,000+, I plan to switch the Physics solver to use TBB's `parallel_for`.

### 🆔 Entity Identification
* **Current State:** Entities use simple, incremental `int` IDs.
* **Reasoning:** For small-scale simulations, array indexing is O(1) and extremely fast.
* **Future:** I plan to migrate to **Spatial Hashing** or **UUIDs** to support streaming large worlds without ID collisions.

### 🎲 Determinism
The engine is **100% deterministic**.
A single `Master Seed` is passed into the Graph, which cascades down to every node. Running `./simgen 99` will produce the exact same pixel-perfect simulation on any machine, which is critical for debugging physics glitches and generating reproducible ML datasets.

---

## 📂 Project Structure

```text
SimGen++/
├── include/
│   ├── core/           # Graph, Node, and Type definitions
│   ├── fabric/         # Scene Database (ECS)
│   └── nodes/          # Physics, Randomizers, and Rendering logic
├── src/
│   ├── main.cpp        # Entry point and wiring
│   ├── core/           # Graph implementation
│   └── nodes/          # Node implementations
├── viewer.html         # WebGL Viewer for playback
└── CMakeLists.txt      # Build configuration
```
---

## 🛠️ Build & Run Instructions

### Prerequisites
Ensure you have the following installed before starting:
* **C++17 Compiler** (GCC, Clang, or MSVC)
* **CMake 3.10+**
* **Python 3** (Optional, required for the Web Viewer)

### 1. Build
Create a build directory and compile the executable.

```bash
# Create and enter the build directory
mkdir build && cd build

# Configure the project with CMake
cmake ..

# Compile the executable
make
```
### 2. Run
```bash
# Usage: ./simgen <SEED>
# Example: Run with seed 12345 to generate trajectory data
./simgen 12345
```