//ECE 2305 - Programming Project 06

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

string decimalToHex(int num) {
    stringstream ss;
    ss << uppercase << setfill('0') << setw(4) << hex << num;
    return ss.str();
}

int hexToDecimal(const string& hex) {
    return stoi(hex, nullptr, 16);
}

bool isValidHex(const string& hex) {
    if (hex.length() != 4) return false;
    for (char c : hex) {
        if (!isxdigit(c)) return false;
    }
    return true;
}

int main() {
    int choice;
    while (true) {
        cout << "\nMenu:\n1. Enter Decimal Number\n2. Enter Hexadecimal Number\n3. End\nChoice: ";
        cin >> choice;
        if (choice == 3) break;

        switch (choice) {
        case 1: {
            int num;
            cout << "Enter decimal (0-65535): ";
            cin >> num;
            if (num >= 0 && num <= 65535)
                cout << "Hexadecimal: " << decimalToHex(num) << "\n";
            else
                cout << "Invalid decimal number.\n";
            break;
        }
        case 2: {
            string hex;
            cout << "Enter 4-digit hexadecimal: ";
            cin >> hex;
            if (isValidHex(hex))
                cout << "Decimal: " << hexToDecimal(hex) << "\n";
            else
                cout << "Invalid hexadecimal number.\n";
            break;
        }
        default:
            cout << "Invalid choice.\n";
        }
        system("pause");
    }
    system("pause");
    return 0;
}

