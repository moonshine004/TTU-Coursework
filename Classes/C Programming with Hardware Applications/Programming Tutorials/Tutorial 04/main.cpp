//ECE 2305 Tutorial 04 - Memory Map of Floating Point

#include <iostream>
using namespace std;

int main()
{
	double a = 10.1;
	double b = 20.2;
	double c = 30.3;
	double d = 40.4;
	double e = 50.5;

	cout << "Integer Memory Map" << endl;
	cout << "Name \tAddress \tData\tBytes" << endl;
	cout << "a \t" << &a << "\t" << a << "\t" << sizeof(a) << endl;
	cout << "b \t" << &b << "\t" << b << "\t" << sizeof(b) << endl;
	cout << "c \t" << &c << "\t" << c << "\t" << sizeof(c) << endl;
	cout << "d \t" << &d << "\t" << d << "\t" << sizeof(d) << endl;
	cout << "e \t" << &e << "\t" << e << "\t" << sizeof(e) << endl;
	cout << endl;

	system("pause");
	return 0;
}
