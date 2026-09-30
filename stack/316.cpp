// mono-stack + last-occurrence, T: O(n), S: O(1)

#include <string>
#include <vector>
#include <cstdint>

class Solution {
public:
    std::string removeDuplicateLetters(std::string s) {
        const int n = static_cast<int>(s.length());
        constexpr int R = 26;
        constexpr int BASE = 'a';

        std::vector<int> lastIdx(R, 0); // pre-compute for future copy judge
        std::vector<uint8_t> inStk(R, 0); // 1 means letter is on stack
        for (int i = 0; i < n; i++) {
            lastIdx[s[i] - BASE] = i;
        }

        std::string stk; // distinct letters
        stk.reserve(R);

        for (int i = 0; i < n; i++) {
            if (inStk[s[i] - BASE]) { continue; } // already placed, ignore a later copy

            while(!stk.empty() && s[i] < stk.back() && lastIdx[stk.back() - BASE] > i) { // pop only top has latter copy
                inStk[stk.back() - BASE] = 0;
                stk.pop_back();
            }
            stk.push_back(s[i]);
            inStk[s[i] - BASE] = 1;
        }
        return stk;
    }
};
