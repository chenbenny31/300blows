#include <algorithm>
#include <cassert>
#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using std::greater;
using std::pair;
using std::priority_queue;
using std::unordered_map;
using std::vector;

// T: O(nlog(k)), S: O(n)
vector<int> topKFrequent(const vector<int>& nums, int k) {
    if (k == 0) { return {}; }
    unordered_map<int, int> freq; // val -> occurrence
    for (int num : nums) {
        freq[num]++;
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap; // {cnt, val}

    for (const auto& [val, cnt] : freq) {
        minHeap.push({cnt, val});
        if (static_cast<int>(minHeap.size()) > k) {
            minHeap.pop();
        }
    }

    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }
    return result;
}

int main() {
    auto result = topKFrequent({1, 1, 1, 2, 2, 3}, 2);
    std::sort(result.begin(), result.end());
    vector<int> expected = {1, 2};
    assert(result == expected); // Select two highest frequencies.

    expected = {-1};
    assert(topKFrequent({-1, -1, 4}, 1) == expected); // Negative value.

    assert(topKFrequent({1, 1}, 0).empty()); // No results requested.
    assert(topKFrequent({}, 0).empty());
}
