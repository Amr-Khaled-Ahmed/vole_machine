#ifndef A1_T4_S5_S19_20231134_20231116_20231042_H
#define A1_T4_S5_S19_20231134_20231116_20231042_H

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <string>
#include <cctype>
#include <sstream>
#include <fstream>
#include <bitset>
#include<cmath>
using namespace std;
// Forward declaration of the Registers class
class Registers;
// Function to convert a single hex digit to an unsigned char
unsigned char hexa_char(char hex);
// Function to convert a hex string to an unsigned char
unsigned char hexa_unsigned_chars(const string &hex);
// Function to convert an unsigned char to a hex string
string char_to_hex(unsigned char value);
// Decode from custom format to decimal
double decode_to_decimal(uint8_t hexValue);
// Encode from decimal to custom format
uint8_t encode_from_decimal(double value);
// Normalize the result
unsigned char Normalize(unsigned char S, unsigned char E, unsigned char M);
// Function to add two 8-bit floating-point numbers in custom format
string Add_two_floating_numbers(const string &hex1, const string &hex2);

// Convert Decimal declaration
int BintoDec(string bin);
// Converter Binary declaration
string BinaryConv(int number);
// Convert the +ve or -ve # to Binary declaration
string TwosComplementConv(int number);
// Adder for 2`s Complement NO declaration
string AddBinary(const string &bin1, const string &bin2);

// Registers class
class Registers {
private:
    vector<int> registers; // Store register values
public:
    Registers();
    int Get_Register_Value(int regIndex); // Get value from a register
    void setValue_to_register(int regIndex, int value); // Set value in a register
    void display_Registers(); // Display register contents
    void clear_register();
    ~Registers();
};

// Memory class inheriting from Registers
class Memory {
private:
    vector<uint8_t> memory; // Store memory as 8-bit cells
public:
    Memory();
    void set_memory_cells(int memory_address, int regIndex); // Sets memory using a register value
    int get_memory_address(int memory_address);
    void display_memory_cells();
    int enter_instructions();
    void clear_memory();
    ~Memory();
};

// Program Instructions class
class ProgIns {
public:
    string instruction;

    ProgIns();  // Constructor
    ~ProgIns(); // Destructor

    char getOpCode_from_ins(); // Extract opcode
    int getRegister_from_ins(); // Extract register index
    int getAddress_Value_from_ins(); // Extract address/value

    // Method to execute program instructions
    void executeInstructions();
};

// Simulator class
class Simulator {
public:
    Simulator(); // Constructor
    void loadProgram_exe_all(); // Load instructions into memory
    void loadProgram_step_by_step();

private:
    Memory memory;    // Memory object
    Registers registers; // Registers object
    int ProgramCounter; // Program counter
};

// Utility functions
void slowPrint(const string& message, int delay = 50);

#endif // VOLE_MACHINE_H
