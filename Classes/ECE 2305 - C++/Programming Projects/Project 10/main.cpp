//ECE 2305 - Programming Project 10

#include <iostream>
#include <cmath>
using namespace std;

const double PI = 4 * atan(1.0);

struct Complex {
    double real, imag;

    Complex(double r = 0, double i = 0) {
        real = r;
        imag = i;
    }

    void show() {
        cout << real << " + j" << imag << endl;
    }

    double mag() {
        return sqrt(real * real + imag * imag);
    }

    double ang() {
        return atan2(imag, real) * (180 / PI);
    }
};

int main() {
    Complex a(3, 4);

    cout << "Complex number: ";
    a.show();
    cout << "Magnitude: " << a.mag() << endl;
    cout << "Angle: " << a.ang() << " degrees" << endl;

    system("pause");
    return 0;
}
