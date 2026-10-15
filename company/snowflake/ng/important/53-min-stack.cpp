// LC tag · LC 155: stack with push, pop, top, getMin, each O(1). pop/top/getMin are
// only called on a non-empty stack.
// Follow-up: use less memory when the min rarely changes.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::min;

// Each entry remembers the min at push time. T: O(1) per op, S: O(n).
class MinStack {
    vector<pair<int, int>> stk;   // (value, min of this entry and everything below)

public:
    void push(int val) {
        int curMin = stk.empty() ? val : min(val, stk.back().second);
        stk.push_back({val, curMin});
    }

    void pop() { stk.pop_back(); }

    int top() { return stk.back().first; }

    int getMin() { return stk.back().second; }
};

// follow-up: a second stack grows only on a new min or a tie. T: O(1) per op, S: O(n).
class MinStack2 {
    vector<int> stk;    // all values
    vector<int> mins;   // non-increasing; back = current min

public:
    void push(int val) {
        stk.push_back(val);
        // <=: a duplicate min must be pushed, or popping one copy loses it
        if (mins.empty() || val <= mins.back()) { mins.push_back(val); }
    }

    void pop() {
        if (stk.back() == mins.back()) { mins.pop_back(); }
        stk.pop_back();
    }

    int top() { return stk.back(); }

    int getMin() { return mins.back(); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MinStack s;
    s.push(-2);
    s.push(0);
    s.push(-3);
    check(s.getMin() == -3, "lc ex1");
    s.pop();
    check(s.top() == 0 && s.getMin() == -2, "min restored");
    MinStack d;
    d.push(1);
    d.push(1);
    d.pop();
    check(d.getMin() == 1, "duplicate min");
}
