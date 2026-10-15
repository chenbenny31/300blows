// A10 (LC 322 is only a prototype): coins {1,5,10,50,100,200}, unlimited; pay n, overpaying allowed and change returned
// in the same coins; minimize coins given + coins returned.

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

class OverpayCoins {
public:
    // try every change amount below 200, greedy count on both sides, T: O(200 * K), S: O(1), K = 6 coin kinds
    int minTotalCoins(int n) {
        int best = INT_MAX;
        for (int change = 0; change < 200; change++) {   // change >= 200: drop one 200 from both sides, 2 coins fewer
            best = std::min(best, greedyCount(n + change) + greedyCount(change));
        }
        return best;
    }

    // same search over a wider change range, used to check the 200 bound, T: O(maxChange * K), S: O(1)
    int minTotalCoins2(int n, int maxChange) {
        int best = INT_MAX;
        for (int change = 0; change <= maxChange; change++) {
            best = std::min(best, greedyCount(n + change) + greedyCount(change));
        }
        return best;
    }

    // largest coin first, T: O(K), S: O(1)
    int greedyCount(int amount) {
        int count = 0;
        for (int c : coins) {
            count += amount / c;
            amount %= c;
        }
        return count;
    }

private:
    vector<int> coins = {200, 100, 50, 10, 5, 1};   // denominations, largest first
};

bool test(int n, int expected) {
    OverpayCoins oc;
    int got = oc.minTotalCoins(n);
    int got2 = oc.minTotalCoins2(n, 1000);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL n=" << n << ": got " << got << " / " << got2 << ", expected " << expected << "\n";
    return false;
}

bool testAgainstDp(int maxN, int maxChange) {
    OverpayCoins oc;
    vector<int> denoms = {1, 5, 10, 50, 100, 200};
    int top = maxN + maxChange;
    vector<int> dp(top + 1, INT_MAX);   // amount -> min coins to make it exactly
    dp[0] = 0;
    for (int a = 1; a <= top; a++) {
        for (int c : denoms) {
            if (c <= a && dp[a - c] != INT_MAX) { dp[a] = std::min(dp[a], dp[a - c] + 1); }
        }
    }
    for (int a = 0; a <= top; a++) {
        if (dp[a] != oc.greedyCount(a)) {
            cout << "FAIL greedy not optimal at " << a << ": dp " << dp[a] << ", greedy " << oc.greedyCount(a) << "\n";
            return false;
        }
    }
    for (int n = 0; n <= maxN; n++) {
        int brute = INT_MAX;
        for (int change = 0; change <= maxChange; change++) { brute = std::min(brute, dp[n + change] + dp[change]); }
        if (oc.minTotalCoins(n) != brute) {
            cout << "FAIL dp search n=" << n << ": got " << oc.minTotalCoins(n) << ", expected " << brute << "\n";
            return false;
        }
    }
    return true;
}

int main() {
    bool ok = true;
    ok &= test(0, 0);                    // nothing to pay
    ok &= test(1, 1);                    // smallest positive
    ok &= test(3, 3);                    // overpaying ties, never beats exact
    ok &= test(4, 2);                    // pay 5, get 1
    ok &= test(8, 3);                    // pay 10, get 1+1
    ok &= test(48, 3);                   // pay 50, get 1+1; exact greedy is 8
    ok &= test(94, 3);                   // pay 100, get 5+1
    ok &= test(140, 3);                  // pay 100+50 get 10, or 200 get 50+10
    ok &= test(145, 3);                  // pay 100+50, get 5
    ok &= test(199, 2);                  // pay 200, get 1
    ok &= test(200, 1);                  // single largest coin
    ok &= test(399, 3);                  // pay 200+200, get 1
    ok &= test(999, 6);                  // pay 5x200, get 1
    ok &= test(1000000000, 5000000);     // large n, all 200s
    ok &= testAgainstDp(600, 1500);      // greedy == DP up to 2100, answer == DP search
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
