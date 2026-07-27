// ECE 2305 Tutorial 05 - Arithmetic Operators and Order of Precedence

#include <iostream>
using namespace std;

int main()
{
	cout << "Arithmetic Operators and Orders of Precedence" << endl;
	cout << endl;

	cout << "x = y = z = 10.0" << endl;
	cout << endl;
	double x = 10.0;
	double y = 10.0;
	double z = 10.0;
	double a = 3 * x + y;
	double b = (x + y) / 7;
	double c = (3 * x + y) / (z + 2);
	cout << "a = 3 * x + y			---> a = " << a << endl;
	cout << "b = (x + y) / 7			---> b = " << b << endl;
	cout << "c = (3 * x + y) / (z + 2)	---> c = " << c << endl;
	cout << endl;

	system("pause");
	return 0;
}