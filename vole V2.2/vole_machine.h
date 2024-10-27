#ifndef VOLE_MACHINE_H
#define VOLE_MACHINE_H

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>

using namespace std;

// Forward declaration of the Registers class
class Registers;

// Function to convert float to IEEE754 representation
int float_IEEE754(float num);

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
void start_execution();

#endif // VOLE_MACHINE_H
