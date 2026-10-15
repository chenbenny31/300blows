#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// two scans, T: O(n), S: O(n)
vector<int> nearestTwo(const vector<int>& nums) {
    int n = nums.size();
    vector<int> dist(n, 0); // dist[i] = dist from 1 at i to nearest 2 so far, 0 means none
    int last = -1; 
    for (int i = 0; i < n; i++) {
        if (nums[i] == 2) { last = i; }
        if (nums[i] == 1 && last != -1) { dist[i] = i - last; }
    }

    last = -1;
    for (int i = n - 1; i >= 0; i--) {
        if (nums[i] == 2) { last = i; }
        if (nums[i] == 1 && last != -1 && (dist[i] == 0 || last - i < dist[i])) {
            dist[i] = last - i;
        }
    }

    vector<int> result;
    for (int i = 0; i < n; i++) {
        if (nums[i] == 1) {
            result.push_back(dist[i] == 0 ? -1 : dist[i]);
        }
    }
    return result;
}

// follow-up: one scan, two states track last1 and last2, T: O(n), S: O(1)
int closestPair(const vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int last1 = -1;
    int last2 = -1;
    int minDist = 0;
    for (int i = 0; i < n; i++) {
        if (nums[i] == 1) {
            last1 = i;
            if (last2 != -1 && (minDist == 0 || i - last2 < minDist)) {
                minDist = i - last2;
            }
        }
        if (nums[i] == 2) {
            last2 = i;
            if (last1 != -1 && (minDist == 0 || i - last1 < minDist)) {
                minDist = i - last1;
            }
        }
    }
    return minDist == 0 ? -1 : minDist;
}

void test(const string& label,
          const vector<int>& nums, const vector<int>& want) {
    cout << label
         << (nearestTwo(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {1, 0, 2, 1, 0, 0, 1, 2}, {2, 1, 1});
    test("no 2", {1, 0, 1}, {-1, -1});
    test("no 1", {2, 0, 2}, {});
}
