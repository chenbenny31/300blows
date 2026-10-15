// LC 341: iterator over a nested list of integers: next() returns the next integer in
// order, hasNext() says whether one is left. Lists may be empty or deeply nested.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

struct NestedInteger {
    bool isInt;
    int val;                          // valid when isInt
    vector<NestedInteger> list;       // valid when !isInt
};

class NestedIterator {
    // (list, next index in it); back = innermost list being walked
    vector<pair<const vector<NestedInteger>*, int>> stk;

public:
    NestedIterator(const vector<NestedInteger>& nestedList) {
        stk.push_back({&nestedList, 0});
    }

    // hasNext leaves the top pointing at an integer. T: O(1) amortized.
    int next() {
        hasNext();
        auto& [lst, i] = stk.back();
        return (*lst)[i++].val;
    }

    // descend into lists and pop finished ones until an integer is on top.
    // T: O(1) amortized, S: O(depth).
    bool hasNext() {
        while (!stk.empty()) {
            auto& [lst, i] = stk.back();
            if (i == (int)lst->size()) {
                stk.pop_back();
                continue;
            }
            const NestedInteger& cur = (*lst)[i];
            if (cur.isInt) { return true; }
            // step past the sublist first so the parent resumes after it
            i++;
            stk.push_back({&cur.list, 0});
        }
        return false;
    }
};

NestedInteger num(int v) { return {true, v, {}}; }
NestedInteger nest(const vector<NestedInteger>& l) { return {false, 0, l}; }

void test(const string& label, const vector<NestedInteger>& nestedList,
          const vector<int>& want) {
    NestedIterator it(nestedList);
    vector<int> got;
    while (it.hasNext()) { got.push_back(it.next()); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {nest({num(1), num(1)}), num(2), nest({num(1), num(1)})},
         {1, 1, 2, 1, 1});
    test("deep", {num(1), nest({num(4), nest({num(6)})})}, {1, 4, 6});
    test("empty lists", {nest({}), nest({nest({})})}, {});
}
