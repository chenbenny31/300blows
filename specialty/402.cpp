// LC 402 Remove K Digits: num is a non-negative integer as a string; remove k digits
// so the result is the smallest number (no leading zeros, "0" if nothing is left).
// Trick: increasing stack; pop a larger digit before a smaller one while k > 0
// Recognize: smallest number/subsequence after removals -> greedy monotonic stack.
#include <iostream>
#include <string>
using std::cout;
using std::string;

// Monotonic increasing stack of digits. T: O(n), S: O(n).
string removeKdigits(const string& num, int k) {
    string stk;   // kept digits, non-decreasing
    for (char c : num) {
        // a bigger digit before a smaller one: dropping it lowers the number most
        while (k > 0 && !stk.empty() && stk.back() > c) {
            stk.pop_back();
            k--;
        }
        stk.push_back(c);
    }
    // still non-decreasing: the largest digits sit at the end
    while (k > 0 && !stk.empty()) {
        stk.pop_back();
        k--;
    }
    int start = 0;
    while (start < (int)stk.size() && stk[start] == '0') { start++; }
    string result = stk.substr(start);
    return result.empty() ? "0" : result;
}

void test(const string& label, const string& num, int k, const string& want) {
    cout << label << (removeKdigits(num, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "1432219", 3, "1219");
    test("leading zeros", "10200", 1, "200");
    test("remove all", "10", 2, "0");
}
