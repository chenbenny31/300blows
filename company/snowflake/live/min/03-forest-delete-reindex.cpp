// A02: forest as a parent array (parent[i] == i means root). Delete x: x's children
// move up to parent[x], or become roots if x is a root. Renumber everything after x.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// One pass. T: O(n), S: O(n) for the output.
vector<int> deleteNode(const vector<int>& parent, int x) {
    int n = parent.size();
    bool xIsRoot = parent[x] == x;
    vector<int> result;   // new parent array, n - 1 entries
    for (int i = 0; i < n; i++) {
        if (i == x) { continue; }
        int p = parent[i];
        // decide the new parent in OLD indices first
        if (p == x) { p = xIsRoot ? i : parent[x]; }
        // then old -> new index: everything after x shifts down by one
        result.push_back(p < x ? p : p - 1);
    }
    return result;
}

void test(const string& label, const vector<int>& parent, int x, const vector<int>& want) {
    cout << label << (deleteNode(parent, x) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("target", {0, 0, 1, 1, 2}, 1, {0, 0, 0, 1});
    test("root", {0, 0, 0}, 0, {0, 1});
    test("parent after x", {1, 1, 0}, 0, {0, 0});
}
