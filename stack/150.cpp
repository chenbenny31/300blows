// stack of eval operands, T: O(n), S: O(n)

#include <vector>
#include <string>

class Solution {
public:
    int evalRPN(std::vector<std::string>& tokens) {
        std::vector<long long> stk; // vals computed so far, recent on top
        stk.reserve(tokens.size());

        for (const std::string& tok : tokens) {
            if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
                long long b = stk.back(); stk.pop_back(); // right operands on top
                long long a = stk.back(); stk.pop_back(); // right operands on top
                long long res;

                if (tok == "+") { res = a + b; }
                else if (tok == "-") { res = a - b; }
                else if (tok == "*") { res = a * b; }
                else { res = a / b; }
                stk.push_back(res);
            } else {
                stk.push_back(std::stoll(tok));
            }
        }
        return static_cast<int>(stk.back());
    }
};
