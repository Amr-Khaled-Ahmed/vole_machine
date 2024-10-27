#include "vole_machine.h"

/**
 * file: A1_T4_s5_20231116_20231134_20231042.cpp
 * By: DR\ Mohamed El_Ramly
 * Author: Amr khaled Ahmed Abd El-Hamid, Mohammed Ahmed Mohammed , George Malak Magdy
 * Section: s5, s19
 * my faculty email: 20231116@stud.fci-cu.edu.eg, 20231134@stud.fci-cu.edu.eg, 20231042@stud.fci-cu.edu.eg
 * IDs: 20231116, 20231134, 20231042
 * Date: 28th october 2024
 **/


int main() {
    // Slow-print a welcome message
    slowPrint("--\"Welcome to Our Vole Machine\"--", 0);
    {
        string choice;
        do {
            cout << "Choose\n"
                 << "1 - To load a new program\n"
                 << "2 - To exit\n"
                 << "Enter your choice:\n";
            cin >> choice;
            if (choice == "2") {
                return 0;
            }
            else if (choice == "1") {
                start_execution();
                cout << "Choose if you want to continue\n";
            }
            else {
                cerr << "Some thing happen wrong\nPlease choose a right option" << endl;
            }

        } while (choice != "2" || choice != "1");
    }
}
