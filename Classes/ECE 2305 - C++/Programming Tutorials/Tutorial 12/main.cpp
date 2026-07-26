// ECE 2305 - Tutorial 12
// IF - ELSE Structure
#include<iostream>
using namespace std;

int main()
{
	cout << "ECE 2305 - Tutorial 12" << endl;
	cout << "IF - ELSE Structure" << endl;

	int a;
	cout << "Please enter an integer: ";
	cin >> a;
	cout << endl;
	cout << "a = " << a << endl;

	if (a < 5) cout << "a < 5" << endl;
	else if (a < 10) cout << "5 <= a < 10" << endl;
	else if (a < 15) cout << "10 <= a < 20" << endl;
	cout << endl;

	system("pause");
	return 0;
}