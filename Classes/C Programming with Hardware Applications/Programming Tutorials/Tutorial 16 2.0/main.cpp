// ECE 2305 - Tutorial 16

#include <iostream>
using namespace std;

int main() {
    cout << "Multiplication Table (1-10)" << endl;

    cout << "\t";
    for (int i = 1; i <= 10; i++) {
        cout << i << "\t";
    }
    cout << "\n------------------------------------------------------------------------------------\n";

    for (int i = 1; i <= 10; i++) {
        cout << i << "|\t";
        for (int j = 1; j <= 10; j++) {
            cout << (i * j) << "\t";
        }
        cout << endl;
    }

    system("pause");
    return 0;
}
