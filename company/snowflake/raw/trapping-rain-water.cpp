#include <algorithm>
#include <cassert>
#include <vector>

using std::vector;

// T: O(n), S: O(n)
long long trap(const vector<int>& height) {
    int left = 0;
    int right = static_cast<int>(height.size()) - 1;
    int leftMax = 0;
    int rightMax = 0;
    long long water = 0;

    while (left <= right) {
        leftMax = std::max(leftMax, height[left]);
        rightMax = std::max(rightMax, height[right]);

        if (leftMax <= rightMax) {
            water += leftMax - height[left];
            left++;
        } else {
            water += rightMax - height[right];
            right--;
        }
    }
    return water;
}

int main() {
    assert(trap({3, 0, 2, 0, 4}) == 7); // Multiple pockets: 3 + 1 + 3.
    assert(trap({2, 0, 2}) == 2); // Equal boundary walls.
    assert(trap({1, 2, 3}) == 0); // No enclosing right wall.
    assert(trap({3, 2, 1}) == 0); // No enclosing left wall.
    assert(trap({5}) == 0); // One bar.
    assert(trap({}) == 0); // Empty input.
}
