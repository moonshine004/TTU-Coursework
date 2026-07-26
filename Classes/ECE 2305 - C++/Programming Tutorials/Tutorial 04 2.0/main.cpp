// Tutorial 04 - Memory Map but with Floating Point Numbers
#include <iostream>
using namespace std;

int main() 
{
    double a = 1.1, b = 2.2, c = 3.3, d = 4.4, e = 5.5;

    cout << "Variable\tValue\tAddress\n";
    cout << "a\t\t" << a << "\t" << &a << endl;
    cout << "b\t\t" << b << "\t" << &b << endl;
    cout << "c\t\t" << c << "\t" << &c << endl;
    cout << "d\t\t" << d << "\t" << &d << endl;
    cout << "e\t\t" << e << "\t" << &e << endl;

    cout << "\nMemory Spacing:\n";
    cout << ((&b - &a) * sizeof(double)) << " bytes\n";
    cout << ((&c - &b) * sizeof(double)) << " bytes\n";
    cout << ((&d - &c) * sizeof(double)) << " bytes\n";
    cout << ((&e - &d) * sizeof(double)) << " bytes\n";

    system("pause");
    return 0;
}
