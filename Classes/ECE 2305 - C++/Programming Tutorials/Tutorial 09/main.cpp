// ECE 2305 - Tutorial 09
// Boolean Functions

#include<iostream>
using namespace std;

int main()
{
	int a = 10;
	int b = 5; // the variable will take the value 5, 10, or 15

	cout << "ECE 2305 - Tutorial 09" << endl;
	cout << "Boolean Operators" << endl;
	cout << endl;

	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << endl;
	cout << "a < b = " << (a < b) << endl;
	cout << "a <= b = " << (a <= b) << endl;
	cout << "a == b = " << (a == b) << endl;
	cout << "a >= b = " << (a >= b) << endl;
	cout << "a > b = " << (a > b) << endl;
	cout << endl;
	cout << "b > 5 && b > 15 = " << (b > 5 && b > 15) << endl;
	cout << "b > 5 && b < 15 = " << (b > 5 && b < 15) << endl;
	cout << "b < 5 && b > 15 = " << (b < 5 && b > 15) << endl;
	cout << "b < 5 && b < 15 = " << (b < 5 && b < 15) << endl;
	cout << endl;
	cout << "b > 5 ||  b > 15 = " << (b > 5 || b > 15) << endl;
	cout << "b > 5 ||  b < 15 = " << (b > 5 || b < 15) << endl;
	cout << "b < 5 ||  b > 15 = " << (b < 5 || b > 15) << endl;
	cout << "b < 5 ||  b < 15 = " << (b < 5 || b < 15) << endl;
	cout << endl;
	system("pause");
	return(0);
}