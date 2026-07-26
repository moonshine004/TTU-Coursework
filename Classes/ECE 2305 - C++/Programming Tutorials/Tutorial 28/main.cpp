// ECE 2305 - Tutorial 28

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
    Fruit fruits[3] = {
        {"Apple", 10, 0.99},
        {"Orange", 15, 1.25},
        {"Banana", 20, 0.75}
    };

    cout << left << setw(10) << "Name" << setw(10) << "Quantity" << "Price" << endl;
    cout << "-----------------------------" << endl;

    for (int i = 0; i < 3; i++) {
        cout << setw(10) << fruits[i].name
            << setw(10) << fruits[i].quantity
            << fixed << setprecision(2) << fruits[i].price << endl;
    }

    system("pause");
    return 0;
}
