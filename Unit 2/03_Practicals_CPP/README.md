# C++ Practicals and Validation

This folder contains the C++ practical implementation of all major Unit 2 topics.

## Structure

- `src/` -> C++ source files
- `bin/` -> compiled executables
- `scripts/validate_all.sh` -> one-command validation script
- `Makefile` -> build and run targets

## Programs

1. `01_process_states.cpp`
- Purpose: validates legal process state transitions
- Pass condition: all transitions in scenario are valid

2. `02_pcb_context_switch.cpp`
- Purpose: simulates context save/restore using PCB
- Pass condition: restored CPU and saved PCB values match expected

3. `03_threads_race_and_mutex.cpp`
- Purpose: demonstrates race condition and mutex fix
- Pass condition: unsafe run shows mismatch and safe run is exact

4. `04_ipc_bounded_buffer.cpp`
- Purpose: producer-consumer synchronization using condition variables
- Pass condition: produced/consumed counts and sums are consistent

5. `05_cpu_scheduling.cpp`
- Purpose: compares FCFS, SJF, Priority, Round Robin
- Pass condition: expected algorithm-order and metric assertions hold

## Commands

Build all:

```bash
make
```

Run all once:

```bash
make run
```

Run automated validation:

```bash
bash scripts/validate_all.sh
```

Clean build outputs:

```bash
make clean
```

## Instructor Usage

- Use source files for live demo during class.
- Ask students to modify one policy and rerun validation.
- Discuss why failures happen and map to OS theory.
