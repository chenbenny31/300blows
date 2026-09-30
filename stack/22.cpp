// backtracking with (open, close) counts, T: O(4^n / sqrt(n)), S: O(n) aux

#include <string>
#include <vector>

class Solution {
public:
    std::vector<std::string> generateParenthesis(int n) {
        std::vector<std::string> out;
        std::string cur;
        cur.reserve(2 * n);
        backtrack(n, 0, 0, cur, out);
        return out;
    }

private:
    void backtrack(int n, int open, int close, std::string cur, std::vector<std::string>& out) {
        if (close == n) {
            out.push_back(cur); // copy
            return;
        }
        if (open < n) {
            cur.push_back('(');
            backtrack(n, open + 1, close, cur, out);
            cur.pop_back();
        }
        if (close < open) {
            cur.push_back(')');
            backtrack(n, open, close + 1, cur, out);
            cur.pop_back();
        }
    }
};
