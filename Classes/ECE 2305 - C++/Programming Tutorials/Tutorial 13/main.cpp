// ECE 2305 - Tutorial 13
// DO - WHILE and SWITCH Structure
#include<iostream>
using namespace std;

int main()
{
	int choice = 0;
	do
	{
		system("cls");

		cout << "ECE 2305 - Tutorial 13" << endl;
		cout << "DO - WHILE and SWITCH Structure" << endl;
		cout << endl;

		cout << "Main Menu." << endl;
		cout << "1. Option 1." << endl;
		cout << "2. Option 2." << endl;
		cout << "3. End." << endl;
		cout << endl;
		cout << "Please make selection: ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			cout << "The user chose option 1." << endl;
			system("pause");
			break;
		case 2:
			cout << "The user chose option 2." << endl;
			system("pause");
			break;
		case 3:
			cout << "The user chose to end." << endl;
			system("pause");
			break;
		default:
			cout << "Please make a proper choice." << endl;
			system("pause");
		}
	} while (choice != 3);

	return 0;
}