// ECE 2305 - Tutorial 16
// Nested FOR Loops
#include<iostream>
using namespace std;

int main()
{
	cout << "ECE 2305 - Tutorial 16" << endl;
	cout << "Nested FOR Loops" << endl;
	cout << endl;

	cout << "\t1\t2\t3\t4\t5\t6\t7\t8\t9\t10" << endl;
	for (int m = 1; m <= 10; m++)
	{
		cout << m << " \t";
		for (int n = 1; n <= 10; n++)
			cout << m * n << " \t";
		cout << endl;
	}
	cout << endl;
	system("pause");
	return 0;
}