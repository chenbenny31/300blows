// LC 121: prices[i] = stock price on day i; one buy then one later sell. Return the max
// profit, 0 if no profit is possible.
// Follow-up LC 122: any number of transactions, holding at most one share at a time.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// Track the cheapest buy so far; sell today against it. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    int minPrice = prices[0];   // cheapest price on any earlier day
    int best = 0;
    for (int p : prices) {
        best = max(best, p - minPrice);
        minPrice = min(minPrice, p);
    }
    return best;
}

// follow-up: collect every upward day-to-day move. T: O(n), S: O(1).
int maxProfitMulti(const vector<int>& prices) {
    int n = prices.size();
    int total = 0;
    for (int i = 1; i < n; i++) {
        total += max(0, prices[i] - prices[i - 1]);
    }
    return total;
}

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {7, 1, 5, 3, 6, 4}, 5);
    test("falling", {7, 6, 4, 3, 1}, 0);
    test("one day", {5}, 0);
}
