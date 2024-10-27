#include "vole_machine.h"
// Function that convert float number that can be positive or negative to IEEE754 representation
int float_IEEE754(float num) {
    uint8_t binaryRepresentation = 0; // Initialize an 8-bit variable to store the binary representation
    uint32_t temp = 0;

    // Copy the bytes of the float into a 32-bit integer
    memcpy(&temp, &num, sizeof(num)); // Copy raw bytes of the float into a 32-bit integer

    // Extract then sign, exponent, and mantissa with same method
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
// Memory class implementation
Memory::Memory() : memory(256, 0) {}


void Memory::set_memory_cells(int memory_address, int value) {
    //int value = Get_Register_Value(regIndex); // Use inherited method from Registers
    memory[memory_address] = (value % 100);
}
int Memory::get_memory_address(int memory_address) {
    if (memory_address >= 0 && memory_address < 256) {
        return memory[memory_address];
    }
}
void Memory::display_memory_cells() {
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

int Registers::Get_Register_Value(int RegIndex)
{
    return registers[RegIndex];
}
void Registers::Set_Register_Value(int RegIndex, int value)
{
    registers[RegIndex] = (value % 100);
}



void Registers::Display_Registers() {
    for (int i = 0; i < 16; ++i) {
        cout << "R" << i << ": " << hex << setw(2) << setfill('0') << registers[i] << "  |  ";
        if (i % 4 == 3) {
            cout << endl;
        }
    }
}

Registers::~Registers() {}




// ProgIns class implementation
ProgIns::ProgIns(){
    instruction = "0000";
    //cout << "Program instructions initialized." << endl;
}
char ProgIns::getOpCode_from_ins() {
    return toupper(instruction[0]); // First char is opcode
}
int ProgIns::getRegister_from_ins() {
    return stoi(instruction.substr(1, 1), nullptr, 16); // Second char is the register
}
int ProgIns::getAddress_Value_from_ins() {
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
            ProgIns Ins;
            while (getline(ProgramFile, line)){
                Ins.instruction = line;
                if (Ins.getOpCode_from_ins() == '1') {
                    int R = Ins.getRegister_from_ins(); // Extract the register index R
                    int XY = Ins.getAddress_Value_from_ins(); // Extract the memory address XY
                    int valueFromMemory = memory.get_memory_address(XY);
                    registers.Set_Register_Value(R, valueFromMemory); // Set register R with value from memory address XY
                    memory.set_memory_cells(XY, registers.Get_Register_Value(XY));
                }
                else if (Ins.getOpCode_from_ins() == '2') {
                    int value = Ins.getAddress_Value_from_ins();
                    int regIndex = Ins.getRegister_from_ins();
                    registers.Set_Register_Value(regIndex, value);
                    memory.set_memory_cells(regIndex, registers.Get_Register_Value(value));
                }
                else if (Ins.getOpCode_from_ins() == '3') {
                    cout << "ins 3\n";
                    // Optionally save something in memory if needed
                }
                else if (Ins.getOpCode_from_ins() == '4') {
                    int destReg = Ins.getRegister_from_ins();
                    int sourceReg = stoi(string(1, Ins.instruction[2]), nullptr, 16);
                    registers.Set_Register_Value(destReg, registers.Get_Register_Value(sourceReg));
                    memory.set_memory_cells(destReg, registers.Get_Register_Value(destReg));
                }
                else if (Ins.getOpCode_from_ins() == '5') {
                    int regIndexResult = Ins.getRegister_from_ins();
                    int regIndex1 = stoi(string(1, Ins.instruction[2]), nullptr, 16);
                    int regIndex2 = stoi(string(1, Ins.instruction[3]), nullptr, 16);

                    // Retrieve the values as signed integers
                    int value1 = registers.Get_Register_Value(regIndex1);
                    int value2 = registers.Get_Register_Value(regIndex2);

                    // Perform addition (this inherently uses two's complement)
                    int sum = value1 + value2;

                    // Check for overflow (optional, depending on your architecture)
                    if (((value1 > 0) && (value2 > 0) && (sum < 0)) || // Positive overflow
                        ((value1 < 0) && (value2 < 0) && (sum > 0))) { // Negative overflow
                        cout << "Overflow occurred during addition!" << endl;
                    }

                    // Store the result in the register
                    registers.Set_Register_Value(regIndexResult, sum);

                    // Save the result in memory (assuming regIndexResult is a valid address)
                    memory.set_memory_cells(regIndexResult, registers.Get_Register_Value(regIndexResult));
                }
                else if (Ins.getOpCode_from_ins() == '6') {
                    int regIndexResult = Ins.getRegister_from_ins();
                    float value1 = static_cast<float>(registers.Get_Register_Value(
                            stoi(string(1, Ins.instruction[2]), nullptr, 16)));
                    float value2 = static_cast<float>(registers.Get_Register_Value(
                            stoi(string(1, Ins.instruction[3]), nullptr, 16)));
                    float difference = value1 - value2;
                    registers.Set_Register_Value(regIndexResult, float_IEEE754(difference)); // Store as binary
                    //memory.set_memory_cells(regIndexResult, float_IEEE754(difference)); // Save the result in memory
                }
                else if(Ins.getOpCode_from_ins() == '7'){

                }
                else if(Ins.getOpCode_from_ins() == '8'){

                }
                else if(Ins.getOpCode_from_ins() == '9'){

                }
                else if(Ins.getOpCode_from_ins() == 'A'){

                }
                else if (Ins.getOpCode_from_ins() == 'B') {
                    int R = Ins.getRegister_from_ins();
                    int XY = Ins.getAddress_Value_from_ins();
                    memory.set_memory_cells(XY, registers.Get_Register_Value(XY));
                    int targetValue = registers.Get_Register_Value(R);
                    int compareValue = registers.Get_Register_Value(0);
                    int memAddr = Ins.getAddress_Value_from_ins();

                    // Check if the target value equals the value in register 0
                    if (targetValue == compareValue) {
                        ProgramCounter = memAddr; // Set the program counter to the target memory address
                    }
                }
                else if (Ins.getOpCode_from_ins() == 'C') {
                    cout << "HALT" << endl;
                    break;
                }
                else {
                    cout << "Unknown operation code: " << Ins.getOpCode_from_ins() << endl;
                }
            }
            registers.Display_Registers();
            cout<<"--------------------------------"<<endl;
            cout << registers.Get_Register_Value(1) << endl;
            memory.set_memory_cells(0, registers.Get_Register_Value(1));
            memory.display_memory_cells();
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
void start_execution() {
    Memory memory;
    Simulator s;
    ProgIns progIns;
    Registers registers;
    s.loadProgram();
    //    registers.Display_Registers();
    progIns.executeInstructions();
}