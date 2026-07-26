// ECE 2305 - Tutorial 08

#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

// Define pi and e manually
const double PI = 3.141592653589793;
const double E = 2.718281828459045;

// Value of pi
void showPi() {
    cout << "The value of pi is: " << PI << endl;
}

// Value of e
void showE() {
    cout << "The value of e (Euler's number) is: " << E << endl;
}

// Absolute values
void showAbsoluteValues() {
    int num1 = -6;
    double num2 = -6.4;

    cout << "Absolute value of " << num1 << " is: " << abs(num1) << endl;
    cout << "Absolute value of " << num2 << " is: " << fabs(num2) << endl;
}

// Convert degrees to radians
void showDegreesToRadians() {
    double degrees = 30.0;
    double radians = degrees * (PI / 180.0);
    cout << degrees << " degrees in radians is: " << radians << endl;
}

// Convert radians to degrees
void showRadiansToDegrees() {
    double radians = (3 * PI) / 4;
    double degrees = radians * (180.0 / PI);
    cout << "3 * pi / 4 in degrees is: " << degrees << endl;
}

// Sine, cosine, and tangent of 30 degrees
void showTrigFunctions() {
    double degrees = 30.0;
    double radians = degrees * (PI / 180.0);

    cout << "Sine of " << degrees << " degrees: " << sin(radians) << endl;
    cout << "Cosine of " << degrees << " degrees: " << cos(radians) << endl;
    cout << "Tangent of " << degrees << " degrees: " << tan(radians) << endl;
}

// Ceil, floor, and round values
void showCeilFloorRound() {
    double values[] = { 8.6, 8.4, -8.4, -8.6 };

    for (double num : values) {
        cout << "Number: " << num << endl;
        cout << "  Ceil (round up): " << ceil(num) << endl;
        cout << "  Floor (round down): " << floor(num) << endl;
        cout << "  Round (nearest): " << round(num) << endl;
        cout << endl;
    }
}

// Logarithm calculation in decibels
void showLogarithm() {
    double value = 100.0;
    double result = 20 * log10(value);
    cout << "20log10(100) in decibels is: " << result << endl;
}

// Square root of 2
void showSquareRoot() {
    cout << "Square root of 2 is: " << sqrt(2) << endl;
}

// 4^3 (4 raised to the power of 3)
void showPower() {
    cout << "4^3 (4 raised to the power of 3) is: " << pow(4, 3) << endl;
}

// Random number
void showRandomNumber() {
    cout << "A random number: " << rand() << endl;
}

// Maximum random number
void showRandMax() {
    cout << "The maximum value for rand() is: " << RAND_MAX << endl;
}

// Main function
int main() {
    showPi();
    showE();
    showAbsoluteValues();
    showDegreesToRadians();
    showRadiansToDegrees();
    showTrigFunctions();
    showCeilFloorRound();
    showLogarithm();
    showSquareRoot();
    showPower();
    showRandomNumber();
    showRandMax();

    system("pause");
    return 0;
}
