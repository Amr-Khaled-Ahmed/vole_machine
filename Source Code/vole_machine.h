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

using namespace std;

// Memory class
class Memory {
private:
    vector<int> memory;
public:
    Memory();  // Constructor
    ~Memory(); // Destructor
    void set_memory_cells(int address, int value); // Set values on cells of memory
    int get_memory_address(int address); // Get address of memory cells
};

// Registers class
class Registers {
private:
    vector<int> registers;
public:
    Registers();  // Constructor
    ~Registers(); // Destructor

    // Methods related to registers
    int getValue(int RegIndex);
    void setValue(int RegIndex, int value);
    void display();
};



// Program Instructions class
class ProgIns {

public:
    string instruction;

    ProgIns();  // Constructor
    ~ProgIns(); // Destructor

    char getOpCode(); // Extract opcode
    int getRegister(); // Extract register
    int getAddress_Value(); // Extract address/value

    // Methods related to program instructions
    void executeInstructions();
};



class Simulator {
public:
    Simulator(); // Constructor
    void loadProgram(); // Load instructions into memory

private:
    Memory memory;    // Memory object
    Registers registers; // Registers object
    int ProgramCounter; // Program counter
};



// some functions
void slowPrint(const string& message, int delay = 50);
void start_excecution();


#endif
