// brackets[i] = {upper_i, percent_i}, sorted by upper; income in (upper_{i-1}, upper_i] taxed at percent_i
// return total tax (LC2303, E01 warm-up)

// brackets sorted, uppers strictly increasing?
// income above the last upper: untaxed, or keep the last rate? (J10689 vs J9367 differ)
// return a double, or exact cents?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class TaxCalculator {
public:
    // walk brackets, tax the slice of income inside each, T: O(B), S: O(1)
    double calculateTax(const vector<vector<int>>& brackets, int income) {
        long long hundredths = 0; // tax * 100, kept exact until the end
        int prevUpper = 0;
        for (const auto& b : brackets) {
            if (income <= prevUpper) {
                break; // nothing left to tax
            }
            int slice = std::min(income, b[0]) - prevUpper;
            hundredths += 1LL * slice * b[1];
            prevUpper = b[0];
        }
        return hundredths / 100.0; // one division, no accumulated rounding
    }
};

int main() {
    TaxCalculator tc;

    assert(tc.calculateTax({{3, 50}, {7, 10}, {12, 25}}, 10) == 2.65); // target: 1.5 + 0.4 + 0.75
    assert(tc.calculateTax({{3, 50}, {7, 10}, {12, 25}}, 3) == 1.5); // edge: income exactly on an upper
    assert(tc.calculateTax({{3, 50}}, 0) == 0.0); // null: no income

    std::cout << "all tests passed\n";
    return 0;
}
