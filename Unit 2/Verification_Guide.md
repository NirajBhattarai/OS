# Unit 2 Verification Guide

This guide helps students verify the concepts shown in the Unit 2 presentation.

## Files to Use

- Presentation: Process_and_Thread_Presentation.md
- Program: thread_race_demo.py

## What to Verify

1. Race condition concept (Presentation section 5)
- Run the program without lock section.
- Counter is usually less than expected because updates are lost.

2. Mutual exclusion concept (Presentation section 5)
- Run the lock section.
- Counter should match expected value exactly.

3. Thread behavior (Presentation section 3)
- Same process shares one global counter across threads.
- Different thread execution order changes the unsafe result.

## How to Run

From the workspace root:

```bash
cd "Unit 2"
python3 thread_race_demo.py
```

## Expected Output Pattern

- "Without lock" -> observed counter < expected (most runs)
- "With lock" -> observed counter == expected

If the unsafe case does not fail once, run again. Thread timing is nondeterministic.
