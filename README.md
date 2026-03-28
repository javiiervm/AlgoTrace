<div align="center">
  <img width="600" height="600" alt="AlgoTrace logo" src="https://github.com/javiiervm/AlgoTrace/blob/main/assets/logo.png" />
  <p>
    <img src="https://img.shields.io/github/last-commit/javiiervm/AlgoTrace/main" alt="Last Commit" />
    <img src="https://img.shields.io/badge/platform-linux%20%7C%20windows%20%7C%20macos-lightgrey" alt="Platform Support" />
    <img src="https://img.shields.io/github/issues/javiiervm/AlgoTrace" alt="Issues" />
    <img src="https://img.shields.io/github/stars/javiiervm/AlgoTrace" alt="Stars" />
    <br />
    <img src="https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B" alt="C++ Version" />
    <img src="https://img.shields.io/badge/CMake-3.14%2B-green?logo=cmake" alt="CMake Version" />
    <img src="https://img.shields.io/badge/Raylib-5.0-black" alt="Raylib Version" />
    <img src="https://img.shields.io/github/license/javiiervm/AlgoTrace" alt="License" />
  </p>
</div><br />

**AlgoTrace** is a high-performance, interactive sorting algorithm visualizer built with C++ and Raylib. It is designed not just as a visualization tool, but as a scalable, modular framework where developers can easily plug in and test new sorting algorithms without touching the core graphics engine.

## ✨ Features

* **Interactive Visualization:** Watch algorithms sort data step-by-step in real-time.
* **Line-by-Line Execution:** A dynamic pseudo-code panel highlights the exact line of code currently being executed by the state machine.
* **Fully Responsive UI:** The interface dynamically recalculates geometry, ensuring perfect layouts across different window sizes.
* **Decoupled Architecture:** The graphical engine (`Visualizer`) is completely separated from the mathematical logic (`Algorithm`).
* **Auto-Discovery Build System:** Powered by CMake, new algorithms are automatically detected and compiled without modifying build scripts.

## 🧰 Tech Stack

* **Language:** C++17
* **Graphics Library:** [Raylib](https://www.raylib.com/) & [raylib-cpp](https://github.com/RobLoach/raylib-cpp)
* **Build System:** CMake & Make

---

## 🚀 Getting Started

### Prerequisites
You don't need to install Raylib manually; CMake will fetch it automatically. You only need:
* A modern C++17 compiler (GCC, Clang, or MSVC)
* CMake (3.14 or higher)
* Make

### Installation & Execution
1. Clone the repository:
   ```bash
   git clone [https://github.com/javiiervm/AlgoTrace.git](https://github.com/javiiervm/AlgoTrace.git)
   cd AlgoTrace
   ```

2. Use the provided Makefile to configure, build, and run the project in one command:
   ```bash
   make run
   ```

*(Note: The first time you run this, it will take a moment to download and compile Raylib. Subsequent builds will be near-instant).*

### Keyboard Controls

* **SPACE:** Pause / Play the visualization.
* **R:** Reset the array and restart the current algorithm.
* **BACKSPACE:** Return to the Main Menu.

---

## 📁 Project Structure

```text
AlgoTrace/
├── assets/
│   └── font.ttf                 # UI Font (JetBrains Mono)
├── include/
│   ├── Algorithm.hpp            # The strict Base Class template
│   ├── AlgorithmRegistry.hpp    # Registry factory declaration
│   ├── Visualizer.hpp           # Graphics and UI engine
│   └── ... [Algorithm Headers]
├── lib/
│   ├── AlgorithmRegistry.cpp    # The ONLY file contributors need to edit to register algorithms
│   ├── Visualizer.cpp           # Core rendering logic
│   └── ... [Algorithm Implementations]
├── src/
│   └── main.cpp                 # Entry point
├── CMakeLists.txt               # Build configuration (Auto-discovers .cpp files)
└── Makefile                     # CLI shortcuts
```

---

## 👨‍💻 How to Contribute (Adding a New Algorithm)

AlgoTrace is built with a **Factory Pattern** and an **Interface-driven design**. You do not need to understand Raylib or the UI code to add a new algorithm. Just follow these 4 simple steps:

### Step 1: Create your files

Create a new header (`.hpp`) in the `include/` folder and a source file (`.cpp`) in the `lib/` folder.

* *Tip: You can copy the structure from `include/TemplateSort.hpp` and `lib/TemplateSort.cpp` if available.*

### Step 2: Inherit from `Algorithm`

Your class must inherit from the `Algorithm` base class and implement its pure virtual methods.

```cpp
// Example: include/MySort.hpp
#pragma once
#include "Algorithm.hpp"

class MySort : public Algorithm {
private:
    std::vector<std::string> pseudoCode;
    int currentCodeLine = 0;
    int stepState = 0;
    size_t currentIndex = 0;

public:
    MySort(std::vector<int>& d);
    void step() override; // The core state machine logic
    std::string getName() const override { return "My Sort"; }
    size_t getCurrentIndex() const override { return currentIndex; }
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    int getCurrentCodeLine() const override { return currentCodeLine; }
};
```

### Step 3: Implement the State Machine

In your `.cpp` file, implement the `step()` function.
**Important:** Do not use a `while` loop to sort the entire array at once. The `step()` function is called once per frame. Use a `stepState` variable to execute exactly **one line of your pseudo-code per frame** so the UI can animate it.

### Step 4: Register your Algorithm

Open `lib/AlgorithmRegistry.cpp`. This is the central hub for all algorithms.

1. Include your header file at the top.
2. Add your algorithm to the `registry` vector using the lambda factory format:

```cpp
#include "AlgorithmRegistry.hpp"
#include "MySort.hpp" // 1. Include your header

std::vector<AlgoRegistryEntry> getAvailableAlgorithms() {
    std::vector<AlgoRegistryEntry> registry;

    registry.push_back({"Stalin Sort", [](std::vector<int>& d) { return std::make_unique<StalinSort>(d); }});
    // 2. Add your algorithm here:
    registry.push_back({"My Sort", [](std::vector<int>& d) { return std::make_unique<MySort>(d); }});

    return registry;
}
```

### Step 5: Run!

Because the `CMakeLists.txt` uses `GLOB_RECURSE CONFIGURE_DEPENDS`, it will automatically detect your new `.cpp` file. Just run:

```bash
make run
```

Your algorithm will instantly appear as a fully functional, clickable button in the main menu!
