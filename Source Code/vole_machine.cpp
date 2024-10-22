#include "vole_machine.h"


// Memory class implementation
Memory::Memory() {
    //cout << "Memory initialized." << endl;
}

Memory::~Memory() {
    //cout << "Memory destroyed." << endl;
}

void Memory::loadMemory() {
    //cout << "Loading memory..." << endl;
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


void startExcution() {
    // Create objects for each class
    Memory memory;
    Registers registers;
    Bytes bytes;
    ProgIns progIns;

    // Call methods on the objects
    memory.loadMemory();
    registers.setRegisters();
    bytes.manipulateBytes();
    progIns.executeInstructions();

}