// ECE 2305 - Project 8

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    int n;
    cout << "Enter the number of equations: ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n + 1));
    vector<double> x(n);

    cout << "Enter the augmented matrix (each row should have " << n + 1 << " values):\n";
    for (int i = 0; i < n; i++) {
        cout << "Row " << i + 1 << ": ";
        for (int j = 0; j <= n; j++) {
            cin >> A[i][j];
        }
    }

    // forward elimination
    for (int i = 0; i < n - 1; i++) {
        if (A[i][i] == 0.0) {
            cout << "Division by zero detected during forward elimination.\n";
            return 1;
        }
        for (int j = i + 1; j < n; j++) {
            double ratio = A[j][i] / A[i][i];
            for (int k = i; k <= n; k++) {
                A[j][k] -= ratio * A[i][k];
            }
        }
    }

    // back substitution
    for (int i = n - 1; i >= 0; i--) {
        x[i] = A[i][n];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        if (A[i][i] == 0.0) {
            cout << "Division by zero detected during back substitution.\n";
            return 1;
        }
        x[i] /= A[i][i];
    }

    cout << "\nSolution:\n";
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(4) << x[i] << endl;
    }

    system("pause");
    return 0;
}
