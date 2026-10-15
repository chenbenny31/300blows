// B20: reverse each maximal run of ASCII letters/digits in place;
// every other char is a separator and stays where it is.
#include <iostream>
#include <string>
#include <utility>
using std::cout;
using std::string;
using std::swap;

// explicit ranges: isalnum depends on locale and is UB on a negative char
bool isAlnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

// Find each run [i, j), then two-pointer swap inside it. T: O(n), S: O(1).
void reverseRuns(string& s) {
    int n = s.size();
    int i = 0;
    while (i < n) {
        if (!isAlnum(s[i])) {
            i++;
            continue;
        }
        int j = i;   // one past the end of the run starting at i
        while (j < n && isAlnum(s[j])) { j++; }
        for (int l = i, r = j - 1; l < r; l++, r--) { swap(s[l], s[r]); }
        i = j;
    }
}

void test(const string& label, string s, const string& want) {
    reverseRuns(s);
    cout << label << (s == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("letters and digits", "ab12-cd!", "21ba-dc!");
    test("edge separators", "--hi  there", "--ih  ereht");
    test("char between Z and a", "AZ[az", "ZA[za");
}
