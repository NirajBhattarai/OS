# Unit 1 (OS): Introduction to Operating Systems

Source: ITEX 220 Operating System — Unit 1 (5 Hours)

---

## Slide 1 — What Is an Operating System?

- Software layer between hardware and user applications.
- Two core roles: **resource manager** and **extended machine**.
- Every program uses it through **system calls**.
- Without an OS every program would control hardware directly — chaos.

```mermaid
%%{init: {'theme':'base','themeVariables':{'primaryColor':'#e8f5e9','primaryBorderColor':'#2f855a','lineColor':'#1f2937','secondaryColor':'#fff8e1','tertiaryColor':'#e3f2fd'}}}%%
flowchart TD
    HW["⚙️ Hardware\n(CPU · Memory · Disk · NIC)"]
    K["🖥️ Operating System Kernel\n(Resource Manager)"]
    SP["🔧 System Programs\n(Shell · Compilers · Utilities)"]
    APP["📱 User Applications\n(Browser · Editor · Game)"]

    HW --> K
    K --> SP
    SP --> APP
    APP -->|"system call"| K
    K -->|"hardware access"| HW
```

---

## Slide 2 — Three Views of an OS

- **Application view**: a clean API of services (files, processes, network).
- **System view**: a manager that allocates CPU, memory, and I/O.
- **Implementation view**: kernel + drivers + libraries cooperating internally.

```mermaid
%%{init: {'theme':'base','themeVariables':{'primaryColor':'#fff3e0','primaryBorderColor':'#d97706','lineColor':'#374151'}}}%%
flowchart LR
    subgraph APP_VIEW["Application View"]
        direction TB
        A1[System Call API]
        A2[File Abstraction]
        A3[Process Environment]
    end
    subgraph SYS_VIEW["System View"]
        direction TB
        B1[CPU Scheduler]
        B2[Memory Manager]
        B3[I/O Manager]
    end
    subgraph IMPL_VIEW["Implementation View"]
        direction TB
        C1[Kernel Core]
        C2[Device Drivers]
        C3[Shell / Libraries]
    end

    APP_VIEW --- SYS_VIEW
    SYS_VIEW --- IMPL_VIEW
```

---

## Slide 3 — History and Evolution of OS

- Each era solved the biggest pain point of the previous one.
- Modern cloud and containers are the latest evolution step.

```mermaid
%%{init: {'theme':'base','themeVariables':{'primaryColor':'#f3e8ff','primaryBorderColor':'#7c3aed','lineColor':'#374151'}}}%%
flowchart LR
    B["1950s\nBatch Systems\n(no interaction)"]
    M["1960s\nMultiprogramming\n(no idle CPU)"]
    T["1960–70s\nTime-sharing\nUNIX born 1969"]
    P["1980s\nPersonal Computers\nDOS · Mac OS"]
    N["1990s\nNetwork OS\nClient-server"]
    C["2000s–now\nCloud · Mobile\nContainers · VMs"]

    B -->|solved idle CPU| M
    M -->|solved responsiveness| T
    T -->|solved cost of access| P
    P -->|solved connectivity| N
    N -->|solved scale| C
```

---

## Slide 4 — Types of Operating Systems

```mermaid
%%{init: {'theme':'base','themeVariables':{'primaryColor':'#e0f2fe','primaryBorderColor':'#0284c7','lineColor':'#334155'}}}%%
flowchart TD
    OS[Operating System Types]

    OS --> U[By Users]
    OS --> T[By Tasks]
    OS --> S[Specialized]

    U --> U1[Single-user\neg. MS-DOS]
    U --> U2[Multi-user\neg. Linux Server]

    T --> T1[Single-tasking\neg. MS-DOS]
    T --> T2[Multi-tasking\neg. Linux · Windows]

    S --> S1[Hard RTOS\neg. pacemaker · aircraft]
    S --> S2[Soft RTOS\neg. multimedia player]
    S --> S3[Embedded OS\neg. FreeRTOS]
    S --> S4[Distributed OS\neg. Plan 9]
    S --> S5[Mobile OS\neg. Android · iOS]
```

---

## Slide 5 — System Calls: The User/Kernel Gateway

- A system call is the **only controlled way** into kernel mode.
- Protected by a CPU **trap instruction** that raises privilege.
- The kernel validates, executes, and returns results safely.

