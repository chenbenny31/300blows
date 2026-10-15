// A18: roles 0..n-1 form a DAG, parents[v] = roles v inherits. Each role has local
// allow / deny lists; effective(v) = (union of parents' effective + allow[v]) - deny[v].
// Return each role's effective permissions sorted; cycle -> {}.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::set;

// Kahn's topo order: a role resolves once all its parents are final.
// T: O(V+E) for the sort, plus O(E*P log P) for the unions (P = permissions).
vector<vector<string>> effective(const vector<vector<int>>& parents,
                                 const vector<vector<string>>& allow,
                                 const vector<vector<string>>& deny) {
    int n = parents.size();
    vector<vector<int>> children(n);   // children[p] = roles that inherit p
    vector<int> indeg(n, 0);           // indeg[v] = parents of v not yet resolved
    for (int v = 0; v < n; v++) {
        for (int p : parents[v]) {
            children[p].push_back(v);
            indeg[v]++;
        }
    }
    queue<int> q;                      // roles whose parents are all resolved
    for (int v = 0; v < n; v++) {
        if (indeg[v] == 0) { q.push(v); }
    }
    vector<set<string>> eff(n);        // eff[v] = v's effective permissions, sorted
    int done = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        done++;
        for (int p : parents[cur]) { eff[cur].insert(eff[p].begin(), eff[p].end()); }
        eff[cur].insert(allow[cur].begin(), allow[cur].end());
        // deny last: beats inherited and same-role allow
        for (const string& s : deny[cur]) { eff[cur].erase(s); }
        for (int nxt : children[cur]) {
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    // roles on or behind a cycle never reach indegree 0
    if (done != n) { return {}; }
    vector<vector<string>> result;
    for (int v = 0; v < n; v++) {
        result.push_back(vector<string>(eff[v].begin(), eff[v].end()));
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& parents,
          const vector<vector<string>>& allow, const vector<vector<string>>& deny,
          const vector<vector<string>>& want) {
    cout << label << (effective(parents, allow, deny) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond deny", {{}, {0}, {0}, {1, 2}}, {{"r"}, {"w"}, {"x"}, {}},
         {{}, {}, {}, {"w"}}, {{"r"}, {"r", "w"}, {"r", "x"}, {"r", "x"}});
    test("parent after child", {{1}, {}}, {{"w"}, {"r"}}, {{}, {}}, {{"r", "w"}, {"r"}});
    test("cycle", {{}, {0, 2}, {1}}, {{"r"}, {}, {}}, {{}, {}, {}}, {});
}
