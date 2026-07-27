// ECE 2305 - Tutorial 10

#include <iostream>
using namespace std;

int main() {
    bool A, B;

    // NOT operation
    cout << "Truth Table for NOT\n";
    A = 0;
    cout << "A = " << A << "\t!A = " << !A << endl;
    A = 1;
    cout << "A = " << A << "\t!A = " << !A << endl;
    cout << endl;

    // AND, OR, NAND, NOR, XOR, XNOR operations
    cout << "Truth Table for Boolean Operations\n";

    A = 0; B = 0;
    cout << "A = " << A << ", B = " << B << "\tAND: " << (A && B) << " OR: " << (A || B) << " NAND: " << !(A && B) << " NOR: " << !(A || B) << " XOR: " << (A && !B || !A && B) << " XNOR: " << (A && B || !A && !B) << endl;

    A = 0; B = 1;
    cout << "A = " << A << ", B = " << B << "\tAND: " << (A && B) << " OR: " << (A || B) << " NAND: " << !(A && B) << " NOR: " << !(A || B) << " XOR: " << (A && !B || !A && B) << " XNOR: " << (A && B || !A && !B) << endl;

    A = 1; B = 0;
    cout << "A = " << A << ", B = " << B << "\tAND: " << (A && B) << " OR: " << (A || B) << " NAND: " << !(A && B) << " NOR: " << !(A || B) << " XOR: " << (A && !B || !A && B) << " XNOR: " << (A && B || !A && !B) << endl;

    A = 1; B = 1;
    cout << "A = " << A << ", B = " << B << "\tAND: " << (A && B) << " OR: " << (A || B) << " NAND: " << !(A && B) << " NOR: " << !(A || B) << " XOR: " << (A && !B || !A && B) << " XNOR: " << (A && B || !A && !B) << endl;

    cout << endl;

    system("pause");
    return 0;
}
