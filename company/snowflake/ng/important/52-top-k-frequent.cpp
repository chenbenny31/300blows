// LC tag · LC 347: return the k most frequent elements of nums, in any order.
// The answer is unique. Follow-up: better than O(n log n).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::priority_queue;
using std::greater;
using std::sort;

// Count, then keep a size-k min-heap of (count, value). T: O(n log k), S: O(n).
vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;   // freq[value] = occurrences in nums
    for (int x : nums) { freq[x]++; }
    // min-heap of (count, value): top = least frequent kept candidate
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (const auto& [val, cnt] : freq) {
        heap.push({cnt, val});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// follow-up: bucket by count (count <= n), scan buckets high to low. T: O(n), S: O(n).
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;   // freq[value] = occurrences in nums
    for (int x : nums) { freq[x]++; }
    int n = nums.size();
    vector<vector<int>> buckets(n + 1);   // buckets[c] = values occurring exactly c times
    for (const auto& [val, cnt] : freq) { buckets[cnt].push_back(val); }
    vector<int> result;
    for (int c = n; c >= 1; c--) {
        for (int val : buckets[c]) {
            result.push_back(val);
            if ((int)result.size() == k) { return result; }
        }
    }
    return result;
}

void test(const string& label, const vector<int>& nums, int k, vector<int> want) {
    vector<int> got = topKFrequent(nums, k);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 1, 1, 2, 2, 3}, 2, {1, 2});
    test("single", {1}, 1, {1});
    test("negatives", {4, -1, -1, 4, 4, 2}, 1, {4});
}
