#include "vole_machine.h"
#include <iostream>
#include <iomanip>
using namespace std; // Add this line
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







}
