# Vole Machine Simulator

## Project Overview
This project aims to develop a **Vole Machine Simulator** that can execute a set of machine instructions as defined by a custom instruction set architecture (ISA). The simulator will be capable of loading programs from a file and executing them based on the opcodes provided.

## Features (Planned)
- **Memory Management**: The simulator will include a memory model to store instructions and data.
- **Registers**: It will simulate 16 general-purpose registers, each holding an 8-bit value.
- **Instruction Set Support**: The machine will support a variety of instructions including:
  - **Load**: Load values from memory or constants into registers.
  - **Store**: Store register values into memory.
  - **Move**: Transfer values between registers.
  - **Arithmetic Operations**: Perform addition in two's complement and floating-point formats.
  - **Jump**: Conditional program jumps based on comparisons.
  - **Halt**: Stop execution.

## Instruction Set (Summary)
| Opcode | Description                                                                 |
|--------|-----------------------------------------------------------------------------|
| 1 RXY  | Load the register `R` with the bit pattern from memory location `XY`.       |
| 2 RXY  | Load the register `R` with the constant `XY`.                               |
| 3 RXY  | Store the bit pattern from register `R` into memory location `XY`.          |
| 4 0RS  | Move the bit pattern from register `R` to register `S`.                     |
| 5 RST  | Add values from registers `S` and `T` and store the result in register `R`. |
| 6 RST  | Perform floating-point addition of registers `S` and `T`, result in `R`.    |
| B RXY  | Jump to instruction at address `XY` if register `R` equals register `0`.    |
| C 000  | Halt execution.                                                             |

## Installation
To get started with the Vole Machine Simulator:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/vole-machine-simulator.git
