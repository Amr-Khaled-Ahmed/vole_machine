#ifndef VOLE_MACHINE_H
#define VOLE_MACHINE_H

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <string>
#include <cctype>
#include <sstream>
#include <fstream>
#include <bitset>
#include<cmath>
using namespace std;

// Forward declaration of the Registers class
class Registers;
// Function to decode the custom hex format to decimal
double decode_to_decimal(uint8_t hexValue);
// Function to encode a decimal number to the custom hex format
uint8_t encode_from_decimal(double value);
// Function to add two values in the custom format and return the result in hex format
uint8_t addCustomFormat(uint8_t hex1, uint8_t hex2);
// Registers class
class Registers {
private:
    vector<int> registers; // Store register values
public:
    Registers();
    int Get_Register_Value(int regIndex); // Get value from a register
    void setValue_to_register(int regIndex, int value); // Set value in a register
    void display_Registers(); // Display register contents
    ~Registers();
};

// Memory class inheriting from Registers
class Memory { // Inherit from Registers
private:
    vector<uint8_t> memory; // Store memory as 8-bit cells
public:
    Memory();
    void set_memory_cells(int memory_address, int regIndex); // Sets memory using a register value
    int get_memory_address(int memory_address);
    void display_memory_cells();
    ~Memory();
};

// Program Instructions class
class ProgIns {
public:
    string instruction;

    ProgIns();  // Constructor
    ~ProgIns(); // Destructor

    char getOpCode_from_ins(); // Extract opcode
    int getRegister_from_ins(); // Extract register index
    int getAddress_Value_from_ins(); // Extract address/value

    // Method to execute program instructions
    void executeInstructions();
};

// Simulator class
class Simulator {
public:
    Simulator(); // Constructor
    void loadProgram(); // Load instructions into memory

private:
    Memory memory;    // Memory object
    Registers registers; // Registers object
    int ProgramCounter; // Program counter
};

// Utility functions
void slowPrint(const string& message, int delay = 50);

#endif // VOLE_MACHINE_H