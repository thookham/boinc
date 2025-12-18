# BOINC Modernization Strategy

## Goal

Transform the legacy BOINC codebase into a modern, easy-to-maintain C++ project.

## 1. Unified Build System (CMake)

**Current State**:

- Unix: Autotools (`configure.ac`, `Makefile.am`)
- Windows: Visual Studio Solutions (`win_build/*.sln`)
- Tests: CMake (`tests/CMakeLists.txt`) - *Partial adoption!*

**Strategy**:

- **Action**: Create a root `CMakeLists.txt`.
- **Phases**:
    1. **Core Libs**: Port `lib/` and `api/` to CMake.
    2. **Client**: Port `client/` to CMake.
    3. **Apps**: Port `apps/` and legacy `sched/`.
- **Benefit**: proper cross-platform support (Windows, Linux, macOS) from a single definition.

## 2. Modern C++ Standard (C++17/20)

**Current State**:

- `configure.ac` enforces C++11 (checked via `std::shared_ptr`).
- Code uses raw pointers and manual memory management (seen in `client_types.h`).

**Strategy**:

- **Action**: Update build standard to **C++17**.
- **Refactoring Targets**:
  - **Filesystem**: Replace custom `filesys.h` logic with `std::filesystem`.
  - **Memory**: Replace raw pointers (`FILE_INFO*`) with `std::unique_ptr` / `std::shared_ptr`.
  - **Concurrency**: Use `std::mutex` and `std::lock_guard` if relying on platform-specific threads.

## 3. Dependency Management (vcpkg)

**Current State**:

- Windows: Uses `vcpkg` (evenced by `win_build/vcpkg_3rdparty_dependencies.vcxproj`).
- Unix: Relies on system package managers (`apt`, `yum`) and `pkg-config`.

**Strategy**:

- **Action**: Standardize on `vcpkg` manifest mode (`vcpkg.json`).
- **Dependencies**: `curl`, `openssl`, `zlib`, `sqlite3` (if used).

## 4. CI/CD (GitHub Actions)

**Current State**:

- Legacy `ci_tools` directory.
- Likely Travis CI (based on file list `TravisEmailAgent` in parent dir, though maybe unrelated).

**Strategy**:

- **Action**: Add `.github/workflows/build.yml`.
- **Matrix**: Ubuntu (gcc/clang), Windows (MSVC), macOS (clang).

## Roadmap for User

1. **Step 1**: Create `vcpkg.json` manifest.
2. **Step 2**: Create root `CMakeLists.txt`.
3. **Step 3**: Verify build on Windows (replacing `.sln` usage).
4. **Step 4**: Refactor one module (e.g., `lib/`) to Proof-of-Concept modern C++.
