#include <cassert>
#include <string>

using std::string;

// T: O(n), S: O(1)
long long calculate(const string& s) {
    long long result = 0;
    long long last = 0; // pendoperands open to * or /
    long long num = 0;
    char op = '+';
    int n = static_cast<int>(s.size());

    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool d = c >= '0' && c <= '9';
        if (d) { num = num * 10 + (c - '0'); }

        if ((!d && c != ' ') || i == n - 1) {
            if (op == '+') {
                result += last;
                last = num;
            } else if (op == '-') {
                result += last;
                last = -num;
            } else if (op == '*') {
                last *= num;
            } else if (op == '/') {
                last /= num;
            }

            op = c;
            num = 0;
        }
    }
    return result + last;
}

int main() {
    assert(calculate("3+2*2") == 7); // Multiplication before addition.
    assert(calculate(" 10-7/2 ") == 7); // Negative term, truncation, spaces.
    assert(calculate("8/3*3") == 6); // Equal precedence: left to right.
    assert(calculate("42") == 42); // Multi-digit final number.
}
