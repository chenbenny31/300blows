#include <cassert>
#include <string>
#include <utility>
#include <vector>

using std::pair;
using std::string;
using std::vector;

// T: O(n), S: O(n)
long long calculate(const string& s) {
    vector<pair<long long, int>> stk;
    long long result = 0;
    long long num = 0;
    int sign = 1;

    for (char c : s) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '+' || c == '-') {
            result += sign * num;
            num = 0;
            sign = c == '+' ? 1 : -1;
        } else if (c == '(') {
            stk.push_back({result, sign});
            result = 0;
            sign = 1;
        } else if (c == ')') {
            result += sign * num;
            num = 0;

            auto [lastResult, lastSign] = stk.back();
            stk.pop_back();
            result = lastResult + lastSign * result;
        }
    }
    return result + sign * num;
}

int main() {
    assert(calculate(" 12 - (5 - 6) ") == 13); // Spaces, digits, subtraction.
    assert(calculate("-(2+3)") == -5); // Leading unary minus.
    assert(calculate("1-(-(2))") == 3); // Nested unary minus.
    assert(calculate("7") == 7); // Final number must be included.
}
