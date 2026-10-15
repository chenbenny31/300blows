// LC 125: after lowercasing and dropping non-alphanumeric chars, is s a palindrome?
// Follow-up LC 680: s of lowercase letters; palindrome after deleting at most one char?
#include <iostream>
#include <string>
using std::cout;
using std::string;

bool isAlnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

char lower(char c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

// Two pointers skipping non-alnum chars from both ends. T: O(n), S: O(1).
bool isPalindrome(const string& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) {
        if (!isAlnum(s[i])) { i++; continue; }
        if (!isAlnum(s[j])) { j--; continue; }
        if (lower(s[i]) != lower(s[j])) { return false; }
        i++;
        j--;
    }
    return true;
}

bool isRangePalindrome(const string& s, int i, int j) {
    while (i < j) {
        if (s[i++] != s[j--]) { return false; }
    }
    return true;
}

// follow-up: at the first mismatch, try skipping either side once. T: O(n), S: O(1).
bool validPalindrome(const string& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) {
        if (s[i] != s[j]) {
            return isRangePalindrome(s, i + 1, j) || isRangePalindrome(s, i, j - 1);
        }
        i++;
        j--;
    }
    return true;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "A man, a plan, a canal: Panama", true);
    test("lc ex2", "race a car", false);
    test("digit vs letter", "0P", false);
    test("only symbols", " .,", true);
}
