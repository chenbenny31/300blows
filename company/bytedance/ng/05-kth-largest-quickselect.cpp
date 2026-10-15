// ByteDance (hack2hire) / LC 215: return the k-th largest element of nums (k-th in
// sorted order, duplicates count). No heap and no full sort.
// Follow-up: size-k min-heap version.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cstdlib>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::rand;
using std::swap;

// Randomized quickselect, 3-way partition so duplicates never stall it.
// T: O(n) average, O(n^2) worst, S: O(1).
int findKthLargest(vector<int> nums, int k) {
    int n = nums.size();
    int target = n - k;   // index of the answer in ascending order
    int lo = 0, hi = n - 1;
    while (true) {
        int pivot = nums[lo + rand() % (hi - lo + 1)];
        // [lo, lt) < pivot, [lt, i) == pivot, (gt, hi] > pivot
        int lt = lo, i = lo, gt = hi;
        while (i <= gt) {
            if (nums[i] < pivot) {
                swap(nums[lt++], nums[i++]);
            } else if (nums[i] > pivot) {
                // i stays: the value swapped in from gt is not classified yet
                swap(nums[i], nums[gt--]);
            } else {
                i++;
            }
        }
        if (target < lt) {
            hi = lt - 1;
        } else if (target > gt) {
            lo = gt + 1;
        } else {
            return pivot;
        }
    }
}

// follow-up: keep the k largest seen in a min-heap; its top is the answer.
// T: O(n log k), S: O(k).
int findKthLargestHeap(const vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> heap;   // k largest so far, min on top
    for (int x : nums) {
        heap.push(x);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    return heap.top();
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (findKthLargest(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {3, 2, 1, 5, 6, 4}, 2, 5);
    test("lc ex2", {3, 2, 3, 1, 2, 4, 5, 5, 6}, 4, 4);
    test("all equal", {7, 7, 7, 7}, 2, 7);
}
