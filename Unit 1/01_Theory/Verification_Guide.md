# Unit 1 Verification Guide

Maps each C++ program output to the specific Unit 1 theory concept it demonstrates.

---

## How to Run Everything

```bash
cd "/Users/dev/Desktop/Operating System/Unit 1/03_Practicals_CPP"
bash scripts/validate_all.sh
```

Expected final line: `Overall Result: PASS`

---

## Program-by-Program Verification

### 1. `01_os_layers` — OS Layers and Views

**What it does**
Simulates a four-layer OS stack:
Hardware → Kernel → System Programs → User Applications.
Each layer calls only the layer directly below it.

**What to observe**
- A user app request travels down through each layer.
- The output shows which layer handled each stage.
- Illegal cross-layer calls are rejected.

**Theory connection**
- Section 2: Three views of OS.
- Section 6.2: Layered architecture.

**Validation criterion**
All legitimate layer transitions succeed; skipped-layer calls are blocked.

---

### 2. `02_system_calls` — System Call Dispatch Simulation

**What it does**
Builds a mock system call dispatch table mapping syscall numbers to handler functions.
Simulates user program making syscall → trap → kernel dispatch → return.

**What to observe**
- Each syscall number routes to the correct handler by name.
- An unknown syscall number returns a clear error.
- Return values come back to the caller as they would in a real OS.

**Theory connection**
- Section 5: System calls and call handling.
- The 9-step dispatch flow in the notes.

**Validation criterion**
All registered syscalls dispatch correctly; invalid number is caught.

---

### 3. `03_os_types` — OS Type Property Comparison

**What it does**
Models OS type descriptors (RTOS, multi-user, embedded, etc.) with property flags.
Runs simulated tasks through each OS type and checks timing/isolation behavior.

**What to observe**
- Hard RTOS enforces strict deadline; tasks missing deadline are flagged.
- Single-tasking OS rejects concurrent task requests.
- Multi-user OS enforces user isolation.

**Theory connection**
- Section 4: Types of operating systems.

**Validation criterion**
Each OS type enforces exactly the properties claimed by its definition.

---

### 4. `04_os_structures` — Monolithic vs Microkernel

**What it does**
Simulates two dispatch models:
- Monolithic: direct function call, zero IPC overhead.
- Microkernel: message-passing between kernel and user-space servers.
Measures call overhead and fault-isolation behavior (crashing one server).

**What to observe**
- Monolithic calls complete instantly with no message overhead.
- Microkernel calls have message-passing steps visible in output.
- When a microkernel server crashes, the kernel stays running.
- In the monolithic model a bad service call affects the whole kernel.

**Theory connection**
- Section 6: OS structures comparison table.

**Validation criterion**
Monolithic demonstrates speed; microkernel demonstrates fault isolation.

---

### 5. `05_virtual_machine` — VM Isolation Simulation

**What it does**
Creates multiple VM objects, each with its own simulated memory space and CPU state.
Verifies that writes in one VM cannot be read from another VM.

**What to observe**
- Each VM has a completely separate address space (simulated).
- Memory written in VM1 does not appear in VM2.
- VM snapshots can be saved and restored.

**Theory connection**
- Section 7: Virtual machines, hypervisors, and isolation.

**Validation criterion**
No memory leak between VMs; snapshot restore returns exact CPU state.

---

## Suggested Experiments

1. Edit `04_os_structures.cpp` and remove the microkernel fault check. Rebuild and observe failure.
2. Edit `03_os_types.cpp` and allow a Hard RTOS task to miss deadline. Watch validation fail.
3. Edit `05_virtual_machine.cpp` and make VM1 write into VM2's memory. Observe detection.
