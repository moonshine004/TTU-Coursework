// ECE 2305 - Programming Project 03
#include <iostream>
#include <cmath>
using namespace std;

double calculatePi(int terms) {
    double pi = 0;
    for (int k = 0; k <= terms; k++) {
        pi += pow(-1, k) * 4.0 / (2 * k + 1);
    }
    return pi;
}

int main() {
    const double ACTUAL_PI = 4 * atan(1); 
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(15);

    cout << "------------------------------------------------------------\n";
    cout << "N\tApprox. Pi\t\tActual Pi\t\t% Error\n";

    for (int N = 0; N <= 15; N++) {
        double approxPi = calculatePi(N);
        double error = ((approxPi - ACTUAL_PI) / ACTUAL_PI) * 100;

        cout << N << "\t" << approxPi << "\t" << ACTUAL_PI << "\t" << error << "%\n";
    }

    system("pause");
    return 0;
}
