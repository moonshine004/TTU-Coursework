// ECE 2305 - Tutorial 15 

#include <iostream>
using namespace std;

int main() {
    cout << "n\tn^2\tn^3" << endl;

    for (int n = 0; n <= 10; n++) {
        cout << n << "\t" << (n * n) << "\t" << (n * n * n) << endl;
    }

    system("pause");
    return 0;
}
