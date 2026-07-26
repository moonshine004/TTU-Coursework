// ECE 2305 - Tutorial 24

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double a[4] = { 2.3, 4.1, 6.2, 8.3 };
    double b = 258.6;
    double* c;
    double* d;

    c = a;
    d = &b;

    cout << left << setw(12) << "Variable" << setw(20) << "Address" << "Value" << endl;
    cout << endl;
    for (int i = 0; i < 4; i++) {
        cout << "a[" << i << "]" << setw(8) << "" << setw(20) << &a[i] << a[i] << endl;
    }
    cout << setw(12) << "b" << setw(20) << &b << b << endl;
    cout << setw(12) << "*c" << setw(20) << c << *c << endl;
    cout << setw(12) << "*d" << setw(20) << d << *d << endl;

    // Perform pointer updates
    cout << "\n--- After pointer arithmetic and assignments ---\n";

    c = c + 3;
    *c = 10.4;
    *d = 517.2;

    cout << left << setw(12) << "Variable" << setw(20) << "Address" << "Value" << endl;
    cout << endl;
    for (int i = 0; i < 4; i++) {
        cout << "a[" << i << "]" << setw(8) << "" << setw(20) << &a[i] << a[i] << endl;
    }
    cout << setw(12) << "b" << setw(20) << &b << b << endl;
    cout << setw(12) << "*c" << setw(20) << c << *c << endl;
    cout << setw(12) << "*d" << setw(20) << d << *d << endl;
    
    system("pause");
    return 0;
}
