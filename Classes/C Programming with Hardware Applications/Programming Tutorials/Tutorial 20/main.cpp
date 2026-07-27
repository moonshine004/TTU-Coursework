// ECE 2305 - Tutorial 20
// Iterative Calculations
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    // Variables to store values
    double phi_old, phi_new, difference, tolerance;
    int count1 = 0, count2 = 0;

    // Getting user input
    cout << "Enter an initial guess for the Golden Ratio: ";
    cin >> phi_old;
    cout << "Enter the desired accuracy (e.g., 0.000001): ";
    cin >> tolerance;

    // First method: sqrt(phi_old + 1)
    do {
        phi_new = sqrt(phi_old + 1);
        difference = fabs(phi_new - phi_old);
        phi_old = phi_new;
        count1++;
    } while (difference >= tolerance);

    cout << "Method 1 (sqrt(phi_old + 1)) took " << count1 << " iterations to converge.\n";

    // Reset values for the second method
    phi_old = 1; // Reset initial guess
    count2 = 0;

    // Second method: 1 + 1 / phi_old
    do {
        phi_new = 1 + 1 / phi_old;
        difference = fabs(phi_new - phi_old);
        phi_old = phi_new;
        count2++;
    } while (difference >= tolerance);

    cout << "Method 2 (1 + 1 / phi_old) took " << count2 << " iterations to converge.\n";

    // Compare results
    if (count1 < count2) {
        cout << "Method 1 converged faster.\n";
    }
    else if (count2 < count1) {
        cout << "Method 2 converged faster.\n";
    }
    else {
        cout << "Both methods took the same number of iterations.\n";
    }

    system("pause");
    return 0;
}

