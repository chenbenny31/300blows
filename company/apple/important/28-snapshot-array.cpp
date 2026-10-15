// LC 1146: SnapshotArray(length) of zeros; set(index, val); snap() returns the snap id
// (0, 1, ...); get(index, snapId) returns the value at index when that snap was taken.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

class SnapshotArray {
    int curSnap = 0;   // id the next snap() returns
    // history[i] = (snapId, val) changes of index i, snapIds strictly increasing
    vector<vector<pair<int, int>>> history;

public:
    // every index starts as 0 from snap 0. S: O(length + sets).
    SnapshotArray(int length) : history(length, vector<pair<int, int>>{{0, 0}}) {}

    // Overwrite if this index already changed since the last snap. T: O(1).
    void set(int index, int val) {
        auto& h = history[index];
        if (h.back().first == curSnap) {
            h.back().second = val;
        } else {
            h.push_back({curSnap, val});
        }
    }

    // T: O(1).
    int snap() { return curSnap++; }

    // Binary search for the last change at or before snapId. T: O(log sets).
    int get(int index, int snapId) {
        const auto& h = history[index];
        int lo = 0, hi = h.size();   // first index with snap > snapId is in [lo, hi]
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (h[mid].first <= snapId) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        // lo >= 1 always: h[0] is snap 0
        return h[lo - 1].second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SnapshotArray a(3);
    a.set(0, 5);
    check(a.snap() == 0, "snap id");
    a.set(0, 6);
    check(a.get(0, 0) == 5, "lc ex1");
    a.set(0, 7);
    check(a.snap() == 1 && a.get(0, 1) == 7, "overwrite before snap");
    a.snap();
    check(a.get(0, 2) == 7 && a.get(1, 2) == 0, "unchanged, untouched");
}
