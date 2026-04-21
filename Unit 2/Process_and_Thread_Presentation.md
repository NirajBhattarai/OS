# Unit 2 (OS): Process, Process Management, Threads, and Scheduling

Source baseline: ITEX 220 Operating System syllabus (Unit Two topic list from your PDF)

---

## 1. Why Unit 2 Matters

- The OS must run many programs safely and efficiently.
- Unit 2 explains how CPU time is shared using processes and threads.
- These concepts drive performance, responsiveness, and correctness.

```mermaid
%%{init: {'theme':'base','themeVariables':{
  'primaryColor':'#e8f5e9',
  'primaryTextColor':'#102a43',
  'primaryBorderColor':'#2f855a',
  'lineColor':'#1f2937',
  'secondaryColor':'#fff8e1',
  'tertiaryColor':'#e3f2fd'
}}}%%
flowchart LR
    U[User Program] --> SC[System Call]
    SC --> K[Kernel Mode]
    K --> PM[Process Manager]
    PM --> PCB[Process Control Block]
    PM --> SCH[CPU Scheduler]
    SCH --> CPU[CPU Core]
    CPU -->|Time Slice Ends| INT[Timer Interrupt]
    INT --> SCH
```

---

## 2. Process Model and PCB

- A process is a program in execution.
- Core states: New, Ready, Running, Waiting, Terminated.
- PCB stores process identity and execution context.
- Context switch = save current PCB state and restore next PCB state.

```mermaid
%%{init: {'theme':'base','themeVariables':{
  'primaryColor':'#fff3e0',
  'primaryTextColor':'#1f2937',
  'primaryBorderColor':'#d97706',
  'lineColor':'#374151'
}}}%%
stateDiagram-v2
    [*] --> New
    New --> Ready: admitted
    Ready --> Running: dispatch
    Running --> Ready: preempt
    Running --> Waiting: I/O wait
    Waiting --> Ready: I/O complete
    Running --> Terminated: exit
    Terminated --> [*]
```

---

## 3. Process vs Thread

- Process: heavy-weight unit with separate address space.
- Thread: light-weight execution path inside a process.
- Threads share process resources but keep their own stack and registers.
- User threads are fast to create; kernel threads are visible to OS scheduler.

```mermaid
%%{init: {'theme':'base','themeVariables':{
  'primaryColor':'#e0f2fe',
  'primaryTextColor':'#0f172a',
  'primaryBorderColor':'#0284c7',
  'lineColor':'#334155'
}}}%%
classDiagram
    class Process {
      +pid: int
      +addressSpace: MB
      +openFiles: list
      +codeDataHeapStack
      +createThread()
    }
    class Thread {
      +tid: int
      +pc: register
      +registerSet
      +stack
      +run()
    }
    Process "1" --> "many" Thread : contains
```

---

## 4. Context Switch (Execution Story)

- Triggered by timer interrupt, system call, or I/O event.
- OS saves old context, picks next ready process, restores new context.
- More context switches improve fairness but add overhead.

```mermaid
%%{init: {'theme':'base','themeVariables':{
  'primaryColor':'#f3e8ff',
  'primaryTextColor':'#111827',
  'primaryBorderColor':'#7c3aed',
  'lineColor':'#374151',
  'actorBorder':'#6d28d9',
  'actorTextColor':'#111827'
}}}%%
sequenceDiagram
    autonumber
    participant P1 as Process A
    participant K as Kernel
    participant S as Scheduler
    participant P2 as Process B
    P1->>K: timer interrupt / trap
    K->>K: save context of P1
    K->>S: select next ready process
    S-->>K: choose P2
    K->>K: load context of P2
    K-->>P2: return to user mode
```

---

## 5. IPC and Synchronization Essentials

- IPC mechanisms: shared memory, message passing, pipes, sockets.
- Race condition: result depends on non-deterministic execution order.
- Critical section: code region touching shared data.
- Mutual exclusion tools: busy waiting, sleep/wakeup, semaphores, monitors, message passing.

Quick rule:

$$
\text{Safe Concurrent Update} = \text{Atomicity} + \text{Mutual Exclusion} + \text{Progress}
$$

---

## 6. Scheduling Algorithms (Unit 2 Core)

- FCFS: simple, may cause convoy effect.
- SJF/SRTN: good average waiting time, needs burst estimation.
- Round Robin: best for interactive systems, depends on time quantum.
- Priority scheduling: urgent tasks first, can starve low priority tasks.
- Multi-level queue: separate classes of workloads with distinct policies.

```mermaid
%%{init: {'theme':'base','themeVariables':{
  'primaryColor':'#ecfeff',
  'primaryTextColor':'#0f172a',
  'primaryBorderColor':'#0891b2',
  'lineColor':'#0f172a'
}}}%%
flowchart TD
    A[Need minimum average waiting time] --> SJF[SJF or SRTN]
    B[Need fairness and responsiveness] --> RR[Round Robin]
    C[Need urgent tasks first] --> PRI[Priority Scheduling]
    D[Mixed workloads] --> MQ[Multi-level Queue]
    RR --> Q[Choose time quantum carefully]
    PRI --> ST[Add aging to prevent starvation]
```

---

## 7. Exam-Focused Summary (Unit 2)

- Be ready to draw process state transition diagram.
- Explain PCB fields and context switching steps.
- Differentiate process and thread with at least 3 points.
- Compare FCFS, SJF, SRTN, RR, and Priority with one use-case each.
- Explain race condition and one mutual exclusion technique clearly.

---

## 8. Fast Revision Card

- Process = resource container + execution context.
- Thread = lightweight execution unit inside a process.
- Scheduler objective = fairness + throughput + response time.
- Synchronization objective = correctness under concurrency.
- Unit 2 links directly to deadlock and memory units.

---

## 9. Student Verification Activity (Practical)

- Run `thread_race_demo.py` to observe race condition and mutual exclusion.
- Use `Verification_Guide.md` to map the output to Unit 2 theory.
- Verify section 5 claims directly by comparing without-lock vs with-lock results.

Run command:

```bash
cd "Unit 2"
python3 thread_race_demo.py
```

