# BOINC (Modernized Fork)

> **Note**: This is an active fork of the [Berkeley Open Infrastructure for Network Computing (BOINC)](https://boinc.berkeley.edu) middleware, focused on modernizing the codebase and improving maintainability.

## 🚀 Project Mission

The goal of this fork is to bring the BOINC middleware into the modern C++ era. We are systematically refactoring the legacy codebase to leverage modern C++ standards (C++17/20), improving safety, performance, and developer experience.

### Key Initiatives

* **Modern C++ Adoption**: Transitioning from C-style patterns to modern C++ idioms.
* **Smart Pointers**: Replacing raw pointers with `std::unique_ptr` and `std::shared_ptr` for robust memory management.
* **Code Cleanup**: Removing obsolete components and improving code readability using range-based loops and auto type deduction.
* **Tech Debt Reduction**: Addressing legacy accumulated technical debt to facilitate future feature development.

## 🚧 Current Status (Dec 2025)

This fork is actively undergoing deep refactoring to enable CMake support on Windows and migrate the codebase to modern C++.
**Current Focus**: Resolving compilation errors in the optimized Client components (`client/*`), specifically targeting string handling and memory management (`std::unique_ptr`).

## 🛠️ Build & Installation

### Windows

The project maintains a Visual Studio solution file for Windows builds.

1. Open `win_build/boinc.sln` in Visual Studio 2019 or later.
2. Select the desired configuration (Debug/Release) and platform (x64/x86).
3. Build the solution.

### Unix / Linux

Standard Autotools build system is used for Unix-like environments.

```bash
# Generate configuration files
./_autosetup

# Configure the build
./configure

# Compile
make
```

## 📂 Documentation & Resources

* **[Roadmap](doc/planning/SCAFFOLD/NOTES.md)**: Detailed modernization plans and refactoring milestones.
* **[Architecture Notes](doc/planning/SCAFFOLD/NOTES.md)**: Technical deep-dives and architectural insights.
* **[History](doc/history)**: Implementation logs and archived notes.

## 🔗 Upstream & Community

* [Official BOINC Website](https://boinc.berkeley.edu)
* [Official Source Code](https://github.com/BOINC/boinc)

---
*Open-source software for volunteer computing and grid computing.*
