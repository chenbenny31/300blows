// ByteDance (hack2hire) / LC 547: n photos; similar[i][j] = 1 if photos i and j are
// similar. Similar photos, directly or through a chain, form a group. Count the groups.
// Follow-up: union-find.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// BFS from each unvisited photo; row cur is its adjacency list. T: O(n^2), S: O(n).
int countGroups(const vector<vector<int>>& similar) {
    int n = similar.size();
    vector<bool> seen(n, false);   // photo already placed in a group
    int groups = 0;
    for (int i = 0; i < n; i++) {
        if (seen[i]) { continue; }
        groups++;
        queue<int> q;   // photos of this group still to expand
        q.push(i);
        seen[i] = true;
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt = 0; nxt < n; nxt++) {
                if (similar[cur][nxt] == 1 && !seen[nxt]) {
                    seen[nxt] = true;
                    q.push(nxt);
                }
            }
        }
    }
    return groups;
}

struct DSU {
    vector<int> parent;   // parent[x] = x's parent; a root is its own parent
    int groups;           // number of disjoint sets

    DSU(int n) : parent(n), groups(n) {
        for (int i = 0; i < n; i++) { parent[i] = i; }
    }

    // path compression: point x straight at its root
    int find(int x) {
        if (parent[x] != x) { parent[x] = find(parent[x]); }
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) { return; }
        parent[ra] = rb;
        groups--;
    }
};

// follow-up: union every similar pair in the upper triangle. T: O(n^2 log n), S: O(n).
int countGroupsDSU(const vector<vector<int>>& similar) {
    int n = similar.size();
    DSU dsu(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (similar[i][j] == 1) { dsu.unite(i, j); }
        }
    }
    return dsu.groups;
}

void test(const string& label, const vector<vector<int>>& similar, int want) {
    cout << label << (countGroups(similar) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{1, 1, 0}, {1, 1, 0}, {0, 0, 1}}, 2);
    test("all alone", {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, 3);
    test("chain", {{1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, 1);
}
