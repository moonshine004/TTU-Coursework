// ECE 2305 Programming Project 04
#include <iostream>
#include <cmath>

using namespace std;

const double PI = 4 * atan(1);

// Structure to represent a complex number
struct Complex {
    double real;
    double imag;
};

// Function prototypes
void inputRectangular(Complex& num);
void inputPolar(Complex& num);
void convertToPolar(Complex num, double& magnitude, double& angle);
void convertToRectangular(double magnitude, double angle, Complex& num);
Complex addComplex(Complex a, Complex b);
Complex conjugateComplex(Complex a);
Complex multiplyComplex(Complex a, Complex b);
Complex divideComplex(Complex a, Complex b);

int main() {
    Complex A = { 0, 0 }, B = { 0, 0 }, C = { 0, 0 };
    int choice;

    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(3);

    do {
        // Display menu
        cout << "\n----------------------------------\n";
        cout << "  ECE 2305 - Complex Number Calculator\n";
        cout << "----------------------------------\n";
        cout << "Complex Number A: " << A.real << " + " << A.imag << "i\n";
        cout << "Complex Number B: " << B.real << " + " << B.imag << "i\n";
        cout << "Complex Number C: " << C.real << " + " << C.imag << "i\n";
        cout << "\nMenu Options:\n";
        cout << "1. Enter Complex Number A (Rectangular)\n";
        cout << "2. Enter Complex Number A (Polar)\n";
        cout << "3. Enter Complex Number B (Rectangular)\n";
        cout << "4. Enter Complex Number B (Polar)\n";
        cout << "5. Add A + B\n";
        cout << "6. Conjugate of A\n";
        cout << "7. Multiply A * B\n";
        cout << "8. Divide A / B\n";
        cout << "9. Exit\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            inputRectangular(A);
            break;
        case 2:
            inputPolar(A);
            break;
        case 3:
            inputRectangular(B);
            break;
        case 4:
            inputPolar(B);
            break;
        case 5:
            C = addComplex(A, B);
            break;
        case 6:
            C = conjugateComplex(A);
            break;
        case 7:
            C = multiplyComplex(A, B);
            break;
        case 8:
            C = divideComplex(A, B);
            break;
        case 9:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid option, try again.\n";
        }
    } while (choice != 9);

    system("pause");
    return 0;
}

// Function to input a complex number in rectangular form
void inputRectangular(Complex& num) {
    cout << "Enter real part: ";
    cin >> num.real;
    cout << "Enter imaginary part: ";
    cin >> num.imag;
}

// Function to input a complex number in polar form
void inputPolar(Complex& num) {
    double magnitude, angle;
    cout << "Enter magnitude: ";
    cin >> magnitude;
    cout << "Enter angle (in degrees): ";
    cin >> angle;
    convertToRectangular(magnitude, angle, num);
}

// Function to convert rectangular form to polar form
void convertToPolar(Complex num, double& magnitude, double& angle) {
    magnitude = sqrt(num.real * num.real + num.imag * num.imag);
    angle = atan2(num.imag, num.real) * 180 / PI;
}

// Function to convert polar form to rectangular form
void convertToRectangular(double magnitude, double angle, Complex& num) {
    num.real = magnitude * cos(angle * PI / 180);
    num.imag = magnitude * sin(angle * PI / 180);
}

// Function to add two complex numbers
Complex addComplex(Complex a, Complex b) {
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

// Function to find the conjugate of a complex number
Complex conjugateComplex(Complex a) {
    Complex result;
    result.real = a.real;
    result.imag = -a.imag;
    return result;
}

// Function to multiply two complex numbers
Complex multiplyComplex(Complex a, Complex b) {
    Complex result;
    result.real = (a.real * b.real) - (a.imag * b.imag);
    result.imag = (a.real * b.imag) + (a.imag * b.real);
    return result;
}

// Function to divide two complex numbers
Complex divideComplex(Complex a, Complex b) {
    Complex result;
    double denominator = (b.real * b.real + b.imag * b.imag);

    if (denominator == 0) {
        cout << "Error: Division by zero is not allowed.\n";
        return { 0, 0 };
    }

    result.real = ((a.real * b.real) + (a.imag * b.imag)) / denominator;
    result.imag = ((a.imag * b.real) - (a.real * b.imag)) / denominator;
    return result;
}
