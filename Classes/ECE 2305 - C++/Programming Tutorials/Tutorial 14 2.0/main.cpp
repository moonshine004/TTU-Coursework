// ECE 2305 - Tutorial 14 

#include <iostream>
using namespace std;

int main() {
    int n = 0;

    cout << "n\tn^2" << endl;
    cout << endl;

    while (n * n < 75) {
        cout << n << "\t" << (n * n) << endl;
        n++;
    }

    system("pause");
    return 0;
}
