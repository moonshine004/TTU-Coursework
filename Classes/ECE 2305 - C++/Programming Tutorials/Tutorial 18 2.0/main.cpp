// ECE 2305 - Tutorial 18

#include <iostream>
using namespace std;

int functionA(int n) {
    static int countA = 0;
    countA++;
    n = n + 1;
    cout << "Function A called " << countA << " times." << endl;
    cout << "Memory Address of n in functionA: " << &n << " Value: " << n << endl;
    return n;
}

int functionB(int n) {
    static int countB = 0;
    countB++;
    n = n + 1;
    cout << "Function B called " << countB << " times." << endl;
    cout << "Memory Address of n in functionB: " << &n << " Value: " << n << endl;
    return n;
}

int main() {
    int a = 0, b = 0, choice;

    while (true) {
        cout << "\nMemory Map for Main Function:" << endl;
        cout << "Variable a Address: " << &a << " Value: " << a << endl;
        cout << "Variable b Address: " << &b << " Value: " << b << endl;

        cout << "\nMenu:" << endl;
        cout << "1. Call functionA" << endl;
        cout << "2. Call functionB" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            a = functionA(a);
        }
        else if (choice == 2) {
            b = functionB(b);
        }
        else if (choice == 3) {
            cout << "Exiting program..." << endl;
            break;
        }
        else {
            cout << "Invalid choice, please try again." << endl;
        }
    }

    system("pause");
    return 0;
}
