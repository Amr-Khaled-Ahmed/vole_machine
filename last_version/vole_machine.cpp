#include "vole_machine.h"

// Function to convert a single hex digit to an unsigned char
unsigned char hexa_char(char hex) {
    int dec = static_cast<unsigned char>(hex);
    if (dec >= 48 && dec <= 57) {
        char chr = static_cast<char>(dec - 48);
        return chr;
    }
    if (dec >= 65 && hex <= 70) {
        char chr = static_cast<char>(dec-65+10);
        return chr;
    }
    if (dec >= 97 && hex <= 102) {
        char chr = static_cast<char>(dec-97+10);
        return chr;
    }
    else {
        return 1; // Error case
    }
}

// Function to convert a hex string to an unsigned char
unsigned char hexa_unsigned_chars(const string &hex) {
        unsigned char high = hexa_char(hex[0]);
        unsigned char low = hexa_char(hex[1]);
        return (high << 4) | low;
}

// Function to convert an unsigned char to a hex string
string char_to_hex(unsigned char value) {
    char temporary[3];
    string result;
    char hex_characters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for(int index = 0;index<3;index++) {
        if(index==0) {
            temporary[index] = hex_characters[(value >> 4) & 15];
            result += temporary[index];
        }
        else if(index==1) {
            temporary[index] = hex_characters[value & 15];
            result += temporary[index];
        }
        else if(index==2) {
            temporary[index] = '\0';
            result += temporary[index];
        }
    }
    return result;
}

// Decode from custom format to decimal
double decode_to_decimal(uint8_t hexValue) {
    bool sign = (hexValue & 128) >> 7; // 1 if negative, 0 if positive
    int exponent = ((hexValue & 112) >> 4) - 4; // Exponent with bias of 4 removed
    int mantissa = hexValue & 15; // Mantissa is the last 4 bits
    double mantissaValue = mantissa / 16.0; // Convert to fractional part
    double decimalValue = pow(2, exponent) * mantissaValue;
    if(sign>0) {
        decimalValue = -decimalValue;
    }
    return decimalValue;
}

    // Encode from decimal to custom format
uint8_t encode_from_decimal(double value) {
    bool sign = value < 0;
    value = abs(value);
    int exponent = 0;

    // Normalize the value to get the exponent
    while (value >= 2.0 && exponent < 3) {
        value /= 2.0;
        exponent++;
    }
    while (value < 1.0 && exponent > -4.0) {
        value *= 2.0;
        exponent-=1;
    }
    exponent += 4; // Apply bias to exponent

    // Scale value to get the mantissa
    int mantissa = static_cast<int>(round(value * 16.0)) & 15; // Round for proper mantissa

    // Cap mantissa to avoid overflow
    if (mantissa > 15) {
        mantissa = 15;
    }
    return (sign << 7) | (exponent << 4) | mantissa;
}

// Normalize the result
unsigned char Normalize(unsigned char S, unsigned char E, unsigned char M) {
    // Normalize mantissa
    while (M >= 16) { // Mantissa exceeds 4 bits (1.xxx)
        M >>= 1;        // Shift mantissa right
        E+=1;            // Increment exponent
    }
    while (M < 8 && E > 0) { // Mantissa less than 1.0 (0.xxx) but E > 0
        M <<= 1;               // Shift mantissa left
        E-=1;                   // Decrement exponent
    }
    return (S << 7) | (E << 4) | (M & 15);
}

// Function to add two 8-bit floating-point numbers in custom format
string Add_two_floating_numbers(const string &hex1, const string &hex2) {
    // Convert hex strings to unsigned char (8 bits)
    unsigned char num1 = hexa_unsigned_chars(hex1);
    unsigned char num2 = hexa_unsigned_chars(hex2);

    // Extract sign, exponent, and mantissa for num1
    unsigned char sin_1 = (num1 >> 7) & 1;
    unsigned char exp_1 = (num1 >> 4) & 7;
    unsigned char mant_1 = num1 & 15;

    // Extract sign, exponent, and mantissa for num2
    unsigned char sin_2 = (num2 >> 7) & 1;
    unsigned char exp_2 = (num2 >> 4) & 7;
    unsigned char mant_2 = num2 & 15;

    // Normalize the exponents
    while(true) {
        if (exp_2 > exp_1) {
            swap(sin_1, sin_2);
            swap(exp_1, exp_2);
            swap(mant_1, mant_2);
            break;
        }
        else {
            break;
        }
    }
    // Adjust the mantissa of the smaller exponent
    unsigned char shift = exp_1 - exp_2;
    mant_2 >>= shift;

    // Perform the addition or subtraction based on the sign
    unsigned char sum_mantesa;
    if (sin_1 == sin_2) {
        sum_mantesa = mant_1 + mant_2; // Same sign, add mantissas
    }
    else {
        if (mant_1 >= mant_2) {
            sum_mantesa = mant_1 - mant_2; // Different signs, subtract mantissas
        }
        else {
            sum_mantesa = mant_2 - mant_1;
            if(mant_1 < mant_2) {
                sin_1=sin_2;
            }
        }
    }
    // Normalize the result
    unsigned char result = Normalize(sin_1, exp_1, sum_mantesa);
    // Convert result to hex string
    return char_to_hex(result);
}
// these function help in case 5
int BintoDec(string bin)
{
    int dec = 0; // result
    int len = bin.length(); // size of bits
    for (int i = 0; i < len; i++) // loop only on 1s and transport it to Decimal NO.
    {
        if (bin[len - 1 - i] == '1')
        {
            dec += pow(2, i);
        }
    }

    return dec;
}

