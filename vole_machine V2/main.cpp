#include "vole_machine.h"
int main() {
    // Slow-print a welcome message
    slowPrint("--\"Welcome to Our Vole Machine\"--", 0);
    {
        string choice;
        do {
            cout << "Choose\n"
                << "1 - To load a new program\n"
                << "2 - To exit\n";
            cin >> choice;
            if (choice == "2") {
                return 0;
            }
            else if (choice == "1") {
                start_excecution();
                cout << "Choose if you want to continue\n";
            }
            else {
                cerr << "Some thing happen wrong\nPlease choose a right option" << endl;
            }

        } while (choice != "2" || choice != "1");
    }

    //int R = stoi(line.substr(1, 1), nullptr, 16); // Extract the register index R
    /**int XY = stoi(line.substr(2, 2), nullptr, 16); // Extract the memory address XY
    regs.setValue(R, memory.get_memory_address(XY)); // Set register R with value from memory address XY**/
}
