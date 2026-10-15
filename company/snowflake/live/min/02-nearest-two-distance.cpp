// A08 · LC 821 variant: for each 1 (in order), the index distance to its nearest 2;
// -1 if there is no 2. Follow-up B25: smallest distance between any 1 and any 2.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Two scans: nearest 2 on the left, then on the right. T: O(n), S: O(n).
vector<int> nearestTwo(const vector<int>& nums) {
    int n = nums.size();
    // dist[i] = distance from the 1 at i to the nearest 2 so far; 0 = none yet
    // (0 is safe: a 1 and a 2 never share an index)
    vector<int> dist(n, 0);
    int last = -1;   // index of the closest 2 seen in the current scan direction
    for (int i = 0; i < n; i++) {
        if (nums[i] == 2) { last = i; }
        if (nums[i] == 1 && last != -1) { dist[i] = i - last; }
    }
    last = -1;
    for (int i = n - 1; i >= 0; i--) {
        if (nums[i] == 2) { last = i; }
        // keep the left answer unless the right 2 is strictly closer
        if (nums[i] == 1 && last != -1 && (dist[i] == 0 || last - i < dist[i])) {
            dist[i] = last - i;
        }
    }
    vector<int> result;
    for (int i = 0; i < n; i++) {
        if (nums[i] == 1) { result.push_back(dist[i] == 0 ? -1 : dist[i]); }
    }
    return result;
}

// follow-up: one scan. The best pair is some element and the latest element
// of the other kind before it. T: O(n), S: O(1).
int closestPair(const vector<int>& nums) {
    int n = nums.size();
    int last1 = -1, last2 = -1;   // index of the latest 1 / latest 2
    int best = 0;                 // smallest 1-2 distance so far; 0 = none yet
    for (int i = 0; i < n; i++) {
        if (nums[i] == 1) {
            last1 = i;
            if (last2 != -1 && (best == 0 || i - last2 < best)) { best = i - last2; }
        }
        if (nums[i] == 2) {
            last2 = i;
            if (last1 != -1 && (best == 0 || i - last1 < best)) { best = i - last1; }
        }
    }
    return best == 0 ? -1 : best;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    cout << label << (nearestTwo(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {1, 0, 2, 0, 0, 1, 0, 2, 1, 0, 0, 2}, {2, 2, 1});
    test("no 2", {1, 0, 1}, {-1, -1});
    test("no 1", {2, 0, 2}, {});
}
