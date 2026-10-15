// A02: forest as a parent array (parent[i] == i means root). Delete node x: x's children move up to parent[x],
// or become roots if x was a root. Remove x from the array and renumber every surviving index and parent value.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class ForestEditor {
public:
    // choose each survivor's parent in old indices, then shift indices above x down by one, T: O(n), S: O(n) output
    vector<int> deleteNode(const vector<int>& parent, int x) {
        int n = parent.size();
        bool xIsRoot = parent[x] == x;
        vector<int> result;                       // new parent array, n - 1 entries
        for (int i = 0; i < n; i++) {
            if (i == x) { continue; }
            int p = parent[i];                    // old index of i's parent
            if (p == x) { p = xIsRoot ? i : parent[x]; }   // child of x: becomes its own root, or moves up
            result.push_back(p < x ? p : p - 1);  // old -> new index; p can no longer be x here
        }
        return result;
    }

    // explicit old -> new index map, T: O(n), S: O(n)
    vector<int> deleteNode2(const vector<int>& parent, int x) {
        int n = parent.size();
        vector<int> newIndex(n, -1);              // old index -> new index, -1 for x
        int next = 0;
        for (int i = 0; i < n; i++) {
            if (i != x) { newIndex[i] = next++; }
        }
        vector<int> result(n - 1);
        for (int i = 0; i < n; i++) {
            if (i == x) { continue; }
            int p = parent[i];
            if (p == x) { p = parent[x] == x ? i : parent[x]; }
            result[newIndex[i]] = newIndex[p];
        }
        return result;
    }
};

bool test(const string& label, const vector<int>& parent, int x, const vector<int>& expected) {
    ForestEditor fe;
    vector<int> got = fe.deleteNode(parent, x);
    vector<int> got2 = fe.deleteNode2(parent, x);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("list correction", {0, 0, 1, 1, 2}, 1, {0, 0, 0, 1});   // the corrected platform example
    ok &= test("only node", {0}, 0, {});                               // smallest input
    ok &= test("root split", {0, 0, 0}, 0, {0, 1});                    // root's children each become a root
    ok &= test("chain", {0, 0, 1, 2}, 1, {0, 0, 1});                   // grandchild keeps its renumbered parent
    ok &= test("last index", {0, 0, 1}, 2, {0, 0});                    // nothing shifts
    ok &= test("two trees", {0, 0, 2, 2, 3}, 1, {0, 1, 1, 2});         // other tree's indices shift too
    ok &= test("parent above x", {1, 1, 0}, 0, {0, 0});                // root at index 1 must also be renumbered
    ok &= test("leaf", {0, 0, 0, 1}, 2, {0, 0, 1});                    // deleting a leaf only shifts later indices
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
