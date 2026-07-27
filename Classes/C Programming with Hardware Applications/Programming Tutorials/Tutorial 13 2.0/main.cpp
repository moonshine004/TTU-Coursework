// ECE 2305 - Tutorial 13 

#include <iostream>
using namespace std;

int main() {
    int choice;

    do {
        system("cls"); // Clear screen
        cout << "Menu Options:" << endl;
        cout << "1. Option 1" << endl;
        cout << "2. Option 2" << endl;
        cout << "3. End" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "You chose Option 1" << endl;
            system("pause");
            break;
        case 2:
            cout << "You chose Option 2" << endl;
            system("pause");
            break;
        case 3:
            cout << "Ending program..." << endl;
            system("pause");
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            system("pause");
        }
    } while (choice != 3);

    system("pause");
    return 0;
}
