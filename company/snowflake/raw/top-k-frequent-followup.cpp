#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

using std::unordered_map;
using std::vector;

// T: O(n), S: O(n)
vector<int> topKFrequent(const vector<int>& nums, int k) {
    if (k == 0) { return {}; }
    unordered_map<int, int> freq; // val -> cnt
    for (int num : nums) {
        freq[num]++;
    }

    int n = static_cast<int>(nums.size());
    vector<vector<int>> buckets(n + 1); // cnt -> [val,] with that cnt
    for (const auto& [val, cnt] : freq) {
        buckets[cnt].push_back(val);
    }

    vector<int> result;
    for (int cnt = n; cnt >= 1; cnt--) {
        for (int val : buckets[cnt]) {
            result.push_back(val);
            if (static_cast<int>(result.size()) == k) {
                return result;
            }
        }
    }
    return result;
}


int main() {
    auto result = topKFrequent({1, 1, 1, 2, 2, 3}, 2);
    std::sort(result.begin(), result.end());
    vector<int> expected = {1, 2};
    assert(result == expected); // Different frequencies.

    result = topKFrequent({-1, -1, 4, 4}, 1);
    assert(result.size() == 1 && (result[0] == -1 || result[0] == 4));
    // Tie: either value is valid, but return exactly k elements.

    assert(topKFrequent({1}, 0).empty());
    assert(topKFrequent({}, 0).empty());
}
