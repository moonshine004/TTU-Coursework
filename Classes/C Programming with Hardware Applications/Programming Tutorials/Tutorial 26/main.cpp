//ECE 2305 - Tutorial 26

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct Fruit {
    string name;
    int quantity;
    double price;
};

int main() {
    Fruit apple = { "Apple", 10, 0.99 };
    Fruit orange = { "Orange", 15, 1.25 };
    Fruit banana = { "Banana", 20, 0.75 };

    cout << left << setw(10) << "Name" << setw(10) << "Quantity" << "Price" << endl;
    cout << "-----------------------------" << endl;
    cout << setw(10) << apple.name << setw(10) << apple.quantity << fixed << setprecision(2) << apple.price << endl;
    cout << setw(10) << orange.name << setw(10) << orange.quantity << fixed << setprecision(2) << orange.price << endl;
    cout << setw(10) << banana.name << setw(10) << banana.quantity << fixed << setprecision(2) << banana.price << endl;

    system("pause");
    return 0;
}
