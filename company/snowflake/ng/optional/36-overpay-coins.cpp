// A10: coins {1,5,10,50,100,200}, unlimited on both sides. Pay n; overpaying is
// allowed and change comes back in the same coins. Minimize coins given + returned.
#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::string;
using std::min;

// Largest coin first; optimal here since each coin divides the next. T: O(K), K = 6.
int greedy(int amount) {
    int coins[] = {200, 100, 50, 10, 5, 1};   // denominations, largest first
    int count = 0;
    for (int c : coins) {
        count += amount / c;
        amount %= c;
    }
    return count;
}

// Paid and change are independent, so try every change c. T: O(200 * K), S: O(1).
int minTotalCoins(int n) {
    int best = INT_MAX;
    // c >= 200: both sides hold a 200 coin; drop one from each, 2 coins fewer
    for (int c = 0; c < 200; c++) { best = min(best, greedy(n + c) + greedy(c)); }
    return best;
}

void test(const string& label, int n, int want) {
    cout << label << (minTotalCoins(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("exact", 200, 1);
    test("pay 50 get 1+1", 48, 3);
    test("pay 200 get 1", 199, 2);
    test("many 200s", 999, 6);
}
