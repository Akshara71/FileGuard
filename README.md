# FileGuard

A user-space RAID-based virtual storage system written in C for Ubuntu / WSL.
Operating Systems & System Programming (PBL) capstone project.

**Use case:** A company stores important data across multiple disks. If one disk
fails, the system should continue working and the data should remain available.

FileGuard simulates physical disks using ordinary files (`data/disk1.img` to
`data/disk4.img`). It is **not** kernel-level RAID or hardware RAID.

## Team

| Name | Roll No. |
|------|----------|
| Akshara.V | [2500031100] |
| Dhanashree.K | [2500031934] |
| Meghana.A| [2500032093] |

## Current status (Review-1)

| Feature | Status |
|---------|--------|
| Create 4 virtual disks with headers (`init`) | Implemented |
| Disk status report (`status`) | Implemented |
| Striped logical volume of 4096 blocks | Implemented |
| Write / read / layout commands | Implemented |
| RAID-5 parity | Planned |
| Disk failure detection and simulation | Planned |
| Disk rebuild / recovery | Planned |
| Processes, IPC, signals, threads, mmap | Planned |

The current volume uses striping only, so it has **no fault tolerance yet**.

## Build and run

Requires Ubuntu / WSL with `gcc` and `make` (package `build-essential`).

```bash
make
./fileguard init
./fileguard write 0 "Hello FileGuard"
./fileguard read 0
./fileguard layout
./fileguard status
```

## Commands

| Command | Description |
|---------|-------------|
| `init` | Create the 4 virtual disks (erases existing data) |
| `status` | Show size, blocks and state of each disk |
| `write <block> "text"` | Write text to a logical block (0 to 4095) |
| `read <block>` | Read a logical block |
| `layout [block]` | Show which disk a logical block maps to |

## Project structure

| Path | Purpose |
|------|---------|
| `src/` | C source files (`main.c`, `volume.c`, `disk.c`) |
| `include/` | Header files |
| `data/` | Virtual disk images created at runtime (not committed) |
| `docs/` | Documentation, diagrams and screenshots |
| `tests/` | Test scripts (to be added) |
| `Makefile` | Build rules |
