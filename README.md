📦 Embedded Packet Engine
==========================

🚀 Overview
-----------

**Embedded Packet Engine** is a lightweight, modular C-based framework for real-time telemetry packet processing on resource-constrained embedded systems (e.g., STM32, ESP32, AVR).

It combines a non-blocking **Finite State Machine (FSM) parser** with a **Circular FIFO Ring Buffer** to ingest noisy UART/serial sensor data streams without dropping packets — making it a solid foundation for embedded communication systems, sensor telemetry, or simulation environments.

---

🎯 Key Features
----------------

- ⚙️ Non-blocking, byte-by-byte FSM packet parser
- 🔄 Circular FIFO ring buffer for safe ISR-to-CPU data handoff
- 📡 Custom packet framing (`START_BYTE`, `LENGTH`, `PAYLOAD`, `CRC8`, `END_BYTE`)
- ✅ CRC8 (XOR-based) checksum validation for error detection
- 🧠 Lightweight, bare-metal-friendly design — no dynamic allocation required
- 🧱 Modular architecture with clean header/source separation
- 🧪 Built-in simulation harness for testing the pipeline end-to-end

---

🧱 Architecture
----------------

The engine is built around a straight-line ingestion pipeline:

```text
[ Hardware UART / ISR Stream ]
              ↓
     [ Circular Ring Buffer ]      ← FIFO storage (64 bytes)
              ↓
        [ FSM Parser ]             ← state-machine frame decoding
              ↓
       [ CRC8 Checksum ]           ← validation & error detection
              ↓
     [ Decoded Frame Output ]
```

Each stage is independent, so you can:

- swap in a larger/smaller ring buffer
- add custom FSM states for new frame types
- extend validation beyond CRC8
- hook the decoded output into your own routing/handler logic

---

📡 Packet Structure
---------------------

| Start Byte | Payload Length | Payload Data | CRC Checksum | End Byte |
| :---: | :---: | :---: | :---: | :---: |
| `0xAA` | 1 Byte (`N`) | `N` Bytes (Max 16) | 1 Byte | `0x55` |

---

📁 Project Structure
----------------------

```text
embedded-packet-engine/
│
├── include/
│   ├── packet_engine.h   # FSM parser states and frame definitions
│   └── ring_buffer.h     # Circular buffer data structures and API
├── src/
│   ├── main.c            # Telemetry stream simulation harness
│   ├── packet_engine.c   # FSM parser logic & CRC validation
│   └── ring_buffer.c     # FIFO ring buffer operations
└── README.md             # Documentation
```

---

⚙️ Build Instructions
------------------------

### 🔧 Requirements

- GCC compiler (MinGW-w64 / MSYS2 on Windows, or native GCC on Linux)

### 🛠️ Compile

```bash
gcc src/main.c src/packet_engine.c src/ring_buffer.c -o embedded_engine.exe
```

### ▶️ Run

```bash
./embedded_engine.exe
```

### Expected Output

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

🧠 Design Philosophy
----------------------

This project follows three core principles:

1. **Simplicity** → minimal overhead, clear byte-by-byte data flow
2. **Modularity** → ring buffer, parser, and validation are independent units
3. **Reliability** → non-blocking design and CRC validation built for noisy, real-world serial links

---

📌 Future Improvements
-------------------------

- [ ] Support larger/variable-size payloads
- [ ] Add a Makefile for streamlined builds
- [ ] Add a `tests/` suite for automated validation
- [ ] Real UART/DMA integration examples (STM32/ESP32 HAL)
- [ ] Configurable checksum (CRC8 → CRC16/CRC32)
- [ ] Memory/footprint optimization pass for constrained MCUs

---

👨‍💻 Author
-------------
ADEM FATTOUCH
---

📜 License
------------

This project is intended for educational and research purposes.
