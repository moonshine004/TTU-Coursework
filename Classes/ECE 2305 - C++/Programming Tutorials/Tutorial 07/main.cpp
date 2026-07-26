// ECE 2305 Tutorial 07 - cout and cin

#include<iostream>
using namespace std;

int main()
{
	cout << "cout and cin" << endl;
	cout << endl;

	int a;
	double b;
	char c;

	cout << "Please input an integer : ";
	cin >> a;
	cout << "You entered the value " << a << endl;
	cout << endl;

	cout << "Please input a floating point number : ";
	cin >> b;
	cout << "You entered the value " << b << endl;
	cout << endl;

	cout << "Please input a character : ";
	cin >> c;
	cout << "You entered the value " << c << endl;
	cout << endl;
	
	system("pause");
	return(0);
}