string BinaryConv(int number)
{
    string bin = ""; // result
    for (int i = 0; i < 8; i++) // store the digits of the Binary in string
    {
        bin = ((number & 1) ? '1' : '0') + bin;
        number >>= 1;
    }
    return bin;
}

string TwosComplementConv(int number)
{
    if (number >= 0) // determine if +Ve NO convert to binary direct
    {
        return BinaryConv(number);
    }
    else // if -Ve NO convert the +Ve NO of it then deal with +Ve version
    {
        string bin = BinaryConv(-number);
        for (char &bit : bin) // convert all 0s to 1s and the opposite
        {
            bit = (bit == '0') ? '1' : '0';
        }

        int cr = 1; // the one will be added to convert the No to 2`s complement
        for (int i = bin.size() - 1; i >= 0; i--) // loop from first digit to last one to add one
        {
            if (bin[i] == '1' && cr == 1)
            {
                bin[i] = '0';
            }
            else if (cr == 1)
            {
                bin[i] = '1';
                cr = 0;
            }
        }

        return bin;
    }
}

string AddBinary( string &bin1, string &bin2)
{
    // take the 2 string NOs and boolean variable to check if there is overflow
    string result = "";
    int carry = 0; // help if was carry one

    for (int i = 7; i >= 0; i--) // loop from the first digit to last
    {
        //convert the digits of the two NO
        int bit1 = bin1[i] - '0';
        int bit2 = bin2[i] - '0';
        // add them with carry
        int sum = bit1 + bit2 + carry;
        // add the digit of the sum and set if there was carry
        result = char((sum % 2) + '0') + result;
        carry = sum / 2;
    }
    return result;
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

void Simulator::loadProgram_exe_all() {
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
                    string value1 = TwosComplementConv(registers.Get_Register_Value(sourceReg1));
                    string value2 = TwosComplementConv(registers.Get_Register_Value(sourceReg2));
                    string restr = AddBinary(value1 , value2);
                    int result = BintoDec(restr);
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
                    int regIndexS = hexa_char(Ins.instruction[2]);
                    int regIndexT = hexa_char(Ins.instruction[3]);

                    cout << "Ins: 6, Adding values from Register R" << (int)regIndexS << " and Register R" << (int)regIndexT << endl;

                    // Get values from registers S&T
                    string hexValue1 = char_to_hex(registers.Get_Register_Value(regIndexS));
                    string hexValue2 = char_to_hex(registers.Get_Register_Value(regIndexT));

                    // Add the two floating-point values
                    string resultHex = Add_two_floating_numbers(hexValue1, hexValue2);

                    cout << "Result of addition: " << resultHex << endl;

                    // Store the result in the target register
                    uint8_t result = hexa_unsigned_chars(resultHex);
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


void Simulator::loadProgram_step_by_step(){
    string ProgramName; // Variable to hold the user-provided file name
    string extension; // Variable to hold the file extension
    ifstream ProgramFile; // Create an input file stream
    while (true) {
        ProgramFile.close(); // Close the file if it's open before starting a new loop'
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
                if (line.size() == 0) {continue;}
                if (line.size() != 4 || !isxdigit(line[0]) || !isxdigit(line[1]) ||
                    !isxdigit(line[2]) || !isxdigit(line[3])) {
                    cout << "Unknown operation code: " << line << endl;
                    continue;
                }
                string CCC;
                do {
                    cout << "Do you want to execute this (Y/N) : " << line << endl;
                    cin >> CCC;
                    CCC[0] = toupper(CCC[0]);
                    if (CCC!= "Y" && CCC!= "N") {
                        cerr << "Invalid choice. Please enter Y or N." << endl;
                        continue;
                    }
                    if (CCC == "N") {
                        break;
                    }
                }while(CCC != "Y");
                if (CCC == "N") {
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
                    int regIndexS = hexa_char(Ins.instruction[2]);
                    int regIndexT = hexa_char(Ins.instruction[3]);

                    cout << "Ins: 6, Adding values from Register R" << (int)regIndexS << " and Register R" << (int)regIndexT << endl;

                    // Get values from registers S&T
                    string hexValue1 = char_to_hex(registers.Get_Register_Value(regIndexS));
                    string hexValue2 = char_to_hex(registers.Get_Register_Value(regIndexT));

                    // Add the two floating-point values
                    string resultHex = Add_two_floating_numbers(hexValue1, hexValue2);

                    cout << "Result of addition: " << resultHex << endl;

                    // Store the result in the target register
                    uint8_t result = hexa_unsigned_chars(resultHex);
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
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                cout << "Current machine code is"<< line << endl;
                string choice;
                cout << setw(50) << setfill('-') << " " << endl;
                cout << "Registers" << endl;
                cout << setw(43) << setfill('-') << " " << endl;
                registers.display_Registers();  // Assuming this displays register info
                cout << setw(43) << setfill('-') << " " << endl;
                do {
                    cout << "1. Show the memory and register information"
                     << "\n2. continue Current Program"
                     << "\n3. Choose another program"
                     << "\nEnter your choice: ";
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
                        continue;

                    }else if (choice == "3"){
                        cout << "Exiting Current program." << endl;
                        ProgramFile.close();  // Close the program file once the loop exits
                        break;
                    }
                    else{
                        cerr << "Invalid choice. Please enter 1 or 2." << endl;
                    }
                } while (choice != "2");

            }

            cout << "End of the Program" << endl;
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