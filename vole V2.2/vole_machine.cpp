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
    memory[memory_address] = value & 0xff;
}
int Memory::get_memory_address(int memory_address) {
    if (memory_address >= 0 && memory_address < 256) {
        return memory[memory_address];
    }
}
void Memory::display_memory_cells() {
    for (int i = 0; i < 256; ++i) {
        cout << "M" << setw(2) << i << ": " << hex << setw(2) << setfill('0') << static_cast<int>(memory[i]) << "  |  ";
        if (i % 8 == 7) {
            cout << endl;
        }
    }
}

Memory::~Memory() {} // Destructor class

// Registers class implementation
Registers::Registers() : registers(16, 0) {}

int Registers::Get_Register_Value(int RegIndex)
{
    return registers[RegIndex];
}
void Registers::setValue_to_register(int RegIndex, int value)
{
    registers[RegIndex] = (value & 0xff);
}



void Registers::display_Registers() {
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
            cout<<endl;
            cout<<"--------------------------------"<<endl;
            cout<<"Operation which as made by ALU"<<endl;
            while (getline(ProgramFile, line)){
                Ins.instruction = line;
                if (Ins.getOpCode_from_ins() == '1') { // LOAD the register R with the bit pattern found in the memory cell whose address is XY.
                    int R = Ins.getRegister_from_ins(); // Extract the register index R
                    int XY = Ins.getAddress_Value_from_ins(); // Extract the memory address XY
                    int valueFromMemory = memory.get_memory_address(XY); // get value from memory
                    registers.setValue_to_register(R, valueFromMemory);
                    cout << "Loaded value " << hex << valueFromMemory << " from memory address " << hex << XY << " into register R" << R << endl;
                }
                else if (Ins.getOpCode_from_ins() == '2') { //LOAD the register R with the bit pattern XY.
                    int R = Ins.getRegister_from_ins(); // Extract the register index R
                    int XY = Ins.getAddress_Value_from_ins(); // Extract the immediate value XY
                    registers.setValue_to_register(R, XY); // load register with value of XY
                    cout << "Loaded immediate value " << hex << XY << " into register R" << R << endl;
                }
                else if (Ins.getOpCode_from_ins() == '3') { //STORE the bit pattern found in register R in the memory cell whose address is XY.
                    int R = Ins.getRegister_from_ins(); // Extract the register index R
                    int XY = Ins.getAddress_Value_from_ins(); // Extract the memory address XY
                    int valueToStore = registers.Get_Register_Value(R);
                    // Check if the address is 00 (special case for screen)
                    if (XY == 0) {
                        cout << "Writing value " << hex << valueToStore << " to screen." << endl;
                    }
                    else {
                        // Store the value in the specified memory address
                        memory.set_memory_cells(XY, valueToStore);
                        cout << "Stored value " << hex << valueToStore << " from register R" << R << " to memory address " << hex << XY << endl;
                    }
                }
                else if (Ins.getOpCode_from_ins() == '4') { //MOVE the bit pattern found in register R to register S.
                    registers.setValue_to_register(stoi(string(1, Ins.instruction[3]), nullptr, 16),
                                                   registers.Get_Register_Value(
                                                           stoi(string(1, Ins.instruction[2]), nullptr, 16)));
                }
                else if (Ins.getOpCode_from_ins() == '5') {
                    /*ADD the bit patterns in registers S and T as though they were two’s complement representations
                      and leave the result in register R.
                    */
                    int sourceReg1 = stoi(string(1, Ins.instruction[2]), nullptr, 16);
                    int sourceReg2 = stoi(string(1, Ins.instruction[3]), nullptr, 16);
                    int destReg = stoi(string(1, Ins.instruction[1]), nullptr, 16);
                    cout << "Adding values from Register R" << sourceReg1 << " and Register R" << sourceReg2 << endl;
                    int value1 = registers.Get_Register_Value(sourceReg1);
                    int value2 = registers.Get_Register_Value(sourceReg2);
                    int result = value1 + value2;
                    if ((value1 > 0 && value2 > 0 && result < 0) || (value1 < 0 && value2 < 0 && result > 0)) {
                        cout << "Overflow occurred during addition!" << endl;
                    }
                    registers.setValue_to_register(destReg, result);
                    cout << "Result of addition: " << result << " stored in Register R" << destReg << endl;
                }
                else if (Ins.getOpCode_from_ins() == '6') {
                    /*ADD the bit patterns in registers S and T as though they represented values in floating-point
                      notation and leave the floating-point result in register R.
                    */
                    int regIndexResult = Ins.getRegister_from_ins();
                    float value1 = static_cast<float>(registers.Get_Register_Value(
                            stoi(string(1, Ins.instruction[2]), nullptr, 16)));
                    float value2 = static_cast<float>(registers.Get_Register_Value(
                            stoi(string(1, Ins.instruction[3]), nullptr, 16)));
                    float sum_floating = value1 + value2;
                    registers.setValue_to_register(regIndexResult, float_IEEE754(sum_floating)); // Store as binary
                }
                else if (Ins.getOpCode_from_ins() == '7') {

                }
                else if (Ins.getOpCode_from_ins() == '8') {

                }
                else if (Ins.getOpCode_from_ins() == '9') {

                }
                else if (Ins.getOpCode_from_ins() == 'A') {

                }
                else if (Ins.getOpCode_from_ins() == 'B') {
                    /**
                    JUMP to the instruction located in the memory cell at address XY if the bit pattern in register R
                    is equal to the bit pattern in register number 0. Otherwise, continue with the normal sequence of
                    execution. (The jump is implemented by copying XY into the program counter during the execute phase.)
                    **/
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
                    cout<<"Program counter now is: "<<hex<<ProgramCounter<<endl;
                }
                else if (Ins.getOpCode_from_ins() == 'C') { // HALT program
                    cout << "HALT" << endl;
                    break;
                }
                else{
                    cout << "Unknown operation code: " << Ins.getOpCode_from_ins() << endl;
                }
            }
            cout<<"--------------------------------"<<endl;
            cout<<"Registers"<<endl;
            registers.display_Registers();
            cout<<"--------------------------------"<<endl;
            cout<<"Memory cells"<<endl;
            memory.display_memory_cells();
            ProgramFile.close();
            break;
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
    Simulator s;
    s.loadProgram();
}
