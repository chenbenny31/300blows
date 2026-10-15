// B22 / LC 2303: brackets[i] = [upper_i, percent_i], uppers increasing; income in (upper_{i-1}, upper_i] is taxed at percent_i.
// Configurable: income above the top upper is either untaxed or taxed at the top rate.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class TaxCalculator {
public:
    TaxCalculator(const vector<vector<int>>& brackets, bool taxAboveTop) : taxAboveTop(taxAboveTop) {
        long long acc = 0;
        int prevUpper = 0;
        for (const auto& b : brackets) {
            uppers.push_back(b[0]);
            percents.push_back(b[1]);
            acc += 1LL * (b[0] - prevUpper) * b[1];
            prefix.push_back(acc);
            prevUpper = b[0];
        }
    }

    // walk brackets, tax the slice of income inside each, T: O(B), S: O(1)
    double calculateTax(int income) {
        int b = uppers.size();
        long long hundredths = 0;   // tax * 100, exact until the final divide
        int prevUpper = 0;
        for (int i = 0; i < b && income > prevUpper; i++) {
            hundredths += 1LL * (std::min(income, uppers[i]) - prevUpper) * percents[i];
            prevUpper = uppers[i];
        }
        if (taxAboveTop && income > uppers[b - 1]) {
            hundredths += 1LL * (income - uppers[b - 1]) * percents[b - 1];
        }
        return hundredths / 100.0;
    }

    // prefix tax at each upper + binary search for the bracket, T: O(log B) per query, S: O(B) built once
    double calculateTax2(int income) {
        int b = uppers.size();
        int idx = std::lower_bound(uppers.begin(), uppers.end(), income) - uppers.begin();   // first upper >= income
        if (idx == b) {
            long long extra = taxAboveTop ? 1LL * (income - uppers[b - 1]) * percents[b - 1] : 0;
            return (prefix[b - 1] + extra) / 100.0;
        }
        long long below = idx == 0 ? 0 : prefix[idx - 1];
        int prevUpper = idx == 0 ? 0 : uppers[idx - 1];
        return (below + 1LL * (income - prevUpper) * percents[idx]) / 100.0;
    }

private:
    bool taxAboveTop;           // true: income past the top upper keeps the top rate
    vector<int> uppers;         // bracket i -> its upper bound
    vector<int> percents;       // bracket i -> its rate in percent
    vector<long long> prefix;   // bracket i -> tax * 100 owed on income exactly uppers[i]
};

bool test(const string& label, const vector<vector<int>>& brackets, bool taxAboveTop, int income, double expected) {
    TaxCalculator tc(brackets, taxAboveTop);
    double got = tc.calculateTax(income);
    double got2 = tc.calculateTax2(income);
    double eps = 1e-9;
    bool pass = got - expected < eps && expected - got < eps && got2 - expected < eps && expected - got2 < eps;
    if (pass) { return true; }
    cout << "FAIL " << label << " income " << income << ": got " << got << " / " << got2 << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    vector<vector<int>> three = {{3, 50}, {7, 10}, {12, 25}};
    vector<vector<int>> big = {{1000000000, 100}, {2000000000, 100}};
    ok &= test("lc1", three, false, 10, 2.65);                   // LC example 1: 1.5 + 0.4 + 0.75
    ok &= test("lc2", {{1, 0}, {4, 25}, {5, 50}}, false, 2, 0.25);   // LC example 2: zero-rate first bracket
    ok &= test("lc3", {{2, 50}}, false, 0, 0.0);                 // LC example 3: no income
    ok &= test("first slice", three, false, 1, 0.5);             // inside the first bracket
    ok &= test("on upper", three, false, 3, 1.5);                // exactly on a boundary
    ok &= test("past upper", three, false, 4, 1.6);              // one past a boundary
    ok &= test("top", three, true, 12, 3.15);                    // exactly the top upper, flag irrelevant
    ok &= test("above ignore", three, false, 20, 3.15);          // above top, untaxed
    ok &= test("above extend", three, true, 20, 5.15);           // above top, top rate continues
    ok &= test("big", big, false, 2000000000, 2000000000.0);     // slice * percent overflows int
    ok &= test("big extend", big, true, INT_MAX, 2147483647.0);  // max income, top rate continues
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
