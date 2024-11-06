#ifndef A1_T4_S5_S19_20231134_20231116_20231042_H
#define A1_T4_S5_S19_20231134_20231116_20231042_H
/**
 * file: A1_T4_s5_20231116_20231134_20231042.cpp
 * By: DR\ Mohamed El_Ramly
 * Author: Amr khaled Ahmed Abd El-Hamid, Mohammed Ahmed Mohammed , George Malak Magdy
 * Section: s5, s19
 * my faculty email: 20231116@stud.fci-cu.edu.eg, 20231134@stud.fci-cu.edu.eg, 20231042@stud.fci-cu.edu.eg
 * IDs: 20231116, 20231134, 20231042
 * Date: 28th october 2024
 **/
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

class UTL {
public:
    // Function to convert a single hex digit to an unsigned char
    unsigned char hexa_char(char hex);
    // Function to convert a hex string to an unsigned char
    unsigned char hexa_unsigned_chars(const string &hex);
    // Function to convert an unsigned char to a hex string
    string char_to_hex(unsigned char value);
    // Normalize the result
    unsigned char Normalize_IEEE(unsigned char S, unsigned char E, unsigned char M);
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
    void slowPrint(const string& message, int delay = 50);
};


// Registers class
class Registers {
private:
    vector<int> registers; // Store register values
public:
    Registers();
    int Get_Register_Value(int regIndex); // Get value from a register
    void setValue_to_register(int regIndex, int value); // Set value in a register
    void display_Registers(); // Display register contents
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
#endif // VOLE_MACHINE_H