//ECE 2305 Tutorial 02 - Basic Data Types in C++
#include <iostream>

using namespace std;

int main() {
    cout << "C++ Data Types Exploration\n\n";

    int intValue = 50;
    cout << "Value of intValue: " << intValue << endl;
    cout << "Size (bytes): " << sizeof(intValue) << endl;
    cout << "Memory Address: " << &intValue << "\n\n";

    double doubleValue = 20.55;
    cout << "Memory Address of doubleValue: " << &doubleValue << endl;
    cout << "Size (bytes): " << sizeof(doubleValue) << endl;
    cout << "Value: " << doubleValue << "\n\n";

    char charValue = 'Z';
    cout << "Character: " << charValue << endl;
    cout << "ASCII Code: " << int(charValue) << endl;
    cout << "Size in Memory: " << sizeof(charValue) << " byte" << endl;
    cout << "Stored at: " << (void*)&charValue << "\n\n";

    bool boolValue = true;
    cout << "Boolean Value: " << boolalpha << boolValue << endl;
    cout << "Size of Boolean: " << sizeof(boolValue) << " byte" << endl;
    cout << "Address in Memory: " << &boolValue << endl;

    system("pause");
    return 0;
}
