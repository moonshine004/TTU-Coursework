// ECE 2305 - Tutorial 19
// Overloaded Functions
#include<iostream>
using namespace std;

void function(int n)
{
	cout << "Welcome to function for int" << endl;
	cout << "n \t" << &n << "\t" << n << endl;
	cout << "Returning to main" << endl;
	cout << endl;
}

void function(double n)
{
	cout << "Welcome to function for double" << endl;
	cout << "n \t" << &n << "/t" << n << endl;
	cout << "Returning to main" << endl;
	cout << endl;
}

int main()
{
	cout << "ECE 2305 - Tutorial 19" << endl;
	cout << "Overloaded Functions" << endl;
	cout << endl;
	cout << "Executing Function main" << endl;
	cout << endl;
	cout << "Call function for int" << endl;
	cout << endl;
	function(100);
	cout << "Welcome back to main" << endl;
	cout << endl;
	cout << "Call function for double" << endl;
	cout << endl;
	function(3.14159);
	cout << endl;
	cout << "End of function main" << endl;
	cout << endl;

	system("pause");
	return 0;
}