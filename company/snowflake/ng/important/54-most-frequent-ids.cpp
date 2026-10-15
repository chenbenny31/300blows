// LC tag · LC 3092: step i adds freq[i] copies of id nums[i] (negative freq removes
// copies; a count never drops below 0). After each step, report the count of the most
// frequent id, 0 if the collection is empty.
// Follow-up: without an ordered set.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::multiset;
using std::priority_queue;

// Multiset of positive counts; max = last element. T: O(n log n), S: O(n).
vector<long long> mostFrequentIDs(const vector<int>& nums, const vector<int>& freq) {
    unordered_map<int, long long> count;   // count[id] = current copies of id
    multiset<long long> counts;            // count of every id with count > 0
    vector<long long> result;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        long long& c = count[nums[i]];
        // erase(find(c)), not erase(c): other ids may share this count
        if (c > 0) { counts.erase(counts.find(c)); }
        c += freq[i];
        if (c > 0) { counts.insert(c); }
        result.push_back(counts.empty() ? 0 : *counts.rbegin());
    }
    return result;
}

// follow-up: max-heap of (count, id) snapshots; pop stale tops lazily.
// T: O(n log n), S: O(n).
vector<long long> mostFrequentIDsHeap(const vector<int>& nums, const vector<int>& freq) {
    unordered_map<int, long long> count;     // count[id] = current copies of id
    priority_queue<pair<long long, int>> heap;   // (count at push time, id)
    vector<long long> result;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        long long& c = count[nums[i]];
        c += freq[i];
        if (c > 0) { heap.push({c, nums[i]}); }
        // a top is stale once its id's count has changed since the push
        while (!heap.empty() && heap.top().first != count[heap.top().second]) {
            heap.pop();
        }
        result.push_back(heap.empty() ? 0 : heap.top().first);
    }
    return result;
}

void test(const string& label, const vector<int>& nums, const vector<int>& freq,
          const vector<long long>& want) {
    cout << label << (mostFrequentIDs(nums, freq) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 3, 2, 1}, {3, 2, -3, 1}, {3, 3, 2, 2});
    test("shared count", {1, 2, 1, 2}, {2, 2, -2, -2}, {2, 2, 2, 0});
    test("empty", {}, {}, {});
}
