// LC 1049: smash two stones x <= y; x is destroyed, y becomes y - x (both go if equal).
// Return the smallest possible weight of the last stone, 0 if none is left.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Split into two groups with sums as close as possible: 0/1 subset sum up to total / 2,
// 1D array filled backwards. T: O(n*S), S: O(S); S = total weight.
int lastStoneWeightII(const vector<int>& stones) {
    int total = 0;
    for (int s : stones) { total += s; }
    int half = total / 2;
    vector<bool> can(half + 1, false);   // can[w] = some subset weighs exactly w
    can[0] = true;
    for (int s : stones) {
        // backwards so each stone is used at most once
        for (int w = half; w >= s; w--) {
            if (can[w - s]) { can[w] = true; }
        }
    }
    int best = 0;   // heaviest reachable group weight <= half
    for (int w = 0; w <= half; w++) {
        if (can[w]) { best = w; }
    }
    return total - 2 * best;
}

void test(const string& label, const vector<int>& stones, int want) {
    cout << label << (lastStoneWeightII(stones) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 7, 4, 1, 8, 1}, 1);
    test("lc ex2", {31, 26, 33, 21, 40}, 5);
    test("even split", {1, 2, 3}, 0);
    test("one stone", {5}, 5);
}
