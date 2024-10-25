#include "vole_machine.h"
// Function that convert float number that can be positive or negative to IEEE754 representation
void printIEEE754(float num) {
    uint8_t binaryRepresentation = 0; // Initialize an 8-bit variable to store the final binary representation

    // Step 1: Copy the bytes of the float into a 32-bit integer
    uint32_t temp = 0;
    memcpy(&temp, &num, sizeof(num)); // Copy the raw byte structure of the float into a 32-bit integer variable

    // Step 2: Extract the sign, exponent, and mantissa
    uint8_t sign = (temp >> 31) & 1; // Extract the sign bit (left-most bit in a 32-bit float)
    int8_t exponent = ((temp >> 23) & 255) - 127; // Extract and un-bias the 8-bit exponent field (32-bit float has a bias of 127)
    uint8_t mantissa = (temp >> 19) & 15; // Keep the first 4 bits of mantissa for 8-bit precision

    // Step 3: Adjust exponent for 8-bit representation (new bias is 3)
    exponent += 3; // Re-bias the exponent to fit within 8-bit format (bias of 3 for 3-bit exponent field)
    if (exponent < 0) exponent = 0; // Clamp exponent to 0 if it's too small
    if (exponent > 7) exponent = 7; // Clamp exponent to 7 if it's too large for 3 bits

    // Step 4: Construct the 8-bit representation
    binaryRepresentation = (sign << 7) | (exponent << 4) | mantissa; // Combine sign, exponent, and mantissa into a single 8-bit variable

    // Step 5: Print the result using bitset
    bitset<8> bits(binaryRepresentation); // Convert 8-bit representation to a bitset for easy printing
    cout << "Sign: " << bits[7] << endl; // Print sign bit
    cout << "Exponent: " << bits.to_string().substr(1, 3) << endl; // Print exponent bits (3 bits)
    cout << "Mantissa: " << bits.to_string().substr(4) << endl; // Print mantissa bits (4 bits)
    cout << "8-bit IEEE 754 Representation: " << bits << endl; // Print the entire 8-bit IEEE 754 representation
}

int float_IEEE754(float num) {
    uint8_t binaryRepresentation = 0; // Initialize an 8-bit variable to store the binary representation
    uint32_t temp = 0;

    // Copy the bytes of the float into a 32-bit integer
    memcpy(&temp, &num, sizeof(num)); // Copy raw bytes of the float into a 32-bit integer

    // Extract the sign, exponent, and mantissa with same method
    uint8_t sign = (temp >> 31) & 1; // Extract sign bit
    int8_t exponent = ((temp >> 23) & 255) - 127; // Extract and un-bias exponent from the 32-bit float
    uint8_t mantissa = (temp >> 19) & 15; // Extract the first 4 bits of the mantissa

    // Adjust for 8-bit
    exponent += 3; // Re-bias exponent to fit 3-bit format (bias of 3)
    if (exponent < 0) exponent = 0; // Clamp exponent to 0 if it's too low
    if (exponent > 7) exponent = 7; // Clamp exponent to 7 if it's too high

    // Construct 8-bit representation
    binaryRepresentation = (sign << 7) | (exponent << 4) | mantissa; // Combine into a single 8-bit binary representation
    return binaryRepresentation; // Return the final 8-bit representation as a float
}
//Memory class implementation
Memory::Memory() : memory(256, 0) {}

void Memory::set_memory_cells(int memory_address, int inside_value) {
    if (memory_address >= 0 && memory_address < 256) {
        if (inside_value >= 0 && inside_value <= 255) {
            memory[memory_address] = static_cast<uint8_t>(inside_value);
            cout << "Memory set at " << memory_address << ": " << inside_value << endl;
        } else {
            cout << "Invalid value: " << inside_value << " (must be between 0 and 255)." << endl;
        }
    } else {
        cout << "Invalid address: " << memory_address << " (must be between 0 and 255)." << endl;
    }
}

int Memory::get_memory_address(int memory_address) {
    if (memory_address >= 0 && memory_address < 256) {
        return memory[memory_address];
    } else {
        cout << "Invalid address: " << memory_address << " (must be between 0 and 255)." << endl;
        return 0;
    }
}

void Memory::display_memory_cells() {
    cout << "Displaying 8-bit memory cells:" << endl;
    for (int i = 0; i < 256; ++i) {
        cout << "M" << setw(3) << i << ": " << hex << setw(2) << setfill('0') << static_cast<int>(memory[i]) << "  |  ";
        if (i % 8 == 7) {
            cout << endl;
        }
    }
}

Memory::~Memory() {}



// Registers class implementation
Registers::Registers() : registers(16, 0) {}

int Registers::getValue(int RegIndex) {
    return registers[RegIndex];
}

void Registers::setValue(int RegIndex, int value) {
    registers[RegIndex] = value;
}

vector<int> Registers::get_registers_vector() {
    return registers;
}

void Registers::display() {
    for (int i = 0; i < 16; ++i) {
        cout << "R" << i << ": " << hex << setw(4) << setfill('0') << registers[i] << "  |  ";
        if (i % 4 == 3) {
            cout << endl;
        }
    }
}

int Registers::getRegister_0() {
    return registers[0];
}

