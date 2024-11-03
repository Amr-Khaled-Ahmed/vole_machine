#include "A1_T4_S5_S19_20231134_20231116_20231042.h"

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
    UTL ut;
    ut.slowPrint("--\"Welcome to Our Vole Machine\"--", 50);
    {
        string choice;
        do {
            cout << "Choose\n"
                 << "1 - To load a new program\n"
                 << "2 - To exit\n"
                 << "Enter your choice:\n";
            cin >> choice;
            if (choice == "2") {
                ut.slowPrint("***\"Thanks for using our program\"***", 50);
                this_thread::sleep_for(chrono::milliseconds(2000));
                return 0;
            }
            else if (choice == "1") {
                Simulator s;
                do{
                    cout << "Choose\n"
                         << "1. To execute line by line" << endl
                         << "2. To execute the whole program in one step:\n";
                    cin >> choice;
                    if (choice == "2") {
                        s.loadProgram_exe_all();
                        break;
                    } else if (choice == "1"){
                        s.loadProgram_step_by_step();
                        break;

                    }else {
                        cerr << "Some thing happen wrong\nPlease choose a right option" << endl;
                        continue;
                    }
                }while (choice != "2" || choice != "1");
            }
            else {
                cerr << "Some thing happen wrong\nPlease choose a right option" << endl;
            }

        } while (choice != "2" || choice != "1");
    }
}
