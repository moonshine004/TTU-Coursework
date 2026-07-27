// ECE 2305 - Tutorial 21
// Recursive Function Calls
#include <iostream>

using namespace std;

// Recursive function to calculate factorial
long long factorial(int n) {
    // Print memory address of n for tracing
    cout << "Calculating factorial(" << n << ") | Memory address of n: " << &n << endl;

    // Base case: if n is 0 or 1, return 1
    if (n == 0 || n == 1) {
        return 1;
    }

    // Recursive case: n! = n * (n-1)!
    long long result = n * factorial(n - 1);

    // Print returning value
    cout << "Returning " << result << " for factorial(" << n << ") | Memory address of result: " << &result << endl;

    return result;
}

int main() {
    int num;

    // Ask the user for input
    cout << "Enter a number to calculate its factorial: ";
    cin >> num;

    // Check for negative numbers
    if (num < 0) {
        cout << "Factorial is not defined for negative numbers." << endl;
    }
    else {
        // Call factorial function and display the result
        long long fact = factorial(num);
        cout << "Factorial of " << num << " is: " << fact << endl;
    }

    system("pause");
    return 0;
}
