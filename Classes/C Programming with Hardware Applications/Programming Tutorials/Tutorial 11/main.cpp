// ECE 2305 - Tutorial 11
// IF Structure
#include<iostream>
using namespace std;

int main()
{
	cout << "ECE 2305 - Tutorial 11" << endl;
	cout << "IF Structure" << endl;

	int a;
	cout << "Please enter an integar: ";
	cin >> a;
	cout << endl;
	if (a < 5) cout << "a < 5" << endl;
	if (a < 10) cout << "a < 10" << endl;
	if (a < 15) cout << "a < 15" << endl;
	if (a < 20) cout << "a < 20" << endl;
	cout << endl;

	system("pause");
	return 0;
}