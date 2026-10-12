#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

using std::vector;

// T: O(n), S: O(1)
double calculateTax(const vector<vector<int>>& brackets, int income) {
    long long hundreths = 0; // tax multiplied by 100
    int prev = 0;

    for (const auto& b : brackets) {
        if (income <= prev) { break; }

        int slice = std::min(income, b[0]) - prev;
        hundreths += 1LL * slice * b[1];
        prev = b[0];
    }
    return hundreths / 100.0;
}

int main() {
    vector<vector<int>> brackets = {{3, 50}, {7, 10}, {12, 25}};

    // 3*50% + 4*10% + 3*25% = 2.65.
    assert(std::abs(calculateTax(brackets, 10) - 2.65) < 1e-9);

    assert(calculateTax(brackets, 3) == 1.5); // Exact bracket boundary.
    assert(calculateTax(brackets, 0) == 0.0); // No income.
}
