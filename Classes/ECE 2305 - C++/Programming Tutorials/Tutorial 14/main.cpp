// ECE 2305 - Tutorial 14
// WHILE Structure
#include<iostream>
using namespace std;

int main()
{
	cout << "ECE 2305 - Tutorial 14" << endl;
	cout << "WHILE Structure" << endl;
	cout << endl;

	int n = 0;
	while (n * n < 75)
	{
		cout << "n = " << n << " , n^2 = " << n * n << endl;
		n = n + 1;
	}
	system("pause");
	return 0;
}