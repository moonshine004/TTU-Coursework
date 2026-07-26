// ECE 2305 Tutorial 05 - Division and Modulo Division

#include<iostream>
using namespace std;

int main()
{
	cout << "Division and Modulo Division" << endl;
	cout << endl;

	cout << "7 / 2 ---> " << 7 / 2 << endl;
	cout << "Integer Disivion returns the whole number part." << endl;
	cout << endl;

	cout << "7 % 2 ---> " << 7 % 2 << endl;
	cout << "Modulo Division returns the remainder parts." << endl;
	cout << endl;

	cout << "7.0 / 2.0 ---> " << 7.0 / 2.0 << endl;
	cout << "Floating Point divison returns the complete result" << endl;
	cout << endl;

	//cout << "7.0 % 2.0 ---> " << 7.0 % 2.0 << endl;
	cout << "7.0 % 2.0 ---> error" << endl;
	cout << "Modulo division cannot be done with floating point numbers." << endl;
	cout << endl;

	system("pause");
	return 0;

}