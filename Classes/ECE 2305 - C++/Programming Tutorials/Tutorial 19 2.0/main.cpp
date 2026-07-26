// ECE 2305 - Tutorial 19

#include <iostream>
using namespace std;

void function(int n) {
    cout << "Welcome to function for int" << endl;
    cout << "Memory Address of n: " << &n << " Value: " << n << endl;
}

void function(double n) {
    cout << "Welcome to function for double" << endl;
    cout << "Memory Address of n: " << &n << " Value: " << n << endl;
}

int main() {
    int intValue = 10;
    double doubleValue = 5.5;

    function(intValue);
    cout << "Welcome back to main" << endl;

    function(doubleValue);
    cout << "Welcome back to main" << endl;

    system("pause");
    return 0;
}
