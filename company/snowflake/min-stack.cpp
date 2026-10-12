#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

using std::pair;
using std::vector;

class MinStack {
private:
    vector<pair<int, int>> stk; // Entry is {value, min under this entry}

public:
    // T: O(1) amortized, S: O(n)
    void push(int value) {
        int min = stk.empty() ? value : std::min(value, stk.back().second);
        stk.push_back({value, min});
    }

    // T: O(1), S: O(1)
    void pop() {
        stk.pop_back();
    }

    // T: O(1), S: O(1)
    int top() const {
        return stk.back().first;
    }

    // T: O(1), S: O(1)
    int getMin() const {
        return stk.back().second;
    }
};

int main() {
    MinStack st;
    st.push(-2);
    st.push(0);
    st.push(-3);
    st.push(-3);

    assert(st.getMin() == -3);
    st.pop();
    assert(st.getMin() == -3); // Duplicate minimum survives.

    st.pop();
    assert(st.top() == 0);
    assert(st.getMin() == -2); // Previous minimum restored.

    st.pop();
    st.pop();
    st.push(5);
    assert(st.top() == 5 && st.getMin() == 5); // Reuse after emptying.
}
