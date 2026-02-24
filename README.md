# SimGen++ Physics Engine 🪐

**A Deterministic, Node-Based Physics Engine (C++) orchestrated by a Command-Line Tooling Pipeline (C#) and containerized via Docker.**

SimGen++ is a custom-built simulation engine designed to generate synthetic data for computer vision tasks and physics experimentation. It features a graph-based execution pipeline, rigid body physics, and a modular architecture for procedural generation.

It features a multi-language architecture: a high-performance C++ backend for rigid body physics/ray-tracing, and a C# (.NET 8) frontend for JSON configuration and process orchestration.



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
### 🐳 Infrastructure: Docker & CI/CD
To prevent "it works on my machine" issues, the engine is packaged using Docker. Furthermore, a Continuous Integration (CI) pipeline is configured via **GitHub Actions**. Every push to the repository automatically provisions a cloud runner, compiles the C++ source using CMake, and builds the Docker container to verify build integrity.

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

This project is designed to be highly modular. You can either run the **Full Ecosystem** (using the C# orchestrator and Docker), or build the **Standalone C++ Engine** natively on your machine.

### Prerequisites
**For the Full Pipeline:**
* **Docker Desktop** (Daemon must be running)
* **.NET 8 SDK** (To run the C# Orchestrator)

**For the Standalone C++ Engine:**
* **C++17 Compiler** (GCC, Clang, or MSVC)
* **CMake 3.10+**

*(Optional) For visual output playback:*
* **Python 3** (To run the GIF generator script)

---
### Option A: The Full Pipeline (C# -> Docker -> C++)
This is the recommended way to run the project. The C# Orchestrator will handle JSON configuration and automatically launch the isolated Linux container.

```bash
# 1. Build the Docker Container
docker build -t simgen-engine .

# 2. Navigate to the C# Manager directory
cd SimGenManager

# 3. Run the pipeline (Follow the terminal prompts)
dotnet run
```
### Option B: Standalone C++ Build (Manual)
Create a build directory and compile the executable.

```bash
# 1. Create and enter the build directory
mkdir build && cd build

# Configure the project with CMake
cmake ..

# Compile the executable
make

# 2. Run

# Usage: ./simgen <SEED>
# Example: Run with seed 12345 to generate trajectory data
./simgen 12345


# 3. Visualize

# Run python script to out a GIF animation
# as th scripts.py is not in the "build" folder
python ../scripts/make_gif.py
```