// step i adds freq[i] copies of id nums[i] (negative freq removes copies)
// after every step, report the count of the most frequent id, 0 if the collection is empty (LC3092)

// freq can be negative? a count never drops below 0?
// return the count, not the id?
// empty collection -> 0?
// counts beyond int?

#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class FrequencyTracker {
public:
    // 1: update the count, rescan every id for the max, T: O(n*U), U: distinct ids, S: O(U)
    vector<long long> mostFrequentIDs_1(const vector<int>& nums, const vector<int>& freq) {
        std::unordered_map<int, long long> count; // id -> current copies
        vector<long long> ans;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            count[nums[i]] += freq[i];
            long long best = 0;
            for (const auto& kv : count) {
                best = std::max(best, kv.second);
            }
            ans.push_back(best);
        }
        return ans;
    }

    // 2: also keep every positive count in a multiset, max = last element, T: O(n logn), S: O(U)
    vector<long long> mostFrequentIDs_2(const vector<int>& nums, const vector<int>& freq) {
        std::unordered_map<int, long long> count; // id -> current copies
        std::multiset<long long> counts; // current count of each id with count > 0
        vector<long long> ans;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            long long& c = count[nums[i]];
            if (c > 0) {
                counts.erase(counts.find(c)); // one copy only; erase(c) would drop every id with that count
            }
            c += freq[i];
            if (c > 0) {
                counts.insert(c);
            }
            ans.push_back(counts.empty() ? 0 : *counts.rbegin());
        }
        return ans;
    }
};

int main() {
    FrequencyTracker ft;
    vector<long long> want;

    want = {3, 3, 2, 2};
    assert(ft.mostFrequentIDs_1({2, 3, 2, 1}, {3, 2, -3, 1}) == want); // target: LC example
    assert(ft.mostFrequentIDs_2({2, 3, 2, 1}, {3, 2, -3, 1}) == want);

    want = {2, 0};
    assert(ft.mostFrequentIDs_1({5, 5}, {2, -2}) == want); // edge: collection becomes empty
    assert(ft.mostFrequentIDs_2({5, 5}, {2, -2}) == want);

    want = {1, 1, 1};
    assert(ft.mostFrequentIDs_1({1, 2, 1}, {1, 1, -1}) == want); // fault: two ids share a count, remove one
    assert(ft.mostFrequentIDs_2({1, 2, 1}, {1, 1, -1}) == want);

    assert(ft.mostFrequentIDs_1({}, {}).empty()); // null
    assert(ft.mostFrequentIDs_2({}, {}).empty());

    std::cout << "all tests passed\n";
    return 0;
}
