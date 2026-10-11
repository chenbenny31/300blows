// static sorted array, answer many lookups (J10556 NG; LC704 / 34 / 35)
// search: index of target or -1; searchRange: first and last index; searchInsert: where target would go
// follow-up: many repeated queries -> prebuilt value -> first index map, O(1) average per query

// duplicates allowed? which index to return, any or leftmost?
// absent target: -1 or insert position?
// array static, or inserts/deletes between queries?
// how many queries vs n? repeated or clustered values?
// int range, target at INT_MAX?

#include <vector>
#include <unordered_map>
#include <climits>
#include <cassert>
#include <iostream>

using std::vector;

class SortedSearcher {
private:
    vector<int> a; // sorted values
    int n;
    std::unordered_map<int, int> firstIndex; // value -> leftmost index

public:
    // T: O(n) build, S: O(n) copy + map
    SortedSearcher(const vector<int>& sorted) : a(sorted), n(static_cast<int>(sorted.size())) {
        for (int i = 0; i < n; i++) {
            firstIndex.emplace(a[i], i); // emplace keeps the first index of a duplicate run
        }
    }

    // LC704, leftmost index on duplicates, T: O(logn)
    int search(int target) const {
        int i = lowerBound(target);
        return (i < n && a[i] == target) ? i : -1;
    }

    // LC34, T: O(logn)
    vector<int> searchRange(int target) const {
        int first = lowerBound(target);
        if (first == n || a[first] != target) {
            return {-1, -1};
        }
        int last = lowerBound(1LL * target + 1) - 1; // widen first, target + 1 overflows at INT_MAX
        return {first, last};
    }

    // LC35, T: O(logn)
    int searchInsert(int target) const {
        return lowerBound(target);
    }

    // follow-up: repeated queries on a static array, T: O(1) average
    int searchCached(int target) const {
        auto it = firstIndex.find(target); // find, not [], so a miss doesn't insert
        return it == firstIndex.end() ? -1 : it->second;
    }

private:
    // first index with a[i] >= target, n if none; answer always stays in [lo, hi]
    int lowerBound(long long target) const {
        int lo = 0;
        int hi = n;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] < target) {
                lo = mid + 1; // mid and everything left of it are too small
            } else {
                hi = mid; // mid could be the answer, keep it
            }
        }
        return lo;
    }
};

int main() {
    SortedSearcher s({-1, 0, 3, 5, 5, 5, 9, 12});
    vector<int> want;

    assert(s.search(9) == 6);
    assert(s.search(2) == -1); // absent, between two values
    assert(s.search(5) == 3); // duplicates: leftmost
    want = {3, 5};
    assert(s.searchRange(5) == want);
    want = {-1, -1};
    assert(s.searchRange(4) == want);
    assert(s.searchInsert(4) == 3);
    assert(s.searchInsert(-5) == 0); // before everything
    assert(s.searchInsert(13) == 8); // past the end
    assert(s.searchCached(5) == 3);
    assert(s.searchCached(2) == -1);

    SortedSearcher empty({});
    assert(empty.search(1) == -1); // empty array
    assert(empty.searchInsert(1) == 0);

    SortedSearcher top({INT_MAX, INT_MAX});
    want = {0, 1};
    assert(top.searchRange(INT_MAX) == want); // target + 1 would overflow as int

    std::cout << "all tests passed\n";
    return 0;
}
