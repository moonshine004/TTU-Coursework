//ECE 2305 - Tutorial 25
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    double a[2][2] = { {2.3, 4.1}, {6.2, 8.3} };
    double b = 258.6;
    double* c;
    double* d;

    c = a[0];
    d = &b;

    cout << left << setw(12) << "Variable" << setw(20) << "Address" << "Value" << endl;
    cout << endl;
    cout << "a[0][0]" << setw(5) << "" << setw(20) << &a[0][0] << a[0][0] << endl;
    cout << "a[0][1]" << setw(5) << "" << setw(20) << &a[0][1] << a[0][1] << endl;
    cout << "a[1][0]" << setw(5) << "" << setw(20) << &a[1][0] << a[1][0] << endl;
    cout << "a[1][1]" << setw(5) << "" << setw(20) << &a[1][1] << a[1][1] << endl;
    cout << endl;
    cout << setw(12) << "b" << setw(20) << &b << b << endl;
    cout << setw(12) << "*c" << setw(20) << c << *c << endl;
    cout << setw(12) << "*d" << setw(20) << d << *d << endl;
    cout << endl;

    cout << "After pointer arithmetic and assignments" << endl;
    cout << "----------------------------------------\n";

    c = c + 3;
    *c = 10.4;
    *d = 517.2;

    cout << left << setw(12) << "Variable" << setw(20) << "Address" << "Value" << endl;
    cout << endl;
    cout << "a[0][0]" << setw(5) << "" << setw(20) << &a[0][0] << a[0][0] << endl;
    cout << "a[0][1]" << setw(5) << "" << setw(20) << &a[0][1] << a[0][1] << endl;
    cout << "a[1][0]" << setw(5) << "" << setw(20) << &a[1][0] << a[1][0] << endl;
    cout << "a[1][1]" << setw(5) << "" << setw(20) << &a[1][1] << a[1][1] << endl;
    cout << endl;
    cout << setw(12) << "b" << setw(20) << &b << b << endl;
    cout << setw(12) << "*c" << setw(20) << c << *c << endl;
    cout << setw(12) << "*d" << setw(20) << d << *d << endl;

    system("pause");
    return 0;
}
