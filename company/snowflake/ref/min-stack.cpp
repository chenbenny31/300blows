// stack with push, pop, top, getMin, every operation O(1) (LC155)

// pop/top/getMin only called on a non-empty stack?
// duplicate minimum values?
// int range?

#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

// 1: every entry remembers the min at the time it was pushed, T: O(1) each, S: O(n) pairs
class MinStack_1 {
private:
    vector<std::pair<int, int>> st; // (value, min of this entry and everything below)

public:
    void push(int x) {
        int m = st.empty() ? x : std::min(x, st.back().second);
        st.push_back({x, m});
    }

    void pop() {
        st.pop_back();
    }

    int top() {
        return st.back().first;
    }

    int getMin() {
        return st.back().second;
    }
};

// 2: second stack only grows when a new min (or a tie) arrives, T: O(1) each, S: O(n) worst
class MinStack_2 {
private:
    vector<int> st; // all values
    vector<int> mins; // non-increasing, top = current min

public:
    void push(int x) {
        st.push_back(x);
        if (mins.empty() || x <= mins.back()) { // <= : a duplicate min must be pushed too
            mins.push_back(x);
        }
    }

    void pop() {
        if (st.back() == mins.back()) {
            mins.pop_back();
        }
        st.pop_back();
    }

    int top() {
        return st.back();
    }

    int getMin() {
        return mins.back();
    }
};

int main() {
    MinStack_1 a;
    a.push(-2);
    a.push(0);
    a.push(-3);
    assert(a.getMin() == -3); // target: LC example
    a.pop();
    assert(a.top() == 0);
    assert(a.getMin() == -2); // min restored after pop
    MinStack_1 a2;
    a2.push(1);
    a2.push(1);
    a2.pop();
    assert(a2.getMin() == 1); // edge: duplicate min survives one pop

    MinStack_2 b;
    b.push(-2);
    b.push(0);
    b.push(-3);
    assert(b.getMin() == -3); // target
    b.pop();
    assert(b.top() == 0);
    assert(b.getMin() == -2);
    MinStack_2 b2;
    b2.push(1);
    b2.push(1);
    b2.pop();
    assert(b2.getMin() == 1); // edge: fails if push used < instead of <=

    std::cout << "all tests passed\n";
    return 0;
}
