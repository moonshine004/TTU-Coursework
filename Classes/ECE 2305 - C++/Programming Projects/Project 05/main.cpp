//ECE 2305 - Programming Project 05

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

const int SIZE = 10;

void fillArray(double arr[], int size) { for (int i = 0; i < size; i++) arr[i] = rand() % 100; }
void displayArray(const double arr[], int size) { for (int i = 0; i < size; i++) cout << arr[i] << " "; cout << "\n"; }
double findMax(const double arr[], int size) { return *max_element(arr, arr + size); }
double findMin(const double arr[], int size) { return *min_element(arr, arr + size); }
void sortDescending(double arr[], int size) { sort(arr, arr + size, greater<double>()); }
double findMean(const double arr[], int size) { double sum = 0; for (int i = 0; i < size; i++) sum += arr[i]; return sum / size; }
double findMedian(const double arr[], int size) { double* tempArr = new double[size]; copy(arr, arr + size, tempArr); sort(tempArr, tempArr + size); double median = size % 2 == 0 ? (tempArr[size / 2 - 1] + tempArr[size / 2]) / 2.0 : tempArr[size / 2]; delete[] tempArr; return median; }
double findVariance(const double arr[], int size, double mean) { double sum = 0; for (int i = 0; i < size; i++) sum += pow(arr[i] - mean, 2); return sum / size; }

int main() {
    srand(time(0));
    double arr[SIZE];
    int choice;
    while (true) {
        cout << "\nMenu:\n1. New array\n2. Display\n3. Max\n4. Min\n5. Sort\n6. Mean\n7. Median\n8. Variance\n9. Exit\nChoice: ";
        cin >> choice;
        if (choice == 9) break;
        switch (choice) {
        case 1: fillArray(arr, SIZE); break;
        case 2: displayArray(arr, SIZE); break;
        case 3: cout << "Max: " << findMax(arr, SIZE) << "\n"; break;
        case 4: cout << "Min: " << findMin(arr, SIZE) << "\n"; break;
        case 5: sortDescending(arr, SIZE); displayArray(arr, SIZE); break;
        case 6: cout << "Mean: " << findMean(arr, SIZE) << "\n"; break;
        case 7: cout << "Median: " << findMedian(arr, SIZE) << "\n"; break;
        case 8: cout << "Variance: " << findVariance(arr, SIZE, findMean(arr, SIZE)) << "\n"; break;
        default: cout << "Invalid choice.\n";
        }
        system("pause");
    }
    return 0;
    system("pause");
}
