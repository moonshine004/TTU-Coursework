// ECE 2305 - Tutorial 17
// Summation
#include <iostream>
using namespace std;

int main()
{
	cout << "ECE 2305 - Tutorial 17" << endl;
	cout << "Summation" << endl;
	cout << endl;

	double sum = 0;
	for (int n = 1; n <= 100; n++)
	{
		cout << "old sum = " << sum << endl;
		cout << "n = " << n << endl;
		cout << "1/n^2 = " << 1 / (double(n) * double(n)) << endl;
		sum = sum + 1 / (double(n) * double(n));
		cout << "new sum = " << sum << endl;
		cout << endl;
	}
	cout << "Final sum = " << sum << endl;
	system("pause");
	return 0;
}