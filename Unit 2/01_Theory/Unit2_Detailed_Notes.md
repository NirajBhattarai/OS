# Unit 2 Detailed Notes: Process, Process Management, Threads, and Scheduling

## 1. Process Model

A process is a program in execution. It includes:
- Program code
- Program counter (current instruction)
- CPU register values
- Stack and heap
- Open files and I/O state

### Process States

Common five-state model:
- New: process created but not yet admitted to ready queue
- Ready: waiting for CPU allocation
- Running: currently executing on CPU
- Waiting (Blocked): waiting for I/O or event
- Terminated: finished execution

### Why this matters

Correct state transitions are core to scheduling, fairness, and system stability.

## 2. Process Control Block (PCB)

PCB is the OS data structure used to manage a process.

Typical PCB fields:
- Process ID (PID)
- Process state
- Program counter
- CPU registers
- Scheduling metadata (priority, queue pointers)
- Memory-management data (base/limit, page tables)
- Accounting and I/O status

### Key idea

Context switching relies on PCB snapshots. Without PCB, preemptive multitasking is not possible.

## 3. Context Switch

A context switch is when CPU switches execution from one process to another.

Steps:
1. Interrupt/trap occurs (timer, I/O, system call)
2. Save current CPU context to current process PCB
3. Select next ready process via scheduler
4. Restore next process context from PCB
5. Resume execution

### Trade-off

More frequent context switches improve responsiveness but add overhead.

## 4. Threads vs Processes

Process:
- Own address space
- Higher creation/switch overhead
- Strong isolation

Thread:
- Shares process memory/resources
- Lower overhead
- Faster communication
- Requires synchronization for shared data

### User-level vs Kernel-level threads

- User-level threads: managed in user space, very fast operations
- Kernel-level threads: managed by OS kernel, better for multicore scheduling and blocking calls

## 5. Race Conditions and Critical Sections

Race condition happens when output depends on unpredictable execution order.

Critical section is code that accesses shared mutable data and must not be concurrently executed by multiple threads.

### Mutual Exclusion techniques

- Mutex lock
- Semaphores
- Monitors
- Message passing

Correctness goals:
- Mutual exclusion
- Progress
- Bounded waiting

## 6. IPC (Inter-Process Communication)

IPC allows processes/threads to exchange data and coordinate.

Common mechanisms:
- Shared memory
- Pipes
- Message queues
- Sockets
- Condition variables + mutex for bounded buffer patterns

Producer-consumer is a classic IPC synchronization model.

## 7. CPU Scheduling

Scheduling objective is to optimize:
- CPU utilization
- Throughput
- Response time
- Turnaround time
- Waiting time
- Fairness

### Core Algorithms

1. FCFS
- First come, first served
- Simple but can cause convoy effect

2. SJF (non-preemptive)
- Chooses shortest burst first
- Good average waiting time

3. Priority
- Chooses highest priority first
- Risk of starvation (mitigated by aging)

4. Round Robin
- Time quantum based
- Good responsiveness for interactive workloads

## 8. Mapping Theory to Practicals

- State transitions -> `03_Practicals_CPP/src/01_process_states.cpp`
- PCB/context switch -> `03_Practicals_CPP/src/02_pcb_context_switch.cpp`
- Race + mutex -> `03_Practicals_CPP/src/03_threads_race_and_mutex.cpp`
- IPC producer-consumer -> `03_Practicals_CPP/src/04_ipc_bounded_buffer.cpp`
- Scheduling algorithms -> `03_Practicals_CPP/src/05_cpu_scheduling.cpp`

## 9. Programmatic Validation

From Unit 2:

```bash
cd "03_Practicals_CPP"
bash scripts/validate_all.sh
```

Validation criteria:
- Each topic executable returns success
- Each executable prints `Validation: PASS`
- The suite prints `Overall Result: PASS`
