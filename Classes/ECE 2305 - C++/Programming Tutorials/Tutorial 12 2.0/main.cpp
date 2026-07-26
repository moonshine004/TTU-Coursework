// ECE 2305 - Tutorial 12

#include <iostream>
using namespace std;

int main() {
    int num;

    cout << "Please enter a value: ";
    cin >> num;

    if (num < 5) {
        cout << "The number is less than 5." << endl;
    }
    else if (num < 10) {
        cout << "5 <= number < 10" << endl;
    }
    else if (num < 15) {
        cout << "10 <= number < 15" << endl;
    }
    else if (num < 20) {
        cout << "15 <= number < 20" << endl;
    }

    system("pause");
    return 0;
}
