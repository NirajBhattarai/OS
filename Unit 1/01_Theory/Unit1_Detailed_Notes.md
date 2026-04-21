# Unit 1 Detailed Notes: Introduction to Operating Systems

Source: ITEX 220 Operating System syllabus, Unit 1 (5 Hours)
References: Tanenbaum & Bos (2023), Silberschatz et al. (2018)

---

## 1. Definition and Role of an Operating System

An operating system is system software that manages computer hardware and software resources
and provides common services for application programs.

### Dual role

- **Resource manager**: allocates CPU time, memory, I/O devices, and file storage fairly
  and efficiently among competing programs.
- **Extended machine / virtual machine**: hides hardware complexity behind clean, portable
  abstractions (files, processes, sockets) so application code does not need to drive hardware
  directly.

### Why the OS is essential

| Without OS | With OS |
|------------|---------|
| Each program drives hardware directly | Programs use standardized APIs |
| No isolation between programs | Each process has protected memory |
| No fair resource sharing | Scheduler ensures all programs get CPU time |
| No file abstraction | Programs read/write files not raw disk sectors |

---

## 2. Three Views of an Operating System

### 2.1 Application View (user and developer perspective)

The OS is a platform that provides:
- A process execution environment
- File system access
- Network communication
- Security and permissions

Developers interact with the OS through **system calls** and **libraries** (POSIX, Win32, etc.).

### 2.2 System View (resource management perspective)

The OS is a resource manager that controls:
- **CPU**: scheduling processes for fair and efficient use
- **Memory**: allocation, protection, and reclaiming
- **I/O devices**: device drivers, buffering, interrupts
- **Storage**: file systems, disk scheduling

### 2.3 Implementation View (internal architecture perspective)

The OS is a collection of cooperating software modules:
- Kernel: core, runs in privileged mode
- Device drivers: hardware-specific code
- System libraries: bridge between user code and kernel
- Shell: command interpreter sitting on top of system calls

---

## 3. History and Evolution of Operating Systems

| Era | Period | Key Development |
|-----|--------|-----------------|
| Batch systems | 1950s | Jobs submitted in batches; no user interaction |
| Multiprogramming | 1960s | Multiple jobs in memory; CPU switches on I/O wait |
| Time-sharing | 1960s–70s | Many interactive users share one machine (UNIX born 1969) |
| Personal computing | 1980s | Single-user OS; DOS, early Mac OS |
| Client-server | 1990s | Network OS, distributed services |
| Mobile and cloud | 2000s–present | Android/iOS, hypervisors, containers |

### Key insight

Each era solved the most painful limitation of the previous one:
- Batch → multiprogramming solved wasted CPU time.
- Multiprogramming → time-sharing solved responsiveness.
- Time-sharing → personal computers solved cost of access.
- PC → cloud solved capacity and availability.

---

## 4. Types of Operating Systems

### 4.1 By number of users

| Type | Description | Example |
|------|-------------|---------|
| Single-user | One user at a time | MS-DOS, early Windows |
| Multi-user | Many concurrent users, strong isolation | Linux, UNIX, Windows Server |

### 4.2 By number of concurrent tasks

| Type | Description | Example |
|------|-------------|---------|
| Single-tasking | Runs one task at a time | MS-DOS |
| Multi-tasking (preemptive) | OS preempts tasks on timer | Linux, Windows, macOS |
| Multi-tasking (cooperative) | Tasks yield voluntarily | Old Mac OS, Windows 3.x |

### 4.3 Specialized types

**Real-Time OS (RTOS)**
- Guarantees response within a strict time bound (deadline).
- Hard RTOS: missing a deadline is a system failure (e.g., aircraft, pacemaker).
- Soft RTOS: missing a deadline degrades quality but is tolerable (e.g., multimedia).
- Examples: FreeRTOS, VxWorks, QNX.

**Network OS**
- Provides networking primitives, file sharing, and remote login.
- Early examples: Novell NetWare, Windows NT Server.

**Distributed OS**
- Makes a cluster of machines appear as one coherent system.
- Handles location transparency and fault tolerance.
- Examples: Amoeba, Plan 9.

**Embedded OS**
- Minimal footprint for resource-constrained devices.
- Examples: FreeRTOS, embedded Linux, Zephyr.

**Mobile OS**
- Optimized for battery, touch input, and always-on connectivity.
- Examples: Android (Linux kernel), iOS (XNU/Mach kernel).

---

## 5. System Calls and Call Handling

### What is a system call?

A system call is the programmatic interface through which a user-space program requests
a service from the OS kernel.

System calls are the **only safe mechanism** for crossing the user/kernel boundary.

### Common system call categories

| Category | Examples |
|----------|---------|
| Process control | `fork()`, `exec()`, `exit()`, `wait()` |
| File management | `open()`, `read()`, `write()`, `close()` |
| Device management | `ioctl()`, `read()`, `write()` on device |
| Information | `getpid()`, `alarm()`, `sleep()` |
| Communication | `pipe()`, `socket()`, `send()`, `recv()` |

### System call execution flow

1. Application calls a **library wrapper** (e.g., `printf` → `write`).
2. Library loads the syscall number into a CPU register.
3. Library executes a **trap instruction** (software interrupt, e.g., `syscall` on x86-64).
4. CPU switches to **kernel mode** (privilege level 0 on x86).
5. Kernel reads syscall number and looks up the **system call table**.
6. Kernel **dispatches** to the appropriate handler function.
7. Handler executes, places return value in register.
8. CPU switches back to **user mode** and returns to library.
9. Library returns result to application.

