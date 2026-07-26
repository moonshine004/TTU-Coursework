#include <iostream>
#include <cmath>

using namespace std;

int main() 
{
    // Constants
    const double V_DC = 10.0;
    const double R = 10000.0;
    const double C = 100.0e-12;

    double t_micro;  // Time in microseconds

    // Prompt user for input
    cout << "Enter time (t) in microseconds (us): ";
    cin >> t_micro;

    // Validate input (must be a number and positive)
    if (cin.fail() || t_micro < 0) {
        cout << "Invalid input. Please enter a positive numeric value." << endl;
        return 1;
    }

    // Convert microseconds to seconds
    double t = t_micro * 1e-6;

    // Compute current (i) in mA
    double i_t = (V_DC / R) * exp(-t / (R * C)) * 1000;

    // Compute voltage (v) in V
    double v_t = V_DC * (1 - exp(-t / (R * C)));

    // Display results
    cout << "Current, i(t): " << i_t << " mA" << endl;
    cout << "Voltage, v(t): " << v_t << " V" << endl;

    return 0;
}
