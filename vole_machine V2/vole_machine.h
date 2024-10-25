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
#include<bitset>
using namespace std;
void printIEEE754(float num);
int float_IEEE754(float num);

// Memory class

class Memory {
private:
    vector<uint8_t> memory; // Store memory as 8-bit cells
public:
    Memory();
    void set_memory_cells(int memory_address, int inside_value);
    int get_memory_address(int memory_address);
    void display_memory_cells();
    ~Memory();
};

// Registers class
class Registers {
private:
    vector<int> registers; // Store register values
public:
    Registers();
    int getValue(int RegIndex);
    void setValue(int RegIndex, int value);
    vector<int> get_registers_vector();
    void display();
    int getRegister_0();
    void loadFromMemory(Memory &memory, int memory_address, int regIndex); // Load from memory to register
    void storeToMemory(Memory &memory, int regIndex, int memory_address); // Store register value to memory
    ~Registers();


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
