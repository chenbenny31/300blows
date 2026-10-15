// Apple DV (hack2hire): (1) flip or clear bit i of an unsigned x in one expression;
// (2) count the set bits of a 32-bit unsigned value without strings or builtins.
// Follow-up: one-hot check (exactly one bit set) without $onehot / builtins.
#include <iostream>
#include <string>
#include <cstdint>
using std::cout;
using std::string;
using std::uint32_t;

// XOR with the mask toggles bit i. T: O(1).
uint32_t flipBit(uint32_t x, int i) { return x ^ (1u << i); }

// AND with the inverted mask clears bit i. T: O(1).
uint32_t clearBit(uint32_t x, int i) { return x & ~(1u << i); }

// Kernighan: x & (x - 1) drops the lowest set bit. T: O(set bits) <= 32, S: O(1).
int popcount(uint32_t x) {
    int cnt = 0;
    while (x != 0) {
        x &= x - 1;
        cnt++;
    }
    return cnt;
}

// follow-up: power of two; x != 0 is needed because 0 & (0 - 1) == 0 too
bool isOneHot(uint32_t x) { return x != 0 && (x & (x - 1)) == 0; }

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    check(flipBit(0b1010, 1) == 0b1000 && flipBit(0b1000, 1) == 0b1010, "flip");
    check(clearBit(0b1010, 3) == 0b0010 && clearBit(0b0010, 3) == 0b0010, "clear");
    check(popcount(0b10110000) == 3, "popcount");
    check(popcount(0) == 0 && popcount(0xFFFFFFFFu) == 32, "popcount edges");
}
