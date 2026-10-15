// LC 384 Shuffle an Array: shuffle() returns a uniformly random permutation of nums;
// reset() returns the original array.
// Trick: Fisher-Yates - for i from n-1 down, swap a[i] with a[rand() % (i+1)]
// Recognize: uniform random permutation in O(n), each of the n! orders equally likely
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::rand;
using std::swap;
using std::sort;

class Solution {
    vector<int> original;   // input as given, returned by reset

public:
    Solution(const vector<int>& nums) : original(nums) {}

    // T: O(1) plus the O(n) copy.
    vector<int> reset() { return original; }

    // Slot i gets a uniform pick from the i+1 not-yet-placed values. T: O(n), S: O(n).
    vector<int> shuffle() {
        vector<int> a = original;   // a[i+1..n-1] are already final
        for (int i = (int)a.size() - 1; i > 0; i--) {
            // j may equal i: the value may stay put
            swap(a[i], a[rand() % (i + 1)]);
        }
        return a;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Solution s({1, 2, 3});
    vector<int> shuffled = s.shuffle();
    sort(shuffled.begin(), shuffled.end());
    check(shuffled == vector<int>{1, 2, 3}, "permutation");
    check(s.reset() == vector<int>{1, 2, 3}, "reset");
}
