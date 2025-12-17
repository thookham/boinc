# Roadmap for boinc

## Project: BOINC Platform Source

The Berkeley Open Infrastructure for Network Computing (BOINC) is the middleware that powers SETI@home, Einstein@Home, and others.

## Status

- **Analysis**: Initial deep dive complete.
- **Sanitization**: Checked and clean.

## Golden Nuggets

These are the most architecturally significant files to study:

### 1. The Credit System (`sched/credit.cpp`)

* **Why**: Defines how "Cobblestones" (credits) are calculated and granted.
- **Key Functions**:
  - `grant_credit()`: The main entry point for updating user/team/host stats.
  - `get_pfc()`: Computes "Peak FLOP Count" for work units.
  - `update_av_scales()`: Normalizes credit across CPU vs GPU app versions (crucial for fairness).

### 2. Client Scheduler (`client/cs_scheduler.cpp`)

* **Why**: The brain of the client. Decides which project runs on which resource (CPU/GPU).
- **Key Logic**:
  - Handles "debt" (long-term balance of resources between projects).
  - Manages deadline miss risks (`RR_SIM` - Round Robin Simulation).

### 3. Work Fetching (`client/work_fetch.cpp`)

* **Why**: Determines when and how to ask the server for more work.
- **Key Logic**:
  - Hysteresis (don't ask too often, don't let queue run dry).

## Scaffold

- See `SCAFFOLD/NOTES.md` for personal scratchpad.
