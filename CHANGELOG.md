# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased] - 2025-12-19

### Added

- **Modernization Refactoring**: Initial pass for Windows build compatibility using CMake.
- **Project Structure**: Added `CHANGELOG.md` to track ongoing refactoring efforts.

### Changed

- **`client/app_start.cpp`**: Refactored legacy string handling to use `std::string` methods (`.empty()`, `.c_str()`), replacing unsafe `strlen` and `strcpy` usage.
- **`client/client_state.cpp`**: Updated iterator logic for `std::vector<std::unique_ptr<T>>` to ensure memory safety and correct pointer access (`.get()`).
- **`client/check_security.cpp`**: Added `_WIN32` preprocessor guards to exclude POSIX-specific directory checking logic, resolving multiple undeclared identifier errors on Windows.
- **`client/cs_scheduler.cpp`**: Comprehensive refactor fixing ~88 compilation errors:
  - Standardized `std::string` to `char*` conversions.
  - Corrected `std::unique_ptr` usage in project/result iteration loops.
  - Added necessary type casts for legacy function signatures.
- **Build System**: Updated `client/CMakeLists.txt` to support `vcpkg` dependency management.

### Fixed

- Resolved over 200 compilation errors related to type mismatches between modern C++ types (`std::string`, `std::unique_ptr`) and legacy C interfaces.
