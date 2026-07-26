// ECE 2305 - Programming Project 07

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int main() {
    ifstream in("C:\\Users\\anmal\\Desktop\\ECE 2305 - C++\\numdata.txt");
    vector<double> nums;
    double x;
    while (in >> x) nums.push_back(x);
    in.close();

    sort(nums.begin(), nums.end());
    ofstream sorted("sorted_data.txt");
    for (double n : nums) sorted << fixed << setprecision(4) << n << "\n";
    sorted.close();

    int bins[10] = {};
    for (double n : nums) if (n >= 0 && n < 100) bins[int(n / 10)]++;

    ofstream hist("histogram_table.txt");
    hist << "Bin\t\tNumber of Values\n";
    for (int i = 0; i < 10; i++) hist << i * 10 << "-" << (i + 1) * 10 << "\t\t" << bins[i] << "\n";
    hist.close();

    cout << "Done.\n";
    system("pause");
    return 0;
}
