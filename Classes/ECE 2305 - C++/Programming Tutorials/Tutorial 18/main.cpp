// ECE 2305 - Tutorial 18 
// User-define Functions and Functions Calls
#include<iostream>
using namespace std;

int functionA(int n)
{
	n = n + 1;
	cout << "Function A has been called " << n << " times." << endl;
	cout << "Memory Map for Function A" << endl;
	cout << "Name \tAddress \tValue" << endl;
	cout << "n \t" << &n << "\t" << n << endl;
	return n;
}
int functionB(int n)
{
	n = n + 1;
	cout << "Function B has been called " << n << " times." << endl;
	cout << "Memory Map for Function B" << endl;
	cout << "Name \tAddress \tValue" << endl;
	cout << "n \t" << &n << "\t" << n << endl;
	return n;
}
int main()
{
	int a = 0;
	int b = 0;
	int choice = 0;
	do
	{
		system("cls");

		cout << "ECE 2305 - Tutorial 18" << endl;
		cout << "User-defined Functions and Function Calls" << endl;
		cout << endl;

		cout << "Memory Map for Function Main" << endl;
		cout << "Name \tAddress \tValue" << endl;
		cout << "a \t" << &a << "\t" << a << endl;
		cout << "b \t" << &b << "\t" << b << endl;
		cout << endl;

		cout << "Main Menu." << endl;
		cout << "1. Call Function A." << endl;
		cout << "2. Call Function B." << endl;
		cout << "3. End." << endl;
		cout << endl;
		cout << "Please make selection: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			cout << "The user chose Function A." << endl;
			cout << endl;
			a = functionA(a);
			cout << endl;
			system("pause");
			break;
		case 2:
			cout << "The user chose Function B." << endl;
			cout << endl;
			b = functionB(b);
			cout << endl;
			system("pause");
			break;
		case 3:
			cout << "The user chose to end." << endl;
			cout << endl;
			system("pause");
			break;
		default:
			cout << "Please make a proper choice." << endl;
			system("pause");
		}
	} while (choice != 3);
	return 0;
}