// ECE 2305 Daily Programming Tutorial 01
// Amna Malik
// January 20, 2025
//
// This program outputs text to the monitor

#include <iostream>  // <iostream> is required for console input/output
using namespace std; // use the standard namespace for the libraries

int main()           // the "main" function returns an integer and
// accepts no arguments
{                    // the bracket "{" starts the function

	// Output text to the monitor
	cout << "ECE 2305 Tutorial 01" << endl;
	cout << "Amna Malik" << endl;
	cout << "January 20, 2025" << endl;
	cout << endl;
	cout << "Hello World!" << endl;
	// the console output (cout) function to displays text on the monitor
	// the insertion operator (<<) inserts the text into the output stream
	// the text to be displayed is enclosed in double quotes
	// the endline (endl) moves the cursor to the next line

	system("pause");    //the pause command needed for the .exe file
	return 0;           //
}                       //the bracket "}" ends the function
