# Unit 1 Operating Systems — Introduction

Course: ITEX 220 Operating System (3 credit hours, Theory + Practical)
Syllabus hours for this unit: 5 hours
Exam weight: 1 MCQ, 1 SAQ

---

## What This Unit Covers

Unit 1 introduces the purpose, structure, and evolution of operating systems —
the foundational knowledge every subsequent unit depends on.

Topics:
1. Definition and role of an OS
2. Three views of an OS (Application, System, Implementation)
3. History and evolution of operating systems
4. Types of operating systems
5. System calls and call handling
6. OS structures (monolithic, layered, microkernel, modular, exokernel)
7. Virtual machines

---

## Folder Structure

```
Unit 1/
├── README.md                        ← you are here
├── 01_Theory/
│   ├── Unit1_Detailed_Notes.md      ← deep concept notes
│   ├── Topic_Checklist.md           ← exam/viva readiness
│   └── Verification_Guide.md        ← how to verify with programs
├── 02_Presentation/
│   └── Unit1_Presentation.md        ← Mermaid slide deck
└── 03_Practicals_CPP/
    ├── README.md                    ← practical usage guide
    ├── Makefile                     ← build system
    ├── scripts/
    │   └── validate_all.sh          ← one-command validation
    ├── src/
    │   ├── 01_os_layers.cpp         ← OS layered architecture
    │   ├── 02_system_calls.cpp      ← system call dispatch simulation
    │   ├── 03_os_types.cpp          ← OS type property comparison
    │   ├── 04_os_structures.cpp     ← monolithic vs microkernel model
    │   └── 05_virtual_machine.cpp   ← VM isolation simulation
    └── bin/                         ← compiled executables (auto-generated)
```

---

## Quick Start

```bash
cd "03_Practicals_CPP"
bash scripts/validate_all.sh
```

All five programs should print `Validation: PASS` and the script should print `Overall Result: PASS`.

---

## Study Path

| Step | File | Action |
|------|------|--------|
| 1 | `01_Theory/Unit1_Detailed_Notes.md` | Read full concept coverage |
| 2 | `02_Presentation/Unit1_Presentation.md` | Review diagrams for class |
| 3 | `03_Practicals_CPP/src/*.cpp` | Study code topic by topic |
| 4 | `03_Practicals_CPP/scripts/validate_all.sh` | Run validation |
| 5 | `01_Theory/Topic_Checklist.md` | Self-assess before exam |

---

## Exam Guidance (Unit 1)

- MCQ (1 mark): expect questions on OS definition, types, system call purpose.
- SAQ (4 marks): expect "explain OS views" or "compare OS structures".
- Unit 1 has **no LAQ requirement** per the marking scheme.
