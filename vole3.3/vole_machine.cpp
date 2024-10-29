#include "vole_machine.h"
// Function to decode the custom hex format to decimal
double decode_to_decimal(uint8_t hexValue) {
    bool sign = (hexValue & 0x80) >> 7; // to get first index which is 1 "negative", 0 "positive"
    int exponent = ((hexValue & 0x70) >> 4) - 4; // Exponent with bias of 4 removed
    int mantissa = hexValue & 0x0F;
    double mantissaValue = mantissa / 16.0;
    double decimalValue = pow(2, exponent) * mantissaValue;
    if(sign==true) {
        return -decimalValue;
    }
    else {
        return decimalValue;
    }
}
// Function to encode a decimal number to the custom hex format
uint8_t encode_from_decimal(double value) {
    bool sign = value < 0;
    value = abs(value); // if value negative make it positive
    int exponent = 0;
    // Adjust value to fit within the range for mantissa encoding
    while (value < 1 && exponent > -4) {
        value *= 2;
        exponent--;
    }
    exponent += 4; // Bias back to the range used in encoding
    value *= 16;
    int temp = trunc(value);
    int mantissa = temp & 0x0F; // Keep 4 bits for mantissa
    return (sign << 7) | (exponent << 4) | mantissa;
}
// Function to add two values in the custom format and return the result in hex format
uint8_t addCustomFormat(uint8_t hex1, uint8_t hex2) {
    return decode_to_decimal(hex1)+decode_to_decimal(hex2);
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
        cout << "Enter the file name (with extension \".txt\") : ";
        cin >> ProgramName;
        size_t dotPos = ProgramName.find_last_of('.');
        if (dotPos != string::npos) {
            extension = ProgramName.substr(dotPos);

        } else {
            cerr << "Invalid file name. Please include a file extension." << endl;
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

            cout << endl << "Operation which as made by ALU" <<endl;
            cout << endl << setw(50) << setfill('-') << " " << endl;


            while (getline(ProgramFile, line)){
                if (line.size() != 4 || !isxdigit(line[0]) || !isxdigit(line[1]) ||
                    !isxdigit(line[2]) || !isxdigit(line[3])) {
                    cout << "Unknown operation code: " << line << endl;
                    continue;
                }
                memory.set_memory_cells(ProgramCounter, stoi(line, nullptr, 16));
                ProgramCounter+=1;
                Ins.instruction = line;
                if (Ins.getOpCode_from_ins() == '1') { // LOAD the register R with the bit pattern found in the memory cell whose address is XY.
                    int R = Ins.getRegister_from_ins(); // Extract the register index R
                    int XY = Ins.getAddress_Value_from_ins(); // Extract the memory address XY
                    int valueFromMemory = memory.get_memory_address(XY); // get value from memory
                    registers.setValue_to_register(R, valueFromMemory);
                    cout << "Ins: 1, Loaded value " << hex << valueFromMemory << " from memory address " << hex << XY << " into register R" << R << endl;
                }
                else if (Ins.getOpCode_from_ins() == '2') { //LOAD the register R with the bit pattern XY.
                    int R = Ins.getRegister_from_ins(); // Extract the register index R
                    int XY = Ins.getAddress_Value_from_ins(); // Extract the immediate value XY
                    registers.setValue_to_register(R, XY); // load register with value of XY
                    cout << "Ins: 2, Loaded immediate value " << hex << XY << " into register R" << R << endl;
                }
                else if (Ins.getOpCode_from_ins() == '3') { //STORE the bit pattern found in register R in the memory cell whose address is XY.
                    int R = Ins.getRegister_from_ins(); // Extract the register index R
                    int XY = Ins.getAddress_Value_from_ins(); // Extract the memory address XY
                    int valueToStore = registers.Get_Register_Value(R);
                    // Check if the address is 00 (special case for screen)
                    if (XY == 0) {
                        cout << "Ins: 3, Writing value " << hex << valueToStore << " to screen." << endl;
                    }
                    else {
                        // Store the value in the specified memory address
                        memory.set_memory_cells(XY, valueToStore);
                        cout << "Ins: 3, Stored value " << hex << valueToStore << " from register R" << R << " to memory address " << hex << XY << endl;
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
                    cout << "Ins: 5, Adding values from Register R" << sourceReg1 << " and Register R" << sourceReg2 << endl;
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
                    /*
                     ADD the bit patterns in registers S and T as though they represented values in floating-point
                      notation and leave the floating-point result in register R.
                    */
                    int regIndexResult = Ins.getRegister_from_ins();
                    // Get the register indexes for the source registers
                    int regIndexS = stoi(string(1, Ins.instruction[2]), nullptr, 16);
                    int regIndexT = stoi(string(1, Ins.instruction[3]), nullptr, 16);
                    cout << "Ins: 6, Adding values from Register R" << regIndexS << " and Register R" << regIndexT << endl;
                    // Get valued from registers S&T
                    int value1 = registers.Get_Register_Value(regIndexS);
                    int value2 = registers.Get_Register_Value(regIndexT);
                    double temp1 = decode_to_decimal(value1);
                    double temp2 = decode_to_decimal(value2);
                    double resultDecimal = temp1 + temp2;
                    int result = encode_from_decimal(resultDecimal);
                    cout << "Result of addition : " << hex << result <<endl;
                    // Store the result in the target register
                    registers.setValue_to_register(regIndexResult, result);
                }
                else if (Ins.getOpCode_from_ins() == '7') {
                    /*
                     * in OR the bit patterns
                     * in registers S and T
                     * and Put the result in register R
                     */
                    int regIndexResult = Ins.getRegister_from_ins();
                    int rigster1 = registers.Get_Register_Value(stoi(string(1, Ins.instruction[2]), nullptr, 16));
                    int rigster2 = registers.Get_Register_Value(stoi(string(1, Ins.instruction[3]), nullptr, 16));
                    cout << "OR to values from Register R" << string(1, Ins.instruction[2]) << " with value("<< rigster1 <<')' << " and Register R" << string(1, Ins.instruction[3]) << " with value("<< rigster2 <<")\n";
                    cout << "the Result will be : " << (rigster1 | rigster2) << '\n';
                    registers.setValue_to_register(regIndexResult,(rigster1 | rigster2));
                    cout << "Ins: 7, Result of AND operation: " << rigster1 << " OR " << rigster2 << " = " << (rigster1 | rigster2) << endl;
                }
                else if (Ins.getOpCode_from_ins() == '8') {
                    /*
                     * in AND the bit patterns
                     * in registers S and T
                     * and Put the result in register R
                     */
                    int regIndexResult = Ins.getRegister_from_ins();
                    int rigster1 = registers.Get_Register_Value(stoi(string(1, Ins.instruction[2]), nullptr, 16));
                    int rigster2 = registers.Get_Register_Value(stoi(string(1, Ins.instruction[3]), nullptr, 16));
                    cout << "AND to values from Register R" << string(1, Ins.instruction[2]) << " with value("<< rigster1 <<')' << " and Register R" << string(1, Ins.instruction[3]) << " with value("<< rigster2 <<")\n";
                    cout << "the Result will be : " << (rigster1 & rigster2) << '\n';
                    registers.setValue_to_register(regIndexResult,(rigster1 & rigster2));
                    cout << "Ins: 8, Result of AND operation: " << rigster1 << " AND " << rigster2 << " = " << (rigster1 & rigster2) << endl;
                }
                else if (Ins.getOpCode_from_ins() == '9') {
                    /*
                     * in Exclusive OR the bit patterns
                     * in registers S and T
                     * and Put the result in register R
                     */
                    int regIndexResult = Ins.getRegister_from_ins();
                    int rigster1 = registers.Get_Register_Value(stoi(string(1, Ins.instruction[2]), nullptr, 16));
                    int rigster2 = registers.Get_Register_Value(stoi(string(1, Ins.instruction[3]), nullptr, 16));
                    cout << "XOR to values from Register R" << string(1, Ins.instruction[2]) << " with value("<< rigster1 <<')' << " and Register R" << string(1, Ins.instruction[3]) << " with value("<< rigster2 <<")\n";
                    cout << "the Result will be : " << (rigster1 ^ rigster2) << '\n';
                    registers.setValue_to_register(regIndexResult,(rigster1 ^ rigster2));
                    cout << "Ins: 9, Result of XOR operation: " << rigster1 << " XOR " << rigster2 << " = " << (rigster1 ^ rigster2) << endl;

                }
                else if (Ins.getOpCode_from_ins() == 'A') {
                    /*
                     * in ROTATE the bit patterns in register R one bit to the Right
                     * X times Each time PLace the bit that started at the low-order
                     * end at the High-order end
                     */
                    int Place = Ins.getRegister_from_ins();
                    unsigned int RegisterV = registers.Get_Register_Value(stoi(string(1, Ins.instruction[1]), nullptr, 16));
                    int Xtimes = stoi(Ins.instruction.substr(2, 2), nullptr, 16);
                    Xtimes %= 8 ; // to prevent it getting out of the bits No
                    cout << "Rotate Right for Register R" << string(1, Ins.instruction[1]) << " with value("<< RegisterV <<")\n";
                    cout << "the Result will be : " <<  ((RegisterV >> Xtimes) | (RegisterV << (8 - Xtimes))) << '\n';
                    registers.setValue_to_register(Place,((RegisterV >> Xtimes) | (RegisterV << (8 - Xtimes))));
                    cout << "Ins: A, Result of rotate operation: " << RegisterV << " shifted " << Xtimes << " places to the right = " << ((RegisterV >> Xtimes) | (RegisterV << (8 - Xtimes))) << endl;
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
                    cout << "Ins: B, Program counter now is: " << hex << ProgramCounter << endl;
                }
                else if (Ins.getOpCode_from_ins() == 'C') { // HALT program
                    line[0] = toupper(line[0]);
                    if (line == "C000"){
                        cout << "Ins: C, HALT" << endl;
                        break;
                    }else{
                        cout << "Unknown operation code: " << line << endl;
                    }
                }
                else{
                    cout << "Unknown operation code: " << Ins.getOpCode_from_ins() << endl;
                }
            }
            string choice;
            cout << setw(50) << setfill('-') << " " << endl;
            cout << "Registers" << endl;
            cout << setw(43) << setfill('-') << " " << endl;
            registers.display_Registers();  // Assuming this displays register info
            cout << setw(43) << setfill('-') << " " << endl;
            cout << "Memory cells" << endl;
            cout << setw(95) << setfill('-') << " " << endl;
            memory.display_memory_cells();
            cout << setw(95) << setfill('-') << " " << endl;
            cout << endl << endl;
            do {
                cout << "1. Show the memory and register information";
                cout << "\n2. Choose another program";
                cout << "\nEnter your choice: ";
                cin >> choice;
                if (choice == "1") {
                    cout << setw(43) << setfill('-') << " " << endl;
                    cout << "Registers" << endl;
                    cout << setw(43) << setfill('-') << " " << endl;
                    registers.display_Registers();  // Assuming this displays register info
                    cout << setw(43) << setfill('-') << " " << endl;
                    cout << "Memory cells" << endl;
                    cout << setw(95) << setfill('-') << " " << endl;
                    memory.display_memory_cells();  // Assuming this displays memory cells
                    cout << setw(95) << setfill('-') << " " << endl;
                } else if (choice == "2") {
                    cout << "Exiting Current program." << endl;
                    ProgramFile.close();  // Close the program file once the loop exits
                    break;
                }else{
                    cerr << "Invalid choice. Please enter 1 or 2." << endl;
                }
            } while (choice != "2");
            ProgramFile.close();  // Close the program file once the loop exits
            break;

        }else{
            cerr << "Error: File does not exist or is not a text file: " << ProgramName << endl;
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
