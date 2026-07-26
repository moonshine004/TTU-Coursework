#include <iostream>
#include <cmath>

using namespace std;
int main() 
{
    double a, b, c;

    // User input
    cout << "Quadratic Equation Solver" << endl;
    cout << "Enter coefficient a: ";
    cin >> a;
    cout << "Enter coefficient b: ";
    cin >> b;
    cout << "Enter coefficient c: ";
    cin >> c;

    // Check if it's a valid quadratic equation
    if (a == 0) {
        if (b == 0) {
            cout << "No solution." << endl;
        }
        else {
            double x = -c / b;
            cout << "One real solution: x = " << x << endl;
        }
        return 0;
    }

    // Compute the discriminant
    double discriminant = b * b - 4 * a * c;

    // Determine the nature of the roots
    if (discriminant > 0) {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        cout << "Two real unequal solutions: x1 = " << x1 << ", x2 = " << x2 << endl;
    }
    else if (discriminant == 0) {
        double x = -b / (2 * a);
        cout << "Two real equal solutions: x = " << x << endl;
    }
    else {
        double realPart = -b / (2 * a);
        double imaginaryPart = sqrt(-discriminant) / (2 * a);
        cout << "Two complex conjugate solutions: x1 = " << realPart << " + " << imaginaryPart << "j, ";
        cout << "x2 = " << realPart << " - " << imaginaryPart << "j" << endl;
    }

    system("pause");
    return 0;
}
