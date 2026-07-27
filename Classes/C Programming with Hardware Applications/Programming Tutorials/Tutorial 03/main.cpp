//ECE 2305 Tutorial 03 - Integer Memory Map

#include <iostream>
using namespace std;

int main()
{
	int a = 10;
	int b = 20;
	int c = 30;
	int d = 40;
	int e = 50;

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
