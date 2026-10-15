// LC 8: atoi: skip leading spaces, optional sign, read digits until a non-digit;
// clamp to [INT_MIN, INT_MAX]; 0 if no digits.
#include <iostream>
#include <string>
#include <climits>
using std::cout;
using std::string;

// Char loop; check overflow before multiplying. T: O(n), S: O(1).
int myAtoi(const string& s) {
    int n = s.size();
    int i = 0;
    while (i < n && s[i] == ' ') { i++; }
    int sign = 1;
    if (i < n && (s[i] == '+' || s[i] == '-')) {
        if (s[i] == '-') { sign = -1; }
        i++;
    }
    int result = 0;   // magnitude so far, always <= INT_MAX
    for (; i < n && s[i] >= '0' && s[i] <= '9'; i++) {
        int d = s[i] - '0';
        // result * 10 + d would exceed INT_MAX
        if (result > (INT_MAX - d) / 10) { return sign == 1 ? INT_MAX : INT_MIN; }
        result = result * 10 + d;
    }
    return sign * result;
}

void test(const string& label, const string& s, int want) {
    cout << label << (myAtoi(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spaces sign", "   -42", -42);
    test("trailing words", "4193 with words", 4193);
    test("words first", "words 987", 0);
    test("overflow", "-91283472332", INT_MIN);
}
