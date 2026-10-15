// ByteDance / LC 239: a window of size k slides over nums one step at a time;
// return the max of each window.
#include <iostream>
#include <vector>
#include <string>
#include <deque>
using std::cout;
using std::string;
using std::vector;
using std::deque;

// Monotonic deque of indices. T: O(n), S: O(k).
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    deque<int> dq;      // indices in the window, values strictly decreasing front to back
    vector<int> result;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (!dq.empty() && dq.front() <= i - k) { dq.pop_front(); }   // left the window
        // a smaller older value can never be a max while nums[i] is in the window
        while (!dq.empty() && nums[dq.back()] <= nums[i]) { dq.pop_back(); }
        dq.push_back(i);
        if (i >= k - 1) { result.push_back(nums[dq.front()]); }
    }
    return result;
}

void test(const string& label, const vector<int>& nums, int k, const vector<int>& want) {
    cout << label << (maxSlidingWindow(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 3, -1, -3, 5, 3, 6, 7}, 3, {3, 3, 5, 5, 6, 7});
    test("single", {1}, 1, {1});
    test("max expires", {4, 2, 12, 3}, 2, {4, 12, 12});
}
