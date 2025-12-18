# BOINC Modernization Roadmap

## Core Vision

Modernize the BOINC middleware fork into a clean, modern, and highly-maintainable C++17 codebase with a standard CMake-based build system.

## Phase 1: Infrastructure & Cleanup (Current)

- [x] **Repository Audit**: Initial assessment of legacy code and historical artifacts.
- [x] **Registry & Dependencies**: Setup `vcpkg.json` and basic CMake infrastructure.
- [/] **Documentation Consolidation**:
  - Move legacy `checkin_notes` to `doc/history/`.
  - Move `todo`, `TODO_OLD`, and `SCAFFOLD` to `doc/planning/`.
  - **Roadmap**: Centralize all future work into this document (`doc/planning/ROADMAP.md`).
- [ ] **Sanitization**: Ensure no PII or sensitive keys remain in historical logs.

## Phase 2: Core Refactoring (Complete)

- [x] **Filesystem Modernization**: Migrated `lib/filesys.cpp` to `std::filesystem`.
- [x] **Smart Pointer Migration**:
  - [x] `FILE_INFO` members (`pers_file_xfer`, `cert_sigs`, `async_verify`).
  - [x] `CLIENT_STATE` subsystems (`http_ops`, `file_xfers`, `pers_file_xfers`, `scheduler_op`).
  - [x] `CLIENT_STATE` collections (`projects`, `apps`, `file_infos`, etc.) to `std::vector<std::unique_ptr<T>>`.
  - [x] `PROJECT::trickle_up_ops` to `std::vector<std::unique_ptr<TRICKLE_UP_OP>>`.
- [x] **Range-Based For Loops**: Converted all C-style indexed loops to modern `auto const&` patterns in:
  - `cpu_sched.cpp`, `rr_sim.cpp`, `work_fetch.cpp`, `sim.cpp`
  - `project.cpp`, `app_config.cpp`, `log_flags.cpp`
  - `cs_prefs.cpp`, `cs_statefile.cpp`, `cs_sporadic.cpp`
  - `app_control.cpp`, `app_graphics.cpp`, `client_state.cpp`
- [ ] **Strict Typing**: Replace `char*` buffers with `std::string` (Future).

## Phase 3: Build System & CI

- [ ] **CMake Completion**: Fully port all sub-projects (client, manager, screensaver) to CMake.
- [ ] **Vcpkg Integration**: Transition all external dependencies (OpenSSL, Curl, Sqlite) to `vcpkg`.
- [ ] **GitHub Actions**: Implement CI for Windows, Linux, and macOS.

## Phase 4: Long-term Evolution (Future)

- **Containerization**: Update `docker_wrapper` for efficient containerized task execution.
- **Web API Modernization**: Transition the PHP-based scheduler RPCs to a more modern, type-safe API.
- **Cross-Platform UI**: Explore modern UI frameworks for the BOINC Manager.

---
> [!NOTE]
> This roadmap is a living document and will be updated as the modernization effort progresses.
