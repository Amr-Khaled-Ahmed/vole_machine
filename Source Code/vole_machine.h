#ifndef VOLE_MACHINE_H
#define VOLE_MACHINE_H

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// Memory class
class Memory {
public:
    Memory();  // Constructor
    ~Memory(); // Destructor

    // Methods related to memory
    void loadMemory();
};

// Registers class
class Registers {
public:
    Registers();  // Constructor
    ~Registers(); // Destructor

    // Methods related to registers
    void setRegisters();
};

// Bytes class
class Bytes {
public:
    Bytes();  // Constructor
    ~Bytes(); // Destructor

    // Methods related to bytes
    void manipulateBytes();
};

// Program Instructions class
class ProgIns {
public:
    ProgIns();  // Constructor
    ~ProgIns(); // Destructor

    // Methods related to program instructions
    void executeInstructions();
};

// Utility function to slow-print messages
void slowPrint(const string& message, int delay = 50);
void startExcution();


#endif
