#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// one pass, T: O(n), S: O(n)
vector<int> deleteNode(const vector<int>& parent, int x) {
    const int n = static_cast<int>(parent.size());
    bool xIsRoot = (parent[x] == x);
    vector<int> result;

    for (int i = 0; i < n; i++) {
        if (i == x) { continue; }
        int p = parent[i];
        // x is root: each children becomes self-root
        // x is not root: inherit x's parent
        if (p == x) { p = xIsRoot ? i : parent[x]; }
        result.push_back(p < x ? p : p - 1); // every idx after x shift left by one
    }
    return result;
}

void test(const string& label,
          const vector<int>& parent, int x, const vector<int>& want) {
    cout << label
         << (deleteNode(parent, x) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("target", {0, 0, 1, 1, 2}, 1, {0, 0, 0, 1});
    test("root", {0, 0, 0}, 0, {0, 1});
    test("parent after x", {1, 1, 0}, 0, {0, 0});
}
