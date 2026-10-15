#include <algorithm>
#include <cassert>
#include <string>

using std::string;

bool isAlnum(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9');
}

// T: O(n), S: O(1)
void reverseRuns(string& s) {
    int n = static_cast<int>(s.size());

    int i = 0;
    while (i < n) {
        if (!isAlnum(s[i])) {
            i++;
            continue;
        }

        int start = i;
        while (i < n && isAlnum(s[i])) {
            i++;
        }
        std::reverse(s.begin() + start, s.begin() + i);
    }
}

int main() {
    // Leading/repeated/trailing separators, mixed letters/digits.
    string s = "!!Ab12--cd!";
    reverseRuns(s);
    assert(s == "!!21bA--dc!");

    s = "abc";
    reverseRuns(s);
    assert(s == "cba"); // Run reaches the end.

    s = "!?";
    reverseRuns(s);
    assert(s == "!?"); // Only separators.

    s = "";
    reverseRuns(s);
    assert(s.empty());
}
