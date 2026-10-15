// LC 137 Single Number II: every element appears three times except one; find it in
// O(n) time and O(1) extra space.
// Trick: count each of the 32 bits mod 3 - the leftover bits form the single number
// Recognize: "appears k times except one" -> per-bit counts mod k
// Follow-up: ones/twos bit-state machine (a mod-3 counter per bit, no 32-bit loop).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Triples add 0 mod 3 to every bit count. T: O(32n), S: O(1).
int singleNumber(const vector<int>& nums) {
    unsigned result = 0;   // unsigned: shifting 1 into bit 31 is well-defined
    for (int bit = 0; bit < 32; bit++) {
        int cnt = 0;   // numbers with this bit set
        for (int v : nums) { cnt += ((unsigned)v >> bit) & 1; }
        if (cnt % 3 != 0) { result |= 1u << bit; }
    }
    return (int)result;
}

// follow-up: per bit, (twos, ones) counts 0 -> 1 -> 2 -> 0 occurrences mod 3.
// T: O(n), S: O(1).
int singleNumberFsm(const vector<int>& nums) {
    int ones = 0;   // bits seen 1 time mod 3
    int twos = 0;   // bits seen 2 times mod 3
    for (int v : nums) {
        ones = (ones ^ v) & ~twos;
        // uses the new ones: a bit that just left ones (3rd hit) must not enter twos
        twos = (twos ^ v) & ~ones;
    }
    return ones;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (singleNumber(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 2, 3, 2}, 3);
    test("lc ex2", {0, 1, 0, 1, 0, 1, 99}, 99);
    test("negatives", {-2, -2, 1, 1, 4, 1, 4, 4, -4, -2}, -4);
}
