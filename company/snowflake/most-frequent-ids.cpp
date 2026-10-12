#include <cassert>
#include <set>
#include <unordered_map>
#include <vector>

using std::multiset;
using std::unordered_map;
using std::vector;

// T: O(nlogn), S: O(n)
vector<long long> mostFrequentIDs(const vector<int>& nums, const vector<int>& freq) {
    unordered_map<int, long long> counts; // id -> cnt
    multiset<long long> sortedFreqs;
    vector<long long> result;

    for (int i = 0; i < static_cast<int>(nums.size()); i++) {
        long long& curFreq = counts[nums[i]];

        if (curFreq > 0) {
            sortedFreqs.erase(sortedFreqs.find(curFreq)); // remove only one copy
        }

        curFreq += freq[i];
        if (curFreq > 0) {
            sortedFreqs.insert(curFreq);
        }
        result.push_back(sortedFreqs.empty() ? 0 : *sortedFreqs.rbegin());
    }
    return result;
}

int main() {
    // Tied counts; remove one ID, then empty the collection.
    vector<long long> expected = {2, 2, 2, 0};
    assert(mostFrequentIDs(
        {1, 2, 1, 2}, {2, 2, -2, -2}) == expected);

    // Updating an existing positive count replaces its old entry.
    expected = {3, 5, 1};
    assert(mostFrequentIDs({1, 1, 1}, {3, 2, -4}) == expected);

    assert(mostFrequentIDs({}, {}).empty());
}