// Load value from memory into the specified register
void Registers::loadFromMemory(Memory &memory, int memory_address, int regIndex) {
    int value = memory.get_memory_address(memory_address);
    setValue(regIndex, value);
}

// Store the value of the specified register into memory
void Registers::storeToMemory(Memory &memory, int regIndex, int memory_address) {
    int value = getValue(regIndex);
    memory.set_memory_cells(memory_address, value);
}

Registers::~Registers() {}




// ProgIns class implementation
ProgIns::ProgIns(){
    instruction = "0000";
    //cout << "Program instructions initialized." << endl;
}
char ProgIns::getOpCode() {
    return toupper(instruction[0]); // First char is opcode
}
int ProgIns::getRegister() {
    return stoi(instruction.substr(1, 1), nullptr, 16); // Second char is the register
}
int ProgIns::getAddress_Value() {
    return stoi(instruction.substr(2, 2), nullptr, 16); // Last two chars represent the address /or/ the value
}
void ProgIns::executeInstructions() {
    //cout << "Executing program instructions..." << endl;
}
ProgIns::~ProgIns() {
    //cout << "Program instructions destroyed." << endl;
}

// Simulator class implementation

Simulator::Simulator() : ProgramCounter(0) {}

void Simulator::loadProgram() {
    string ProgramName; // Variable to hold the user-provided file name
    string extension; // Variable to hold the file extension
    ifstream ProgramFile; // Create an input file stream

    while (true) {
        cout << "Enter the file name (with extension): ";
        cin >> ProgramName;
        size_t dotPos = ProgramName.find_last_of('.');
        if (dotPos != string::npos) {
            extension = ProgramName.substr(dotPos);
        } else {
            cout << "Invalid file name. Please include a file extension." << endl;
            continue;
        }

        if (extension == ".txt") {
            ProgramFile.open(ProgramName);
            if (!ProgramFile) {
                cerr << "Error: File does not exist or cannot be opened: " << ProgramName << endl;
                continue;
            }

            string line;
            while (getline(ProgramFile, line)) {
                ProgIns Ins;
                Ins.instruction = line;

                switch (Ins.getOpCode()) {
                    case '1': {
                        int R = Ins.getRegister(); // Extract the register index R
                        int XY = Ins.getAddress_Value(); // Extract the memory address XY
                        registers.setValue(R, memory.get_memory_address(XY)); // Set register R with value from memory address XY
                        break;
                    }
                    case '2': {
                        registers.setValue(Ins.getRegister(), Ins.getAddress_Value());
                        break;
                    }
                    case '3': {
                        cout << "ins 3\n";
                        break;
                    }
                    case '4': {
                        int destReg = Ins.getRegister();
                        int sourceReg = stoi(string(1, Ins.instruction[2]), nullptr, 16);
                        registers.setValue(destReg, registers.getValue(sourceReg));
                        break;
                    }
                    case '5': {
                        int regIndexResult = Ins.getRegister();
                        int regIndex1 = stoi(string(1, Ins.instruction[2]), nullptr, 16);
                        int regIndex2 = stoi(string(1, Ins.instruction[3]), nullptr, 16);
                        int value1 = registers.getValue(regIndex1);
                        int value2 = registers.getValue(regIndex2);
                        registers.setValue(regIndexResult, value1 + value2); // Simplified addition
                        break;
                    }
                    case '6': {
                        int regIndexResult = Ins.getRegister();
                        float value1 = static_cast<float>(registers.getValue(stoi(string(1, Ins.instruction[2]), nullptr, 16)));
                        float value2 = static_cast<float>(registers.getValue(stoi(string(1, Ins.instruction[3]), nullptr, 16)));
                        float difference = value1 - value2;
                        registers.setValue(regIndexResult, float_IEEE754(difference)); // Store as binary
                        break;
                    }
                    case 'B': {
                        int R = Ins.getRegister();
                        int XY = Ins.getAddress_Value();
                        memory.set_memory_cells(XY, registers.getValue(R));
                        int regIndex = Ins.getRegister();
                        int targetValue = registers.getValue(regIndex);
                        int compareValue = registers.getRegister_0();
                        int memAddr = Ins.getAddress_Value();

                        // Check if the target value equals the value in register 0
                        if (targetValue == compareValue) {
                            ProgramCounter = memAddr; // Set the program counter to the target memory address
                            cout << "Branch taken. Program counter set to address: " << memAddr << endl;
                        }
                        else {
                            cout << "No branch taken. Target value: " << targetValue << ", Compare value (R0): " << compareValue << endl;
                        }
                        break;
                    }
                    case 'C': {
                        cout << "HALT" << endl;
                        break;
                    }
                    default: {
                        cout << "Unknown operation code: " << Ins.getOpCode() << endl;
                    }
                }
            }

            registers.display();
            ProgramFile.close(); // Close the file after processing
            break; // Exit the loop after processing
        }
    }
}




void slowPrint(const string& message, int delay) {
    for (char c : message) {
        cout << c;
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << endl;
}


void start_excecution() {
    Memory memory;
    Simulator s;
    ProgIns progIns;
    Registers registers;
    s.loadProgram();
    memory.display_memory_cells();
    //    registers.display();
    progIns.executeInstructions();
}
