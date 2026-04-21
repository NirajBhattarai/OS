# Unit 2 Verification Guide

This guide helps students verify the concepts shown in the Unit 2 presentation.

## Files to Use

- Presentation: `02_Presentation/Process_and_Thread_Presentation.md`
- Primary practical pack: `03_Practicals_CPP/`
- Optional demo: `04_Practicals_Python/thread_race_demo.py`

## Recommended Validation Path (C++)

From Unit 2 folder:

```bash
cd "03_Practicals_CPP"
bash scripts/validate_all.sh
```

This validates all Unit 2 practical topics programmatically.

## What to Verify

1. Process model and states
- `01_process_states.cpp` should pass all legal transition checks.

2. PCB and context switch
- `02_pcb_context_switch.cpp` should show saved state and restored state correctly.

3. Race condition and mutual exclusion
- `03_threads_race_and_mutex.cpp` should show unsafe mismatch and safe exact count.

4. IPC correctness
- `04_ipc_bounded_buffer.cpp` should preserve item count and sum.

5. Scheduling behavior
- `05_cpu_scheduling.cpp` should report expected order patterns and pass metric checks.

## Expected Output Pattern

- Every program prints `Validation: PASS`
- Final script prints `Overall Result: PASS`

If race-condition failure does not appear in one unsafe run, rerun the thread demo.
