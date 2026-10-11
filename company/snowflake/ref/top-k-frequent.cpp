// return the k most frequent values in nums, any order (LC347)

// ties at the k-th frequency: any of them, or guaranteed unique?
// output order?
// k <= number of distinct values?
// stream with a time window later? (needs eviction, see follow-ups)

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <functional>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::pair;

class FrequencyRanker {
public:
    // count + min-heap of size k, top is the weakest of the current best k
    // T: O(n logk), S: O(n)
    vector<int> topKFrequent_1(const vector<int>& nums, int k) {
        std::unordered_map<int, int> freq; // value -> count
        for (int x : nums) {
            freq[x]++;
        }
        std::priority_queue<pair<int, int>, vector<pair<int, int>>, std::greater<pair<int, int>>> minHeap; // (count, value)
        for (const auto& kv : freq) {
            minHeap.push({kv.second, kv.first});
            if (static_cast<int>(minHeap.size()) > k) {
                minHeap.pop(); // drop the least frequent, k best remain
            }
        }
        vector<int> result;
        while (!minHeap.empty()) {
            result.push_back(minHeap.top().second);
            minHeap.pop();
        }
        return result;
    }

    // bucket by count, a count is at most n, T: O(n), S: O(n)
    vector<int> topKFrequent_2(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        std::unordered_map<int, int> freq; // value -> count
        for (int x : nums) {
            freq[x]++;
        }
        vector<vector<int>> buckets(n + 1); // count -> values with that count
        for (const auto& kv : freq) {
            buckets[kv.second].push_back(kv.first);
        }
        vector<int> result;
        for (int c = n; c >= 1; c--) {
            for (int v : buckets[c]) {
                result.push_back(v);
                if (static_cast<int>(result.size()) == k) {
                    return result;
                }
            }
        }
        return result;
    }
};

int main() {
    FrequencyRanker fr;
    vector<int> want;
    vector<int> got;

    want = {1, 2};
    got = fr.topKFrequent_1({1, 1, 1, 2, 2, 3}, 2);
    std::sort(got.begin(), got.end()); // any order is valid
    assert(got == want); // target
    got = fr.topKFrequent_2({1, 1, 1, 2, 2, 3}, 2);
    std::sort(got.begin(), got.end());
    assert(got == want);

    want = {-1};
    assert(fr.topKFrequent_1({-1, -1, 4}, 1) == want); // edge: negative values, k = 1
    assert(fr.topKFrequent_2({-1, -1, 4}, 1) == want);

    assert(fr.topKFrequent_1({}, 0).empty()); // null: empty input
    assert(fr.topKFrequent_2({}, 0).empty());

    std::cout << "all tests passed\n";
    return 0;
}
