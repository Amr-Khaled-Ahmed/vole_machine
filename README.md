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

## Instruction Set
| Opcode | Format | Description                                                                                          |
|--------|--------|------------------------------------------------------------------------------------------------------|
| `1 RXY`| Load   | Load register `R` with the 8-bit value from memory location `XY`.                                    |
| `2 RXY`| Load   | Load register `R` with the constant 8-bit value `XY`.                                                |
| `3 RXY`| Store  | Store the 8-bit value in register `R` to memory location `XY`.                                       |
| `4 0RS`| Move   | Copy the 8-bit value from register `R` to register `S`.                                              |
| `5 RST`| Add    | Add the values of registers `S` and `T`, storing the result in register `R`.                         |
| `6 RST`| FAdd   | Perform floating-point addition of registers `S` and `T`, storing the result in `R`.                 |
| `7 RST`| Subtract | Subtract the value in register `T` from register `S` and store the result in register `R`.        |
| `8 RST`| Multiply | Multiply values in registers `S` and `T` and store the result in register `R`.                     |
| `9 RST`| Divide | Divide the value in register `S` by the value in register `T`, storing the result in register `R`.   |
| `A RXY`| Compare| Set a flag if register `R` holds a value equal to memory location `XY`; otherwise, clear the flag.   |
| `B RXY`| Jump   | Jump to the instruction at address `XY` if the value in register `R` equals zero.                    |
| `C 000`| Halt   | Stop the execution of the simulator.                                                                 |


## Installation
To get started with the Vole Machine Simulator:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/mohamedahmed2005/vole-machine-simulator.git
   cd vole-machine-simulator
