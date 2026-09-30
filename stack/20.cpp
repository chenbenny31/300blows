// stack, T: O(n), S: O(n)

#include <string>
#include <vector>

class Solution {
public:
    bool isValid(std::string s) {
        std::vector<char> stk; // stores closing brackets owned
        stk.reserve(s.size());
        for (char c : s) {
            if (c == '(') { stk.push_back(')'); }
            else if (c == '[') { stk.push_back(']'); }
            else if (c == '{') { stk.push_back('}'); }
            else if (stk.empty() || stk.back() != c) { return false; }
            else { stk.pop_back(); }
        }
        return stk.empty();
    }
};
