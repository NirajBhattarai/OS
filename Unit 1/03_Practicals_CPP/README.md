# C++ Practicals and Validation — Unit 1

All five programs below correspond directly to a Unit 1 topic.
Each includes built-in assertions so you can verify correctness in one command.

---

## Programs

| File | Topic | Validates |
|------|-------|-----------|
| `01_os_layers.cpp` | OS Layered Architecture | Legal transitions allowed; illegal ones blocked |
| `02_system_calls.cpp` | System Call Dispatch | Each syscall number routes to correct handler |
| `03_os_types.cpp` | Types of OS | Each OS type enforces its specific policy |
| `04_os_structures.cpp` | Monolithic vs Microkernel | Speed vs fault isolation trade-off |
| `05_virtual_machine.cpp` | Virtual Machine Isolation | Memory isolation, snapshot, independent CPU state |

---

## Quick Start

```bash
# From Unit 1/03_Practicals_CPP
bash scripts/validate_all.sh
```

---

## Individual Build and Run

```bash
make                    # build all to bin/
make run                # build + run all sequentially
./bin/01_os_layers      # run one topic
make clean              # remove bin/
```

---

## Suggested Lab Experiments

1. **Break the layer rule**: in `01_os_layers.cpp`, add a direct User App → Hardware call to `legal_request` and watch validation fail.
2. **Add a syscall**: in `02_system_calls.cpp`, add a new entry to the dispatch table and a test case for it.
3. **Kill a Hard RTOS deadline**: in `03_os_types.cpp`, change a HardRTOS task's `actual_time_ms` to exceed `deadline_ms`. Observe rejection.
4. **Crash a monolithic service**: in `04_os_structures.cpp`, observe that the exception propagates vs the microkernel server crash which is contained.
5. **Test VM memory isolation**: in `05_virtual_machine.cpp`, write the same address in both VMs and confirm values remain independent.

---

## Exam-to-Code Reference

| Exam question | Code to run |
|---------------|-------------|
| Draw layered OS diagram | `01_os_layers` output |
| Trace a system call step by step | `02_system_calls` output |
| Compare OS types | `03_os_types` output |
| Monolithic vs microkernel trade-offs | `04_os_structures` output |
| Explain VM isolation | `05_virtual_machine` output |
