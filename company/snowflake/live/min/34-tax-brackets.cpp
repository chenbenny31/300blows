// B22 · LC 2303: brackets[i] = [upper_i, percent_i]; income in (upper_{i-1}, upper_i] taxed at percent_i; taxAboveTop: income past the top keeps the top rate. Follow-up: many queries.
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using std::cout;
using std::vector;

// walk brackets, tax the slice of income inside each, T: O(B), S: O(1)
double calculateTax(const vector<vector<int>>& brackets, int income, bool taxAboveTop) {
    int b = brackets.size();
    long long hundredths = 0;   // tax * 100, exact until the final divide
    int prevUpper = 0;
    for (int i = 0; i < b && income > prevUpper; i++) {
        hundredths += 1LL * (std::min(income, brackets[i][0]) - prevUpper) * brackets[i][1];
        prevUpper = brackets[i][0];
    }
    if (taxAboveTop && income > brackets[b - 1][0]) {
        hundredths += 1LL * (income - brackets[b - 1][0]) * brackets[b - 1][1];
    }
    return hundredths / 100.0;
}

// follow-up: prefix tax at each upper + binary search for the bracket, T: O(B) build, O(log B) per query, S: O(B)
class TaxCalculator {
public:
    TaxCalculator(const vector<vector<int>>& brackets, bool taxAboveTop) : taxAboveTop(taxAboveTop) {
        long long acc = 0;
        int prevUpper = 0;
        for (const auto& br : brackets) {
            uppers.push_back(br[0]);
            percents.push_back(br[1]);
            acc += 1LL * (br[0] - prevUpper) * br[1];
            prefix.push_back(acc);
            prevUpper = br[0];
        }
    }

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

// hundredths / 100.0 is one correctly rounded divide, so == against the literal is exact
void test(const vector<vector<int>>& brackets, int income, bool taxAboveTop, double expect) {
    double got = calculateTax(brackets, income, taxAboveTop);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void test2(const vector<vector<int>>& brackets, int income, bool taxAboveTop, double expect) {
    double got = TaxCalculator(brackets, taxAboveTop).calculateTax2(income);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    vector<vector<int>> three = {{3, 50}, {7, 10}, {12, 25}};
    vector<vector<int>> big = {{1000000000, 100}, {2000000000, 100}};
    test(three, 10, false, 2.65);                   // target example: 1.5 + 0.4 + 0.75
    test({{2, 50}}, 0, false, 0.0);                 // no income
    test(three, 20, true, 5.15);                    // above top, top rate continues
    test(big, 2000000000, false, 2000000000.0);     // slice * percent overflows int

    test2(three, 10, false, 2.65);                  // target example
    test2(three, 3, false, 1.5);                    // exactly on an upper, lower_bound picks that bracket
    test2(three, 20, false, 3.15);                  // above top, untaxed
    test2(big, INT_MAX, true, 2147483647.0);        // max income, top rate continues
}
