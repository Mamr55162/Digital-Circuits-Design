# ⚡ Digital Circuits Design Simulator

> **A modern C++ digital logic simulation library inspired by real-world TTL/CMOS integrated circuits, accompanied by complete NI Multisim hardware implementations.**

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Cross--Platform-lightgrey)
![Status](https://img.shields.io/badge/Status-Actively%20Developed-success)

---

# 📖 Overview

This project is a comprehensive implementation of **Digital Logic Design** in modern C++, recreating many of the most commonly used combinational logic circuits and real-world 74HC-series integrated circuits entirely in software.

Unlike simple truth-table demonstrations, every circuit is constructed from the ground up using **NAND gates**, demonstrating the principle of **functional completeness** exactly as digital hardware is built.

Every major combinational circuit is also recreated graphically using **NI Multisim**, allowing direct comparison between software simulation and actual digital hardware implementations.

The project serves simultaneously as

- 📚 an educational resource
- 🔬 a digital logic simulator
- 🧠 a Boolean algebra playground
- ⚙️ a reusable C++ library
- 💡 a bridge between hardware and software engineering

---

# ✨ Features

## 🔹 Fundamental Logic Gates

Implemented entirely using **NAND gates**:

- NOT
- AND
- OR
- NOR
- XOR
- XNOR
- Tri-State Buffer

---

## 🔹 Arithmetic Circuits

- Half Adder
- Full Adder
- Half Subtractor
- Full Subtractor
- 2×2 Binary Multiplier
- 4×4 Binary Multiplier
- Carry Lookahead Adder
- 74HC283 4-Bit Binary Adder/Subtractor

---

## 🔹 Comparators

- Identity Comparator
- 74HC85 4-Bit Magnitude Comparator

---

## 🔹 Multiplexers

- 4-to-1 Multiplexer
- 74HC151 8-to-1 Multiplexer
- Decoder-built Multiplexer

---

## 🔹 Decoders

- 2-to-4 Decoder
- 74HC154 4-to-16 Decoder
- 74HC42 BCD-to-Decimal Decoder
- BCD to 7-Segment Display Decoder

---

## 🔹 Encoders

- 4-to-2 Encoder
- Decimal-to-BCD Encoder
- 74HC148 Priority Encoder
- 74HC147 Decimal Priority Encoder

---

## 🔹 Digital Signal Utilities

- Wire abstraction
- LED indicators
- Bus monitor
- Binary display
- Decimal display
- Probe utility

---

## 🔹 Waveform Analysis Toolkit

Includes utilities for

- Clock generation
- ASCII waveform generation
- Timed wave simulation
- Rising edge detection
- Falling edge detection
- Pulse counting
- Duty cycle calculation
- Glitch density analysis
- Signal inversion
- Signal concatenation
- Signal slicing
- Signal stretching
- Propagation delay simulation
- Bitwise signal operations

---

## 🔹 Boolean Logic Utilities

Automatic generation of

- Truth Tables
- SOP Expressions
- POS Expressions
- Gate construction estimation
- NAND-only implementations

for

- 2-variable functions
- 3-variable functions
- 4-variable functions

---

# 🧱 Architecture

The project follows a modular object-oriented architecture.

```
Digital-Circuits-Design/
│
├── include/
|   ├── combinational_circuits.h
|
├── src/
│   ├── combinational_circuits.cpp
|
├── test_cases/
|   ├── test_combinational_circuits.cpp
│
├── multisim-design/
│   ├── Half Adder
│   ├── Full Adder
│   ├── Multiplexers
│   ├── Decoders
│   ├── Comparators
│   ├── Encoders
│   └── ...
│
├── LICENSE
└── README.md
```

Each circuit is implemented as an independent reusable function while sharing a common NAND-based gate implementation.

---

# 🖥️ Technologies Used

- **C++17**
- **NI Multisim**
- **Object-Oriented Programming**
- **Boolean Algebra**
- **Digital Logic Design**
- **74HC-Series IC Architecture**

---

# 🔬 Hardware Verification

One of the main goals of this project is ensuring that software implementations match actual hardware behavior.

For that reason,

✅ Every major combinational circuit included in the C++ library has an equivalent implementation designed and simulated using **NI Multisim**.

This allows direct comparison between:

- Boolean equations
- Hardware implementation
- Software implementation

---

# 🧪 Testing

Every public function in the library is tested using

```
test_combinational_circuits.cpp
```

Compile and run:

```bash
g++ -std=c++17 combinational_circuits.cpp test_combinational_circuits.cpp -o test_combinational_circuits

./test_combinational_circuits
```

The testing suite covers every implemented circuit, verifying expected outputs across a wide variety of input combinations.

---

# 🚀 Example

```cpp
bitset<4> A("0101");
bitset<4> B("0011");

auto result = combinational_circuits::_74HC283_(A, B, false, false);

Indicator::BinaryVector(result);
```

Output

```
01000
```

---

# 🎯 Educational Goals

This project was built to deepen understanding of

- Digital Logic Design
- Boolean Algebra
- Combinational Circuit Design
- Hardware Description Concepts
- Computer Architecture
- Logic Optimization
- Integrated Circuit Design
- Hardware Simulation
- C++ Software Engineering

It is intended for

- Computer Engineering students
- Electrical Engineering students
- Embedded Systems developers
- FPGA beginners
- Digital Logic enthusiasts
- Anyone learning how hardware really works

---

# 📚 Current Implemented Components

- ✅ Logic Gates
- ✅ Arithmetic Circuits
- ✅ Comparators
- ✅ Multiplexers
- ✅ Decoders
- ✅ Encoders
- ✅ Waveform Utilities
- ✅ Boolean Logic Converters
- ✅ Digital Signal Utilities

---

# 🔮 Future Roadmap

The long-term vision is transforming this repository into a complete Digital Logic Simulation Framework.

Planned additions include

- Sequential Logic
- SR Latch
- D Flip-Flop
- JK Flip-Flop
- T Flip-Flop
- Registers
- Shift Registers
- Counters
- RAM Simulation
- ROM Simulation
- ALU
- CPU Components
- Finite State Machines
- Clock Tree Simulation
- Timing Analysis
- Hazard Detection
- GUI-based Circuit Simulator
- Interactive Waveform Viewer
- Verilog/VHDL Export
- Complete Educational Documentation

---

# 🤝 Contributions

Contributions are always welcome.

If you have

- new IC implementations,
- optimization ideas,
- additional Multisim designs,
- documentation improvements,
- or educational examples,

feel free to open an Issue or submit a Pull Request.

---

# ⭐ If you enjoyed this project...

Consider giving the repository a ⭐.

It helps others discover the project and motivates future development.

---

# 📜 License

This project is licensed under the **MIT License**.

See the `LICENSE` file for details.
