// ECE 2305 - Tutorial 27

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
struct Fruit {
    string name;
    int quantity;
    double price;
};

double purchaseFruit(Fruit& f, int numBought) {
    if (numBought > f.quantity) {
        cout << "Not enough " << f.name << " in stock.\n";
        return 0.0;
    }
    f.quantity -= numBought;
    return numBought * f.price;
}

int main() {
    Fruit apple = { "Apple", 10, 0.99 };

    int toBuy;
    cout << "How many " << apple.name << "s would you like to purchase? ";
    cin >> toBuy;

    double total = purchaseFruit(apple, toBuy);

    if (total > 0.0) {
        cout << fixed << setprecision(2);
        cout << "Total cost: $" << total << endl;
        cout << "Remaining " << apple.name << "s: " << apple.quantity << endl;
    }

    system("pause");
    return 0;
}
