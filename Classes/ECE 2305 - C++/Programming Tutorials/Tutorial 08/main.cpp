// ECE 2305 - Tutorial 08
// Math Function Calls

#include<iostream>
#include<cstdlib>
#include<cmath>
using namespace std;

int main()
{
	cout << "Math Function calls" << endl;
	cout << "<cmath> and <cstdlib> functions:" << endl;
	cout << endl;
	const double PI = 4 * atan(1.0);
	cout << "PI = " << PI << endl;
	const double e = exp(1.0);
	cout << "e = " << e << endl;
	cout << "abs(-6) = " << abs(-6) << endl;
	cout << "abs(-6.4) = " << abs(-6.4) << endl;
	cout << "30 deg = " << 30 * PI / 180 << " radians" << endl;
	cout << "3*PI/4 radians = " << (3 * PI / 4) * 180 / PI << "degrees" << endl;
	cout << "sin(30) = " << sin(30 * PI / 180) << endl;
	cout << "cos(30) = " << cos(30 * PI / 180) << endl;
	cout << "tan(30) = " << tan(30 * PI / 180) << endl;
	cout << "ceil(8.6) = " << ceil(8.6) << endl;
	cout << "ceil(8.4) = " << ceil(8.4) << endl;
	cout << "ceil(-8.4) = " << ceil(-8.4) << endl;
	cout << "ceil(-8.6) = " << ceil(-8.6) << endl;
	cout << "floor(8.6) = " << floor(8.6) << endl;
	cout << "floor(8.4) = " << floor(8.4) << endl;
	cout << "floor(-8.4) = " << floor(-8.4) << endl;
	cout << "floor(-8.6) = " << floor(-8.6) << endl;
	cout << "round(8.6) = " << round(8.6) << endl;
	cout << "round(8.4) = " << round(8.4) << endl;
	cout << "round(-8.4) = " << round(-8.4) << endl;
	cout << "round(-8.6) = " << round(-8.6) << endl;
	cout << "20*log10(100) = " << 20 * log10(100) << " dB" << endl;
	cout << "sqrt(2) = " << sqrt(2) << endl;
	cout << "sqrt(-2) = " << sqrt(-2) << endl;
	cout << "random number = " << rand() << endl;
	cout << "random number = " << rand() << endl;
	cout << "random number = " << rand() << endl;
	cout << "random number = " << rand() << endl;
	cout << "RAND_MAX = " << RAND_MAX << endl;

	system("pause");
	return(0);
}