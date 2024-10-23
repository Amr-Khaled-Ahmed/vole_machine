#include "vole_machine.h"
#include<bits/stdc++.h>
using namespace std;
// Memory class implementation
Memory::Memory() : memory(256, 0) { // make vector with 256 index and all are zeros
    cout << "Memory initialized with 256 cells."<<endl;
}
void Memory::set_memory_cells(int memory_address,int inside_value) {
    if(memory_address>=0) { // check if memory cell is greater than or equal to 0 or not
        if(memory_address<256) { // check if memory cell is smaller than to 256 or not
            memory[memory_address] = inside_value;
            cout<<"Memory set at "<<memory_address<<endl;
        }
    }
    else {
        cout<<"invalid address: "<<memory_address<<endl; // invalid address
    }
}
int Memory::get_memory_address(int memory_address) {
    if(memory_address>=0) { // check if memory cell is greater than or equal to 0 or not
        if(memory_address<256) { // check if memory cell is smaller than to 256 or not
            return memory[memory_address];
        }
        else {
            cout<<"invalid address: "<<memory_address<<endl; // invalid address
            return 0;
        }
    }
}

Memory::~Memory() { // Destructor to kill class
    cout << "Memory destroyed." << endl;
}
// Registers class implementation
Registers::Registers() {
    //cout << "Registers initialized." << endl;
}

Registers::~Registers() {
    //cout << "Registers destroyed." << endl;
}

void Registers::setRegisters() {
    //cout << "Setting registers..." << endl;
}

// Bytes class implementation
Bytes::Bytes() {
    //cout << "Bytes initialized." << endl;
}

Bytes::~Bytes() {
    //cout << "Bytes destroyed." << endl;
}

void Bytes::manipulateBytes() {
    //cout << "Manipulating bytes..." << endl;
}

// Program Instructions class implementation
ProgIns::ProgIns() {
    //cout << "Program instructions initialized." << endl;
}

ProgIns::~ProgIns() {
    //cout << "Program instructions destroyed." << endl;
}

void ProgIns::executeInstructions() {
    //cout << "Executing program instructions..." << endl;
}

// Slow-print utility function
void slowPrint(const std::string& message, int delay) {
    for (char c : message) {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << endl;
}


void start_excecution() {
    Memory memory;
    memory.set_memory_cells(10, 42);
    int memory_value = memory.get_memory_address(300);
    Registers registers;
    registers.setRegisters();
    Bytes bytes;
    bytes.manipulateBytes();
    ProgIns progIns;
    progIns.executeInstructions();
}
