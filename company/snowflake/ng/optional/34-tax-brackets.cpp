// B22 · LC 2303: brackets[i] = [upper_i, percent_i], uppers increasing; income in
// (upper_{i-1}, upper_i] is taxed at percent_i. Income above the top upper is untaxed.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;

// Tax each bracket's slice of income; the loop never slices past the top upper,
// so that part stays untaxed. T: O(B), S: O(1).
double calculateTax(const vector<vector<int>>& brackets, int income) {
    long long hundredths = 0;   // tax * 100, exact (and overflow-safe) until the divide
    int prev = 0;               // upper bound of the previous bracket
    for (const auto& b : brackets) {
        if (income <= prev) { break; }
        hundredths += 1LL * (min(income, b[0]) - prev) * b[1];
        prev = b[0];
    }
    return hundredths / 100.0;
}

void test(const string& label, const vector<vector<int>>& brackets, int income,
          double want) {
    cout << label << (calculateTax(brackets, income) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> brackets = {{3, 50}, {7, 10}, {12, 25}};
    test("three slices", brackets, 10, 2.65);
    test("above top", brackets, 20, 3.15);
    test("on upper", brackets, 3, 1.5);
    test("no income", brackets, 0, 0.0);
}
