// J10271 · LC 202: repeatedly replace n (positive) by the sum of the squares of its
// digits; n is happy if this reaches 1, else it loops forever.
// Follow-up: O(1) space.
#include <iostream>
#include <string>
#include <unordered_set>
using std::cout;
using std::string;
using std::unordered_set;

int digitSquareSum(int n) {
    int sum = 0;
    while (n > 0) {
        int d = n % 10;
        sum += d * d;
        n /= 10;
    }
    return sum;
}

// Remember every value; a repeat means a cycle without 1.
// T: O(k log n), S: O(k); k = steps.
bool isHappy(int n) {
    unordered_set<int> seen;   // values already produced
    while (n != 1 && seen.insert(n).second) { n = digitSquareSum(n); }
    return n == 1;
}

// follow-up: Floyd; fast moves 2 steps, slow 1, they meet inside the cycle.
// T: O(k log n), S: O(1).
bool isHappyFloyd(int n) {
    int slow = n;
    int fast = digitSquareSum(n);   // start one ahead so the loop does not exit at once
    while (fast != 1 && slow != fast) {
        slow = digitSquareSum(slow);
        fast = digitSquareSum(digitSquareSum(fast));
    }
    return fast == 1;
}

void test(const string& label, int n, bool want) {
    cout << label << (isHappy(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", 19, true);
    test("cycle", 2, false);
    test("already 1", 1, true);
}
