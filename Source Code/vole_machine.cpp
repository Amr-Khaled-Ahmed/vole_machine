#include "vole_machine.h"


// Memory class implementation
Memory::Memory() : memory(256, 0) { // make vector with 256 index and all are zeros
    cout << "Memory initialized with 256 cells." << endl;
}
void Memory::set_memory_cells(int memory_address, int inside_value) {
    if (memory_address >= 0) { // check if memory cell is greater than or equal to 0 or not
        if (memory_address < 256) { // check if memory cell is smaller than to 256 or not
            memory[memory_address] = inside_value;
            cout << "Memory set at " << memory_address << endl;
        }
    }
    else {
        cout << "invalid address: " << memory_address << endl; // invalid address
    }
}
int Memory::get_memory_address(int memory_address) {
    if (memory_address >= 0) { // check if memory cell is greater than or equal to 0 or not
        if (memory_address < 256) { // check if memory cell is smaller than to 256 or not
            return memory[memory_address];
        }
        else {
            cout << "invalid address: " << memory_address << endl; // invalid address
            return 0;
        }
    }
}
Memory::~Memory() { // Destructor to kill class
    //cout << "Memory destroyed." << endl;
}



// Registers class implementation
Registers::Registers() : registers(16, 0) {}
int Registers::getValue(int RegIndex)
{
    return registers[RegIndex];
}
void Registers::setValue(int RegIndex, int value)
{
    registers[RegIndex] = value;

}
void Registers::display() {
    for (int i = 0; i < 16; ++i) {
        cout << "R" << i << ": " << hex << setw(4) << setfill('0') << registers[i] << "  |  ";
        if (i % 4 == 3) {
            cout << endl; // Newline after 4 registers
        }
    }
}
Registers::~Registers() {
    //cout << "Registers destroyed." << endl;
}




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
        // Find the last dot in the filename to extract the extension
        size_t dotPos = ProgramName.find_last_of('.');
        if (dotPos != string::npos) {
            extension = ProgramName.substr(dotPos); // Extract the extension
        }
        else {
            cout << "Invalid file name. Please include a file extension." << endl;
            continue; // Prompt for input again if no extension is found
        }

        // Check the file extension and open the file accordingly
        if (extension == ".txt") { // Read Text File
            ProgramFile.open(ProgramName); // Open the text file
            // Check if the file opened successfully
            if (!ProgramFile) {
                cerr << "Error: File does not exist or cannot be opened: " << ProgramName << endl; // Display error message
                continue;
            }
            // Read and display each line from the text file
            string line;
            Registers regs;
            ProgIns Ins;
            while (getline(ProgramFile, line)) {
                Ins.instruction = line;
                if (Ins.getOpCode() == '1') {
                    cout << "ins 1\n";

                }
                else if (Ins.getOpCode() == '2') {
                    regs.setValue(Ins.getRegister(), Ins.getAddress_Value());
                    //done
                }
                else if (Ins.getOpCode() == '3') {
                    cout << "ins 3\n";

                }
                else if (Ins.getOpCode() == '4') {
                    // 4023 would cause the contents of register 2 to be copied into register 3.
                    regs.setValue(stoi(string(1, Ins.instruction[3]), nullptr, 16), regs.getValue(stoi(string(1, Ins.instruction[2]), nullptr, 16)) );
                    // done
                }
                else if (Ins.getOpCode() == '5') {
                    // Example: 5726 would cause the binary values in registers 2 and 6 to be added and the sum placed in register 7.
                    float value1 = static_cast<float>(registers.getValue(stoi(string(1, Ins.instruction[2]), nullptr, 16)));
                    float value2 = static_cast<float>(registers.getValue(stoi(string(1, Ins.instruction[3]), nullptr, 16)));
                    float sum = value1 + value2; // Adding the two floating-point values
                    registers.setValue(stoi(string(1, Ins.instruction[1]), nullptr, 16), *reinterpret_cast<int*>(&sum)); // Store the float as binary in the register
                }
                else if (Ins.getOpCode() == '6') {
                    // 6726 would cause the binary values in registers 2 and 6 to be subtracted and the result placed in register 7, IEEE 754 floating-point representation.
                    float value1 = static_cast<float>(registers.getValue(stoi(string(1, Ins.instruction[2]), nullptr, 16)));
                    float value2 = static_cast<float>(registers.getValue(stoi(string(1, Ins.instruction[3]), nullptr, 16)));
                    float difference = value1 - value2; // Subtracting the two floating-point values
                    registers.setValue(stoi(string(1, Ins.instruction[1]), nullptr, 16), *reinterpret_cast<int*>(&difference)); // Store the float as binary in the register
                }
                else if (Ins.getOpCode() == 'B') {
                    cout << "ins 7\n";

                }
                else if (Ins.getOpCode() == 'C') {
                    cout << "ins 8\n";

                }
            }
            regs.display();
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




    memory.set_memory_cells(10, 42);
    int memory_value = memory.get_memory_address(300);
    s.loadProgram();
//    registers.display();
    progIns.executeInstructions();
}

