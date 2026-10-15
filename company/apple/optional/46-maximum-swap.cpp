// LC 670: swap two digits of num at most once to get the maximum value.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::to_string;
using std::stoi;
using std::swap;

// For each digit left to right, swap with the last occurrence of the largest bigger
// digit to its right. T: O(10 * d), S: O(d); d = number of digits.
int maximumSwap(int num) {
    string s = to_string(num);
    int n = s.size();
    vector<int> last(10, -1);   // last[d] = rightmost index of digit d
    for (int i = 0; i < n; i++) { last[s[i] - '0'] = i; }
    for (int i = 0; i < n; i++) {
        for (int d = 9; d > s[i] - '0'; d--) {
            // rightmost copy: moves the smaller digit as far right as possible
            if (last[d] > i) {
                swap(s[i], s[last[d]]);
                return stoi(s);
            }
        }
    }
    return num;
}

void test(const string& label, int num, int want) {
    cout << label << (maximumSwap(num) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", 2736, 7236);
    test("already max", 9973, 9973);
    test("dup max", 1993, 9913);
}
