# Real-Time Edge Signal Processing & Telemetry Node

A lightweight, high-performance C-based telemetry packet engine designed for resource-constrained embedded systems (e.g., STM32, ESP32, AVR). Implements a non-blocking Finite State Machine (FSM) stream parser coupled with a Circular FIFO Ring Buffer to ingest noisy UART/Serial sensor data streams without losing packets.

---

## Architecture Overview

```text
[ Hardware UART / ISR Stream ]
              │
              ▼
    ┌──────────────────┐
    │  Circular Buffer  │  (FIFO Storage - 64 Bytes)
    │   (Ring Buffer)   │
    └─────────┬─────────┘
              │
              ▼
    ┌──────────────────┐
    │    FSM Parser     │  (State Machine Stream Ingestion)
    └─────────┬─────────┘
              │
              ▼
    ┌──────────────────┐
    │   CRC8 Checksum   │  (Error Detection & Validation)
    └─────────┬─────────┘
              │
              ▼
    [ Decoded Frame Output ]
```

---

## Key Features

- **Circular Ring Buffer (FIFO):** Safely stores incoming asynchronous bytes from hardware interrupts (ISR) to prevent buffer overflows during CPU tasks.
- **Byte-by-Byte FSM Parser:** Non-blocking state machine processing bytes sequentially, filtering out transmission noise and preamble junk dynamically.
- **Data Framing & Verification:** Implements custom packet framing (`START_BYTE`, `LENGTH`, `PAYLOAD`, `CRC8`, `END_BYTE`) with bitwise XOR CRC validation.
- **Modular C Architecture:** Strict separation of interface headers (`include/`) and source logic (`src/`) following bare-metal C best practices.

---

## Packet Structure

| Start Byte | Payload Length | Payload Data | CRC Checksum | End Byte |
| :---: | :---: | :---: | :---: | :---: |
| `0xAA` | 1 Byte (`N`) | `N` Bytes (Max 16) | 1 Byte | `0x55` |

---

## Directory Layout

```text
embedded-packet-engine/
├── include/
│   ├── packet_engine.h   # FSM parser states and frame definitions
│   └── ring_buffer.h     # Circular buffer data structures and API
├── src/
│   ├── main.c            # Telemetry stream simulation harness
│   ├── packet_engine.c   # FSM parser logic & CRC validation
│   └── ring_buffer.c     # FIFO ring buffer operations
└── README.md             # Project documentation
```

---

## Build and Run

### Prerequisites

- GCC Compiler (MinGW-w64 / MSYS2 on Windows, or native GCC on Linux)

### Compilation

```bash
gcc src/main.c src/packet_engine.c src/ring_buffer.c -o embedded_engine.exe
```

### Execution

```bash
./embedded_engine.exe
```

### Example Output

```text
=== PHASE 3: Ring Buffer + FSM Parser Pipeline ===

[ISR] Receiving raw stream into Ring Buffer...
  Pushed: 0x77
  Pushed: 0xBB
  Pushed: 0xAA
  Pushed: 0x04
  Pushed: 0x10
  Pushed: 0x20
  Pushed: 0x30
  Pushed: 0x40
  Pushed: 0x40
  Pushed: 0x55

[CPU] Processing Ring Buffer contents through FSM Parser...
  Popped: 0x77 -> Parsing...
  Popped: 0xBB -> Parsing...
  Popped: 0xAA -> Parsing...
  Popped: 0x04 -> Parsing...
  Popped: 0x10 -> Parsing...
  Popped: 0x20 -> Parsing...
  Popped: 0x30 -> Parsing...
  Popped: 0x40 -> Parsing...
  Popped: 0x40 -> Parsing...
  Popped: 0x55 ->

>>> [SUCCESS] Full Frame Decoded! Payload: 0x10 0x20 0x30 0x40 <<<
```

---

## About

A modular C-based UART data framing and CRC validation engine for embedded systems.