// LC 135 Candy: each child gets >= 1 candy; a child rated higher than a neighbor gets
// more than that neighbor. Return the minimum total candies.
// Trick: two passes: left-to-right for rising slopes, right-to-left taking the max
// Recognize: each element constrained by both neighbors -> one pass per direction.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Each pass satisfies one side minimally; max keeps the left constraint while
// adding the right one. T: O(n), S: O(n).
int candy(const vector<int>& ratings) {
    int n = ratings.size();
    vector<int> candies(n, 1);   // candies[i] = min candies for child i so far
    for (int i = 1; i < n; i++) {
        if (ratings[i] > ratings[i - 1]) { candies[i] = candies[i - 1] + 1; }
    }
    for (int i = n - 2; i >= 0; i--) {
        // max, not assign: don't break the left-neighbor rule from pass 1
        if (ratings[i] > ratings[i + 1]) {
            candies[i] = max(candies[i], candies[i + 1] + 1);
        }
    }
    int total = 0;
    for (int c : candies) { total += c; }
    return total;
}

void test(const string& label, const vector<int>& ratings, int want) {
    cout << label << (candy(ratings) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 0, 2}, 5);
    test("equal neighbors", {1, 2, 2}, 4);
    test("peak", {1, 3, 2, 2, 1}, 7);
}
