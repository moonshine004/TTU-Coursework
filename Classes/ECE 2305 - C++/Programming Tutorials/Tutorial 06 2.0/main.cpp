// ECE 2305 - Tutorial 06

#include <iostream>
using namespace std;

int main() 
{
    double x = 10.0;
    double y = 10.0;
    double z = 10.0;

    double a = 3 * x + y;
    double b = (x + y) / 7;
    double c = (3 * x + y) / (z + 2);

    cout << "Results of calculations:" << endl;
    cout << "a = 3x + y = "  << endl;
    cout << "b = (x + y) / 7 = " << endl;
    cout << "c = (3x + y) / (z + 2) = " << endl;
    cout << endl;

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;

    system("pause");
    return 0;
}
