// Tutorial 05 - Int, Modulo, and Floating Pt Division
#include <iostream>
using namespace std;

int main() 
{
    cout << "Operations on Division and Modulus\n\n";

    cout << "7 / 2 = " << 7 / 2 << "  (Integer division, no decimals)\n";
    cout << "7 % 2 = " << 7 % 2 << "  (Remainder after division)\n";
    cout << "7.0 / 2.0 = " << 7.0 / 2.0 << "  (Floating-point division, includes decimals)\n";
    cout << "7.0 % 2.0 = Error (Modulus only works with whole numbers)\n";

    system("pause");
    return 0;
}
