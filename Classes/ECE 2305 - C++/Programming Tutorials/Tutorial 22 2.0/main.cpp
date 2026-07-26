// ECE 2305 - Tutorial 22

#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    int a = 10;
    int* pa = &a; 

    int b[4] = { 1, 2, 3, 4 };
    int* pb = b; 

    int* pc = new int;
    *pc = 20;

    int* pd = new int[4];
    pd[0] = 5; pd[1] = 10; pd[2] = 15; pd[3] = 20;

    cout << left << setw(15) << "Variable" << setw(15) << "Address" << setw(15) << "Value" << endl;
    cout << endl;
    cout << setw(15) << "a" << setw(15) << &a << setw(15) << a << endl;
    cout << setw(15) << "pa" << setw(15) << &pa << setw(15) << pa << endl;
    cout << setw(15) << "b[0]" << setw(15) << &b[0] << setw(15) << b[0] << endl;
    cout << setw(15) << "b[1]" << setw(15) << &b[1] << setw(15) << b[1] << endl;
    cout << setw(15) << "b[2]" << setw(15) << &b[2] << setw(15) << b[2] << endl;
    cout << setw(15) << "b[3]" << setw(15) << &b[3] << setw(15) << b[3] << endl;
    cout << setw(15) << "pb" << setw(15) << &pb << setw(15) << pb << endl;
    cout << setw(15) << "pc" << setw(15) << &pc << setw(15) << pc << endl;
    cout << setw(15) << "*pc" << setw(15) << pc << setw(15) << *pc << endl;
    cout << setw(15) << "pd[0]" << setw(15) << &pd[0] << setw(15) << pd[0] << endl;
    cout << setw(15) << "pd[1]" << setw(15) << &pd[1] << setw(15) << pd[1] << endl;
    cout << setw(15) << "pd[2]" << setw(15) << &pd[2] << setw(15) << pd[2] << endl;
    cout << setw(15) << "pd[3]" << setw(15) << &pd[3] << setw(15) << pd[3] << endl;
    cout << setw(15) << "pd" << setw(15) << &pd << setw(15) << pd << endl;

    delete pc;
    delete[] pd;

    return 0;
    system("pause");
}
