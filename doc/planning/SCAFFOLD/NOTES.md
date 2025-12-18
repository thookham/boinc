# BOINC Scaffold & Notes

## Architectural Highlights

### 1. Scheduler (`sched/`)

- **Credit System**: `sched/credit.cpp` - Critical for calculating user credits (Cobblestones).
- **Validator**: `sched/validator.cpp` - Validates results, handles replication, and credit granting.
- **Feeder**: `sched/feeder.cpp` - Feeds work to the shared memory segment for the scheduler.

### 2. Core Client (`client/`)

- **Work Fetch**: `client/work_fetch.cpp` - Logic for requesting work from projects.
- **Scheduler**: `client/cs_scheduler.cpp` - CPU/GPU scheduling logic.
- **Generic Graphics**: `client/hostinfo_unix.cpp` / `client/coproc_detect.cpp` - GPU detection logic (CUDA/OpenCL).

### 3. VirtualBox Wrapper (`samples/vboxwrapper/`)

- **VBox Management**: `vboxwrapper.cpp` - Manages VM lifecycle, snapshots, and remote desktop (RDP) forwarding.
- **Logging**: `vbox.cpp` - Handling VBox logs and error codes.

### 4. BOINC Manager (`clientgui/`)

- **RPC Client**: `lib/gui_rpc_client_ops.cpp` - Communicates with the core client via XML RPC.
- **Views**: `clientgui/View*.cpp` - The various tabs in the Advanced View.

## Codebase Quirks

- **Checkin Notes**: History is stored in `checkin_notes_YYYY` files in the root.
- **TODOs**: `todo` and `TODO_OLD` files in root contain legacy planning items.
- **Build System**: Mixed `configure.ac` (Autotools) for Unix and `.sln` (Visual Studio) for Windows.

## Key "Golden Nuggets" (from 2012 Notes)

- **Remote Desktop**: Added in Jan 2012 to `vboxwrapper` (port forwarding + "Show VM Console" in Manager).
- **OpenCL/GPU**: Major refactoring in `coproc_detect.cpp` for accurate memory reporting and correlations with CUDA devices.
- **Storage Simulator**: `ssim/` seems to be a simulation tool for volunteer data archival.
