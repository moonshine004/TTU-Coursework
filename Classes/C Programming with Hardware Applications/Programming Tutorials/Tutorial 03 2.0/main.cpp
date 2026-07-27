// Tutorial 03 - Memory Map of Integers
#include <iostream>
using namespace std;

int main() 
{
    // Declare 5 integer variables and assign values
    int a = 10, b = 20, c = 30, d = 40, e = 50;

    cout << "Variable\tValue\tAddress\n";
    cout << "a\t\t" << a << "\t" << &a << endl;
    cout << "b\t\t" << b << "\t" << &b << endl;
    cout << "c\t\t" << c << "\t" << &c << endl;
    cout << "d\t\t" << d << "\t" << &d << endl;
    cout << "e\t\t" << e << "\t" << &e << endl;

    system("pause");
    return 0;
}
