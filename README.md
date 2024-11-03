# Vole Machine Simulator

## Overview
The **Vole Machine Simulator** is a custom virtual machine capable of executing a predefined set of instructions defined by a unique instruction set architecture (ISA). This simulator can load programs from files and execute them based on the provided opcodes, allowing users to emulate machine-level operations.

## Features
- **Memory Management**: The simulator includes a memory model for storing instructions and data.
- **Registers**: Supports 16 general-purpose 8-bit registers for flexible data manipulation.
- **Comprehensive Instruction Set**: Executes operations such as:
  - **Load, Store, Move**: Basic data manipulation between memory and registers.
  - **Arithmetic**: Supports addition, floating-point addition, subtraction, multiplication, and division.
  - **Jumping**: Conditional branching based on comparison operations.
  - **Bitwise Operations**: Supports AND, OR, and NOT for binary data manipulation.
  - **Halt**: Cleanly terminates program execution.

# Instruction Set

| Op-Code | Format | Operand | Description                                                                                          |
|---------|--------|---------|------------------------------------------------------------------------------------------------------|
| `1`     | Load   | `RXY`   | LOAD the register `R` with the bit pattern found in the memory cell whose address is `XY`. Example: `14A3` would cause the contents of the memory cell located at address `A3` to be placed in register `4`. |
| `2`     | Load   | `RXY`   | LOAD the register `R` with the bit pattern `XY`. Example: `20A3` would cause the value `A3` to be placed in register `0`. |
| `3`     | Store  | `RXY`   | STORE the bit pattern in register `R` into the memory cell at address `XY`. Example: `30A3` would store the contents of register `0` into memory cell `A3`. |
| `4`     | Move   | `0RS`   | MOVE the bit pattern found in register `R` to register `S`. Example: `40A4` would cause the contents of register `A` to be copied into register `4`. |
| `5`     | Add    | `RST`   | ADD the bit patterns in registers `S` and `T` and place the result in register `R`. Example: `5C4B` would cause the result of adding the contents of registers `C` and `B` to be placed in register `4`. |
| `6`     | FAdd   | `RST`   | Perform floating-point ADDITION on the values in registers `S` and `T`, storing the result in register `R`. Example: `60C5` would add the floating-point values in registers `C` and `5` and store the result in register `C`. |
| `7`     | OR     | `RST`   | OR the bit patterns in registers `S` and `T` and place the result in register `R`. Example: `7CB4` would cause the result of ORing the contents of registers `B` and `4` to be placed in register `C`. |
| `8`     | AND    | `RST`   | AND the bit patterns in registers `S` and `T` and place the result in register `R`. Example: `8045` would cause the result of ANDing the contents of registers `4` and `5` to be placed in register `0`. |
| `9`     | EXCLUSIVE OR | `RST` | EXCLUSIVE OR the bit patterns in registers `S` and `T` and place the result in register `R`. Example: `95F3` would cause the result of EXCLUSIVE ORing the contents of registers `F` and `3` to be placed in register `5`. |
| `A`     | Rotate | `R0X`   | ROTATE the bit pattern in register `R` one bit to the right `X` times. Each time, place the bit that started at the low-order end at the high-order end. Example: `A403` would cause the contents of register `4` to be rotated 3 bits to the right in a circular fashion. |
| `B`     | Jump   | `RXY`   | JUMP to the instruction at address `XY` if the value in register `R` is non-zero. Example: `B0A3` would jump to address `A3` if the contents of register `0` are non-zero. |
| `C`     | Halt   | `000`   | HALT execution. Example: `C000` would cause program execution to stop. |


## Installation
To get started with the Vole Machine Simulator:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/mohamedahmed2005/vole-machine-simulator.git
   cd vole-machine-simulator
