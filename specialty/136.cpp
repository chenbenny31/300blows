// LC 136 Single Number: every element appears twice except one; find it in O(n) time
// and O(1) extra space.
// Trick: XOR everything - pairs cancel (x ^ x = 0) and only the single remains
// Recognize: "appears twice except one" + O(1) space -> XOR
// Follow-up LC 260: exactly two elements appear once; return both (any order).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// XOR is commutative and x ^ x = 0, x ^ 0 = x. T: O(n), S: O(1).
int singleNumber(const vector<int>& nums) {
    int x = 0;   // XOR of all values seen
    for (int v : nums) { x ^= v; }
    return x;
}

// follow-up LC 260: xor = a ^ b != 0; a set bit of it splits a and b into two groups,
// each pair lands in one group, so XOR each group. T: O(n), S: O(1).
vector<int> singleNumberIII(const vector<int>& nums) {
    long long x = 0;   // a ^ b
    for (int v : nums) { x ^= v; }
    // lowest set bit; long long so -x cannot overflow at INT_MIN
    long long low = x & -x;
    int a = 0, b = 0;   // XOR of the group with / without the low bit
    for (int v : nums) {
        if (v & low) {
            a ^= v;
        } else {
            b ^= v;
        }
    }
    return {a, b};
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (singleNumber(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 2, 1}, 1);
    test("lc ex2", {4, 1, 2, 1, 2}, 4);
    test("single", {1}, 1);
}
