// ByteDance (hack2hire) / LC 415 variant: num1 and num2 are non-negative integers as
// digit char arrays; return their sum as a digit char array without converting to int.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::reverse;

// Grade-school addition from the back with a carry. T: O(m+n), S: O(m+n).
vector<char> addStrings(const vector<char>& num1, const vector<char>& num2) {
    vector<char> sum;   // result digits, least significant first
    int i = (int)num1.size() - 1, j = (int)num2.size() - 1, carry = 0;
    // carry > 0 keeps going after both run out, e.g. 999 + 1
    while (i >= 0 || j >= 0 || carry > 0) {
        int d = carry;
        if (i >= 0) { d += num1[i--] - '0'; }
        if (j >= 0) { d += num2[j--] - '0'; }
        sum.push_back('0' + d % 10);
        carry = d / 10;
    }
    reverse(sum.begin(), sum.end());
    return sum;
}

void test(const string& label, const vector<char>& num1, const vector<char>& num2,
          const vector<char>& want) {
    cout << label << (addStrings(num1, num2) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {'1', '1'}, {'1', '2', '3'}, {'1', '3', '4'});
    test("carry", {'4', '5', '6'}, {'7', '7'}, {'5', '3', '3'});
    test("new digit", {'9', '9', '9'}, {'1'}, {'1', '0', '0', '0'});
}
