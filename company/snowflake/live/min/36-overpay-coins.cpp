// A10 · LC 322 prototype only: coins {1,5,10,50,100,200}, unlimited; pay n, overpay allowed, change in the same coins; minimize coins given + returned.
#include <iostream>
#include <algorithm>
#include <climits>
using std::cout;

// largest coin first (divisibility chain -> greedy optimal), T: O(K), S: O(1)
int greedyCount(int amount) {
    int coins[] = {200, 100, 50, 10, 5, 1};   // denominations, largest first
    int count = 0;
    for (int c : coins) {
        count += amount / c;
        amount %= c;
    }
    return count;
}

// try every change amount below 200, greedy count on both sides, T: O(200 * K), S: O(1)
int minTotalCoins(int n) {
    int best = INT_MAX;
    for (int change = 0; change < 200; change++) {   // change >= 200: drop one 200 from both sides, 2 coins fewer
        best = std::min(best, greedyCount(n + change) + greedyCount(change));
    }
    return best;
}

void test(int n, int expect) {
    int got = minTotalCoins(n);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    test(48, 3);                     // target: pay 50, get 1+1; exact greedy is 8
    test(0, 0);                      // nothing to pay
    test(200, 1);                    // change = 0 must be tried
    test(145, 3);                    // pay 100+50, get 5
    test(1000000000, 5000000);       // large n, all 200s
}
