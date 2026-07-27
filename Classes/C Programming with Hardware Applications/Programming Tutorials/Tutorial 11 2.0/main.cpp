// ECE 2305 - Tutorial 11

#include <iostream>
using namespace std;

int main() {
    int num;
    cout << "Please enter a value: ";
    cin >> num;

    if (num < 5) {
        cout << "The number is less than 5." << endl;
    }
    if (num < 10) {
        cout << "The number is less than 10." << endl;
    }
    if (num < 15) {
        cout << "The number is less than 15." << endl;
    }
    if (num < 20) {
        cout << "The number is less than 20." << endl;
    }

    system("pause");
    return 0;
}