### Why the boundary matters

- User mode: limited privileges, cannot directly access hardware or other processes' memory.
- Kernel mode: full hardware access.
- The trap instruction is the controlled gate between them.

---

## 6. System Programs and OS Structures

### System programs

Software that ships with the OS and provides common utilities:
- File management tools (ls, cp, mkdir)
- Status information tools (ps, top, df)
- Text editors (vi, nano)
- Compilers and interpreters
- Networking utilities (ssh, ping, curl)

### OS Kernel Architecture Styles

#### 6.1 Monolithic Kernel

All OS services (scheduler, memory manager, file system, device drivers) run in one large
kernel binary in kernel mode.

- Pros: very fast, no inter-process communication overhead between services.
- Cons: a bug in any service can crash the entire OS.
- Examples: early UNIX, Linux.

#### 6.2 Layered Architecture

OS is divided into a hierarchy of layers, each layer using only services from the layer below.

- Pros: clean abstraction, easier to debug layer by layer.
- Cons: performance overhead from crossing many layers; hard to define clean layers in practice.
- Example: THE multiprogramming system, early OS/2.

#### 6.3 Microkernel

Only essential services (address space management, basic IPC, minimal scheduling) live in
the kernel. Everything else (file system, device drivers, network stack) runs as user-space
server processes communicating via **message passing**.

- Pros: a crash in a server does not crash the kernel; easier to port.
- Cons: message-passing overhead between services.
- Examples: Mach, QNX, MINIX 3, seL4, macOS (uses Mach microkernel + BSD layer).

#### 6.4 Modular / Loadable Kernel Modules

Core kernel plus dynamically loadable modules.

- Modules are loaded/unloaded at runtime.
- Combines some benefits of monolithic (speed) and microkernel (modularity).
- Example: Linux (supports loadable kernel modules, e.g., device drivers).

#### 6.5 Exokernel

Kernel exposes raw hardware with minimal abstraction. Libraries above it implement
OS abstractions (LibOS).

- Pros: maximum flexibility for applications.
- Cons: application developer complexity.
- Example: Xok (MIT research).

### Structure comparison summary

| Structure | Service location | Communication | Speed | Fault isolation |
|-----------|-----------------|---------------|-------|-----------------|
| Monolithic | Kernel space | Direct call | Fast | Poor |
| Layered | Kernel space | Layer-to-layer | Moderate | Moderate |
| Microkernel | User space servers | Message passing | Slower | Excellent |
| Modular | Kernel + modules | Direct call | Fast | Moderate |
| Exokernel | LibOS (user space) | Lib calls | Fastest | Application-level |

---

## 7. Virtual Machines

### What is a virtual machine?

A virtual machine (VM) is a software emulation of a complete computer system.
The **hypervisor** (Virtual Machine Monitor, VMM) creates and manages VMs.

Each VM believes it has dedicated hardware (CPU, memory, disk, NIC) but
actually shares physical hardware with other VMs.

### Types of hypervisors

**Type 1 (bare-metal)**
- Runs directly on hardware, no host OS.
- Higher performance.
- Examples: VMware ESXi, Microsoft Hyper-V, Xen.

**Type 2 (hosted)**
- Runs on top of a host OS.
- Easier to set up, slightly more overhead.
- Examples: VirtualBox, VMware Workstation.

### Key benefits of VMs

- **Isolation**: a crash or security breach in one VM does not affect others.
- **Portability**: a VM image can run on any compatible hypervisor.
- **Consolidation**: many VMs on one physical server reduce hardware cost.
- **Snapshots**: save and restore entire machine state instantly.
- **Testing**: run different OS versions or configurations safely.

### OS relevance

Virtual machines are themselves implemented using OS concepts:
- Each VM's CPU state is a context switched by the hypervisor.
- VM memory is managed using page tables and extended page tables (EPT).
- VM I/O is virtualized through emulated or paravirtual device drivers.

---

## 8. Concept Map

```
Operating System
├── Definition: resource manager + extended machine
├── Views
│   ├── Application: system call API
│   ├── System: CPU + memory + I/O management
│   └── Implementation: kernel + drivers + shell
├── History: batch → multiprog → timesharing → PC → cloud
├── Types
│   ├── Single/Multi user
│   ├── Single/Multi tasking
│   ├── RTOS (hard/soft)
│   ├── Network OS
│   ├── Distributed OS
│   └── Embedded / Mobile
├── System Calls
│   └── trap → kernel mode → dispatch table → handler → return
├── OS Structures
│   ├── Monolithic
│   ├── Layered
│   ├── Microkernel
│   ├── Modular
│   └── Exokernel
└── Virtual Machines
    ├── Type 1 hypervisor (bare-metal)
    └── Type 2 hypervisor (hosted)
```

---

## 9. Theory-to-Practical Mapping

| Topic | Program |
|-------|---------|
| OS layers and views | `01_os_layers.cpp` |
| System call dispatch | `02_system_calls.cpp` |
| OS type properties | `03_os_types.cpp` |
| OS structures | `04_os_structures.cpp` |
| Virtual machine isolation | `05_virtual_machine.cpp` |