```mermaid
%%{init: {'theme':'base','themeVariables':{'primaryColor':'#ecfeff','primaryBorderColor':'#0891b2','lineColor':'#0f172a','actorBorder':'#0e7490'}}}%%
sequenceDiagram
    autonumber
    participant App as User Application
    participant Lib as C Library (glibc)
    participant CPU as CPU Register
    participant Trap as TRAP Instruction
    participant KD as Kernel Dispatch Table
    participant SH as Syscall Handler

    App->>Lib: call write(fd, buf, n)
    Lib->>CPU: load syscall number (e.g. 1)
    Lib->>Trap: execute syscall instruction
    Trap->>Trap: switch CPU to kernel mode
    Trap->>KD: lookup handler for number 1
    KD->>SH: dispatch to sys_write()
    SH-->>Trap: return bytes written
    Trap->>Trap: switch CPU back to user mode
    Trap-->>App: return value to caller
```

---

## Slide 6 — OS Kernel Structures Compared

```mermaid
%%{init: {'theme':'base','themeVariables':{'primaryColor':'#fef9c3','primaryBorderColor':'#ca8a04','lineColor':'#1f2937'}}}%%
flowchart TD
    subgraph MONO["Monolithic Kernel (Linux)"]
        direction LR
        M1[Scheduler] --- M2[FS] --- M3[Driver] --- M4[Net]
    end
    subgraph MICRO["Microkernel (MINIX 3 · QNX)"]
        direction TB
        K2["Tiny Kernel\n(IPC + Scheduler only)"]
        FS2["File Server\n(user space)"]
        DRV2["Driver Server\n(user space)"]
        K2 <-->|message| FS2
        K2 <-->|message| DRV2
    end
    subgraph MOD["Modular / LKM (Linux)"]
        direction LR
        MK[Core Kernel] -->|load/unload| LM1[Module A]
        MK -->|load/unload| LM2[Module B]
    end

    MONO -->|fast but fragile| MONO
    MICRO -->|isolated but slower| MICRO
    MOD -->|flexible balance| MOD
```

---

## Slide 7 — Virtual Machines

- A **hypervisor** presents each VM with a complete virtual computer.
- VMs are isolated: a crash or breach in one VM cannot affect another.
- Foundational to modern cloud (AWS EC2, GCP, Azure all use Type 1 hypervisors).

```mermaid
%%{init: {'theme':'base','themeVariables':{'primaryColor':'#fce7f3','primaryBorderColor':'#be185d','lineColor':'#1f2937'}}}%%
flowchart TD
    HW["Physical Hardware\n(CPU · RAM · Disk · NIC)"]

    subgraph T1["Type 1 Bare-metal Hypervisor\neg. VMware ESXi · Xen"]
        VM1["VM 1\nLinux"]
        VM2["VM 2\nWindows"]
        VM3["VM 3\nFreeBSD"]
    end

    subgraph T2["Type 2 Hosted Hypervisor\neg. VirtualBox · VMware Workstation"]
        HostOS["Host OS (macOS / Linux)"]
        GVM1["Guest VM 1"]
        GVM2["Guest VM 2"]
        HostOS --> GVM1
        HostOS --> GVM2
    end

    HW --> T1
    HW --> T2
```

---

## Slide 8 — Exam Summary Card

| Topic | Key Fact to Remember |
|-------|---------------------|
| OS Definition | Resource manager + extended machine |
| Application view | Programs see clean system call API |
| System view | OS manages CPU, memory, I/O |
| History | batch → multiprog → timesharing → PC → cloud |
| Types | Single/multi user+task, RTOS, embedded, distributed |
| System call | Trap → kernel mode → dispatch table → handler |
| Monolithic | All in kernel, fast, poor fault isolation |
| Microkernel | Minimal kernel, IPC servers, excellent isolation |
| Type 1 VM | Hypervisor on bare metal — cloud standard |
| Type 2 VM | Hypervisor on host OS — developer use |

---

## Slide 9 — Student Verification Activity

- Read `01_Theory/Unit1_Detailed_Notes.md` for deep coverage.
- Tick each item in `01_Theory/Topic_Checklist.md`.
- Run C++ validation suite to verify all five concepts programmatically:

```bash
cd "03_Practicals_CPP"
bash scripts/validate_all.sh
```

- Expected result: all five programs print `Validation: PASS`.
