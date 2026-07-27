// ECE 2305 - Tutorial 07

#include <iostream>
using namespace std;

int main() {
    // variables
    int num;
    double decimal;
    char letter;

    // integer
    cout << "Enter an integer: ";
    cin >> num;
    cout << "You entered: " << num << endl;

    // floating-point number
    cout << "Enter a decimal number: ";
    cin >> decimal;
    cout << "You entered: " << decimal << endl;

    // character
    cout << "Enter a single character: ";
    cin >> letter;
    cout << "You entered: '" << letter << "'" << endl;

    system("pause"); 
    return 0;
}
