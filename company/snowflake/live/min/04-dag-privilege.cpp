// A09: roles 0..n-1, grant [u, v] means v inherits u. Each role's privileges =
// own + all ancestors', deduped, sorted; cycle -> {}.
// Follow-up J12349: merge parents in grant order, then own; keep first occurrence.
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

// Kahn's algorithm; also fills parents. Returned size < n means a cycle.
vector<int> topoOrder(int n, const vector<vector<int>>& grants,
                      vector<vector<int>>& parents) {
    vector<vector<int>> children(n);   // children[u] = roles that inherit u
    vector<int> indeg(n, 0);           // indeg[v] = parents of v not yet processed
    for (const auto& g : grants) {
        children[g[0]].push_back(g[1]);
        parents[g[1]].push_back(g[0]);
        indeg[g[1]]++;
    }
    queue<int> q;                      // roles whose parents are all processed
    for (int r = 0; r < n; r++) {
        if (indeg[r] == 0) { q.push(r); }
    }
    vector<int> order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : children[cur]) {
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return order;
}

// Topo order, then each role unions its parents' final sets.
// T: O(V+E) for the topo sort, plus O(E*P log P) for the unions (P = privileges).
vector<vector<string>> resolve(int n, const vector<vector<int>>& grants,
                               const vector<vector<string>>& own) {
    vector<vector<int>> parents(n);   // parents[v] = roles v inherits, in grant order
    vector<int> order = topoOrder(n, grants, parents);
    if ((int)order.size() != n) { return {}; }
    vector<set<string>> eff(n);       // eff[v] = v's effective privileges, sorted
    for (int v : order) {
        eff[v].insert(own[v].begin(), own[v].end());
        // eff[p] is already final: p came earlier in topo order
        for (int p : parents[v]) { eff[v].insert(eff[p].begin(), eff[p].end()); }
    }
    vector<vector<string>> result;
    for (int r = 0; r < n; r++) {
        result.push_back(vector<string>(eff[r].begin(), eff[r].end()));
    }
    return result;
}

// follow-up: same topo order; merge parents in GRANT order, then own,
// keeping first occurrence. T: O(V+E) + O(E*P).
vector<vector<string>> resolveOrdered(int n, const vector<vector<int>>& grants,
                                      const vector<vector<string>>& own) {
    vector<vector<int>> parents(n);   // parents[v] = roles v inherits, in grant order
    vector<int> order = topoOrder(n, grants, parents);
    if ((int)order.size() != n) { return {}; }
    vector<vector<string>> eff(n);    // eff[v] = v's privileges, first-occurrence order
    for (int v : order) {
        unordered_set<string> seen;   // privileges already in eff[v]
        for (int p : parents[v]) {
            for (const string& s : eff[p]) {
                if (seen.insert(s).second) { eff[v].push_back(s); }
            }
        }
        for (const string& s : own[v]) {
            if (seen.insert(s).second) { eff[v].push_back(s); }
        }
    }
    return eff;
}

void test(const string& label, int n, const vector<vector<int>>& grants,
          const vector<vector<string>>& own, const vector<vector<string>>& want) {
    cout << label << (resolve(n, grants, own) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, {{"a"}, {"b"}, {"c"}, {"d"}},
         {{"a"}, {"a", "b"}, {"a", "c"}, {"a", "b", "c", "d"}});
    test("cycle", 3, {{0, 1}, {1, 2}, {2, 1}}, {{"a"}, {"b"}, {"c"}}, {});
}
