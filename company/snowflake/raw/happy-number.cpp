#include <cassert>

// T: O(logn), S: O(1)
int nextValue(int n) {
    int sum = 0;
    while (n > 0) {
        int d = n % 10;
        sum += d * d;
        n /= 10;
    }
    return sum;
}

bool isHappy(int n) {
    int slow = n;
    int fast = nextValue(n);

    while (fast != 1 && slow != fast) {
        slow = nextValue(slow);
        fast = nextValue(nextValue(fast));
    }
    return fast == 1;
}

int main() {
    assert(isHappy(19)); // 19 -> 82 -> 68 -> 100 -> 1.
    assert(!isHappy(2)); // Enters a cycle that excludes 1.
    assert(isHappy(1)); // Already happy.
}
