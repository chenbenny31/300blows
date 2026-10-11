// replace n by the sum of the squares of its digits; happy if this reaches 1, else it loops forever (LC202, J10271)

// n positive?
// return only true/false?

#include <unordered_set>
#include <cassert>
#include <iostream>

class HappyNumber {
public:
    // 1: remember every value seen, a repeat means a cycle without 1, T: O(k * digits), S: O(k), k: steps
    bool isHappy_1(int n) {
        std::unordered_set<int> seen; // values already produced
        while (n != 1 && seen.insert(n).second) {
            n = next(n);
        }
        return n == 1;
    }

    // 2: Floyd, slow moves 1 step, fast moves 2; they meet inside the cycle, T: O(k * digits), S: O(1)
    bool isHappy_2(int n) {
        int slow = n;
        int fast = next(n);
        while (fast != 1 && slow != fast) {
            slow = next(slow);
            fast = next(next(fast));
        }
        return fast == 1;
    }

private:
    int next(int n) {
        int sum = 0;
        while (n > 0) {
            int d = n % 10;
            sum += d * d;
            n /= 10;
        }
        return sum;
    }
};

int main() {
    HappyNumber h;

    assert(h.isHappy_1(19)); // target: 19 -> 82 -> 68 -> 100 -> 1
    assert(h.isHappy_2(19));
    assert(!h.isHappy_1(2)); // fault: 2 enters the 4 -> 16 -> ... -> 4 cycle
    assert(!h.isHappy_2(2));
    assert(h.isHappy_1(1)); // null: already 1
    assert(h.isHappy_2(1));

    std::cout << "all tests passed\n";
    return 0;
}
