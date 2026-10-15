// ByteDance (hack2hire): sqrt of a non-negative int val to `precise` decimal places,
// truncated, as a string with exactly `precise` decimals, e.g. (2, 3) -> "1.414".
#include <iostream>
#include <string>
using std::cout;
using std::string;
using std::to_string;

// Binary search the integer part, then long-hand digit by digit.
// T: O(log val + p), S: O(p).
// Exact integer math: rem <= 2r keeps every product in long long for val < 2^31, p <= 10.
string sqrtPrecise(int val, int precise) {
    long long lo = 0, hi = val;   // floor(sqrt(val)) lies in [lo, hi]
    while (lo < hi) {
        long long mid = (lo + hi + 1) / 2;
        if (mid * mid <= val) { lo = mid; } else { hi = mid - 1; }
    }
    long long r = lo;              // r = digits so far = floor(sqrt(val * 100^d))
    long long rem = val - r * r;   // rem = val * 100^d - r^2
    string result = to_string(r);
    if (precise > 0) { result += '.'; }
    for (int d = 0; d < precise; d++) {
        rem *= 100;
        // largest digit x with (10r + x)^2 <= 100 * r^2 + rem, i.e. (20r + x) * x <= rem
        int x = 9;
        while ((20 * r + x) * x > rem) { x--; }
        rem -= (20 * r + x) * x;
        r = r * 10 + x;
        result += char('0' + x);
    }
    return result;
}

void test(const string& label, int val, int precise, const string& want) {
    cout << label << (sqrtPrecise(val, precise) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("irrational", 2, 3, "1.414");
    test("perfect square", 4, 2, "2.00");
    test("zero", 0, 5, "0.00000");
    test("int max", 2147483647, 10, "46340.9500010519");
}
