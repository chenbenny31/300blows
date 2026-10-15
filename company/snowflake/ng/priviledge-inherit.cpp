#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::set;
using std::unordered_set;

// Kahn's, return topo-order and build parents, T: O(V+E), S: O(V)
vector<int> topoOrder(int n, const vector<vector<int>>& grants,
                      vector<vector<int>>& parents) {
    vector<vector<int>> children(n);
    vector<int> indeg(n, 0);
    for (const auto& g : grants) {
        children[g[0]].push_back(g[1]);
        parents[g[1]].push_back(g[0]);
        indeg[g[1]]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    vector<int> order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);

        for (int nxt : children[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return order;
}

// use set to build perms by topo-order
vector<vector<string>> resolve(int n, const vector<vector<int>>& grants,
                               const vector<vector<string>>& own) {
    vector<vector<int>> parents(n);
    vector<int> order = topoOrder(n, grants, parents);

    if (static_cast<int>(order.size()) != n) { return {}; } // topo order fails all nodes

    vector<set<string>> eff(n); // std::set yields fixed-order of perms
    for (int i : order) {
        eff[i].insert(own[i].begin(), own[i].end());

        for (int p : parents[i]) {
            eff[i].insert(eff[p].begin(), eff[p].end());
        }
    }

    vector<vector<string>> result;
    for (int i = 0; i < n; i++) {
        result.push_back(vector<string>(eff[i].begin(), eff[i].end())); // cast from std::set to vector
    }
    return result;
}

// follow-up: merge parents in grant order then own
vector<vector<string>> resultOrdered(int n, const vector<vector<int>>& grants,
                                     const vector<vector<string>>& own) {
    vector<vector<int>> parents(n);
    vector<int> order = topoOrder(n, grants, parents);

    if (static_cast<int>(order.size()) != n) { return {}; }

    vector<vector<string>> eff(n);
    for (int i : order) {
        unordered_set<string> seen; // track added perms
        for (int p : parents[i]) {
            for (const string& perm : eff[p]) {
                if (seen.insert(perm).second) {
                    eff[i].push_back(perm);
                }
            }
        }

        for (const string& perm : own[i]) {
            if (seen.insert(perm).second) {
                eff[i].push_back(perm);
            }
        }
    }
    return eff;
}

void test(const string& label,
          int n, const vector<vector<int>>& grants,
          const vector<vector<string>>& own,
          const vector<vector<string>>& want) {
    cout << label
         << (resolve(n, grants, own) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}},
         {{"r"}, {"w"}, {"x"}, {"r"}},
         {{"r"}, {"r", "w"}, {"r", "x"}, {"r", "w", "x"}});
    test("cycle", 3, {{0, 1}, {1, 2}, {2, 1}},
         {{"r"}, {"w"}, {"x"}}, {});
}
