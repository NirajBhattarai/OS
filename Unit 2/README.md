# Unit 2 Operating Systems (Restructured)

This Unit 2 folder is organized as a complete study + practice + validation pack.

## Structure

- `01_Theory/`
	- Detailed concept notes and validation checklist
- `02_Presentation/`
	- Mermaid-based presentation deck for class delivery
- `03_Practicals_CPP/`
	- C++ topic-wise programs, build system, and automated validation
- `04_Practicals_Python/`
	- Optional Python demo (legacy supplementary practical)

## Start Here

1. Read theory: `01_Theory/Unit2_Detailed_Notes.md`
2. Review slides: `02_Presentation/Process_and_Thread_Presentation.md`
3. Run C++ practicals: `03_Practicals_CPP/README.md`
4. Validate all topics in one command:

```bash
cd "03_Practicals_CPP"
bash scripts/validate_all.sh
```

## Unit 2 Topics Covered

- Process model and process state transitions
- Process Control Block (PCB)
- Context switch mechanism
- Threads vs processes
- Race condition and mutual exclusion
- IPC via producer-consumer bounded buffer
- Scheduling: FCFS, SJF, Priority, Round Robin

## Learning Outcome Focus

- Explain concepts clearly (theory)
- Observe behavior from code (practical)
- Verify correctness using automated checks (validation)

## Recommended Lab Order

1. `01_process_states.cpp`
2. `02_pcb_context_switch.cpp`
3. `03_threads_race_and_mutex.cpp`
4. `04_ipc_bounded_buffer.cpp`
5. `05_cpu_scheduling.cpp`
