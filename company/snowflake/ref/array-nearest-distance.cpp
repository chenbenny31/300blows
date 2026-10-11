// array of 0 (empty) / 1 / 2, distance = index difference (Q19 A11, C09 NG round 2)
// version A: for each 1, in order, distance to its nearest 2, -1 if no 2 (J10352, LC821 pattern)
// version B: shortest distance over any (1, 2) pair, -1 if none (LC243 pattern)

// each 1's nearest 2, the global closest pair, or all pairs?
// distance = index difference? 0s don't block?
// output order: by position of the 1s?
// no 2 at all: -1 per 1? no 1: empty?
// 1D or 2D? (2D -> multi-source BFS from every 2)

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class ArrayDistance {
public:
    // version A: two passes, nearest 2 is either the last 2 on the left or the first 2 on the right
    // T: O(n), S: O(n)
    vector<int> nearestTwoPerOne(const vector<int>& a) {
        int n = static_cast<int>(a.size());
        vector<int> dist(n, -1); // index of a 1 -> distance to nearest 2 found so far, -1 = none yet
        int last = -1; // index of the closest 2 on the side already scanned
        for (int i = 0; i < n; i++) {
            if (a[i] == 2) {
                last = i;
            } else if (a[i] == 1 && last != -1) {
                dist[i] = i - last;
            }
        }

        last = -1;
        for (int i = n - 1; i >= 0; i--) {
            if (a[i] == 2) {
                last = i;
            } else if (a[i] == 1 && last != -1) {
                if (dist[i] == -1 || last - i < dist[i]) {
                    dist[i] = last - i;
                }
            }
        }

        vector<int> result; // one entry per 1, in order of appearance
        for (int i = 0; i < n; i++) {
            if (a[i] == 1) {
                result.push_back(dist[i]);
            }
        }
        return result;
    }

    // version B: one pass, the closest pair is adjacent in the 1/2 subsequence, so only the latest of each matters
    // T: O(n), S: O(1)
    int closestPair(const vector<int>& a) {
        int last1 = -1; // index of the latest 1 seen
        int last2 = -1; // index of the latest 2 seen
        int best = -1;
        for (int i = 0; i < static_cast<int>(a.size()); i++) {
            if (a[i] == 1) {
                last1 = i;
                if (last2 != -1 && (best == -1 || i - last2 < best)) {
                    best = i - last2;
                }
            } else if (a[i] == 2) {
                last2 = i;
                if (last1 != -1 && (best == -1 || i - last1 < best)) {
                    best = i - last1;
                }
            }
        }
        return best;
    }
};

int main() {
    ArrayDistance ad;
    vector<int> want;

    want = {2, 2, 3};
    assert(ad.nearestTwoPerOne({1, 0, 2, 0, 1, 1}) == want); // 2 on the right, then on the left
    want = {1};
    assert(ad.nearestTwoPerOne({2, 0, 0, 1, 2}) == want); // 2 on both sides, right one is nearer
    want = {-1, -1};
    assert(ad.nearestTwoPerOne({1, 0, 1}) == want); // no 2 at all
    assert(ad.nearestTwoPerOne({2, 0}).empty()); // no 1
    assert(ad.nearestTwoPerOne({}).empty()); // empty

    assert(ad.closestPair({1, 0, 2, 0, 1, 1}) == 2);
    assert(ad.closestPair({2, 0, 0, 1, 2}) == 1); // pair found when the 2 comes after the 1
    assert(ad.closestPair({1, 0, 1}) == -1); // no 2
    assert(ad.closestPair({}) == -1);

    std::cout << "all tests passed\n";
    return 0;
}
