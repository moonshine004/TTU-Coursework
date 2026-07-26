// ECE 2305 - Tutorial 17

#include <iostream>
using namespace std;

int main() {
    double sum = 0;
    for (int i = 1; i <= 100; i++) sum += 1.0 / (i * i);
    cout << "The answer to the summation is = " << sum << endl;
    cout << endl;

    system("pause");
    return 0;
}