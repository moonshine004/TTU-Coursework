// ECE 2305 - Tutorial 23

#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    double a = 3.1416;
    double* pa = &a;

    double b[4] = { 1.1, 2.2, 3.3, 4.4 };
    double* pb = b;

    double* pc = new double;
    *pc = 5.5;

    double* pd = new double[4];
    pd[0] = 6.6; pd[1] = 7.7; pd[2] = 8.8; pd[3] = 9.9;

    cout << left << setw(15) << "Variable" << setw(20) << "Address" << "Value" << endl;
    cout << endl;
    cout << setw(15) << "a" << setw(20) << &a << a << endl;
    cout << setw(15) << "pa" << setw(20) << &pa << pa << endl;

    for (int i = 0; i < 4; i++) {
        cout << "b[" << i << "]" << setw(12) << "" << setw(20) << &b[i] << b[i] << endl;
    }
    cout << setw(15) << "pb" << setw(20) << &pb << pb << endl;

    cout << setw(15) << "pc" << setw(20) << &pc << pc << endl;
    cout << setw(15) << "*pc" << setw(20) << pc << *pc << endl;

    for (int i = 0; i < 4; i++) {
        cout << "pd[" << i << "]" << setw(12) << "" << setw(20) << &pd[i] << pd[i] << endl;
    }
    cout << setw(15) << "pd" << setw(20) << &pd << pd << endl;

    delete pc;
    delete[] pd;
    
    system("pause");
    return 0;
}
