// A09: roles 0..n-1, grant [u, v] means v inherits u. Each role's privileges = own + all ancestors', deduplicated.
// Base output: each role's set sorted. Follow-up: merge parents in grant input order, then own; keep first occurrence. Cycle -> {}.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::unordered_set;

class PrivilegeResolver {
public:
    // Kahn order, a role unions its parents' final sets, T: O(V + E + (A + E*P) log P), S: O(V*P)
    vector<vector<string>> resolve(int n, const vector<vector<int>>& grants, const vector<vector<string>>& own) {
        vector<vector<int>> parents(n);           // role -> roles it inherits from, grant input order
        vector<int> order = topoOrder(n, grants, parents);
        if ((int)order.size() != n) { return {}; }
        vector<set<string>> eff(n);               // role -> effective privileges, sorted
        for (int v : order) {
            eff[v].insert(own[v].begin(), own[v].end());
            for (int p : parents[v]) { eff[v].insert(eff[p].begin(), eff[p].end()); }   // eff[p] is final: p came earlier
        }
        vector<vector<string>> result;
        for (int r = 0; r < n; r++) { result.push_back(vector<string>(eff[r].begin(), eff[r].end())); }
        return result;
    }

    // same order, merge parents in grant order then own, keep first occurrence, T: O(V + E + A + E*P) avg, S: O(V*P)
    vector<vector<string>> resolveOrdered(int n, const vector<vector<int>>& grants, const vector<vector<string>>& own) {
        vector<vector<int>> parents(n);           // role -> roles it inherits from, grant input order
        vector<int> order = topoOrder(n, grants, parents);
        if ((int)order.size() != n) { return {}; }
        vector<vector<string>> eff(n);            // role -> effective privileges, first-occurrence order
        for (int v : order) {
            unordered_set<string> seen;           // privileges already in eff[v]
            for (int p : parents[v]) {            // grant input order, NOT topo order
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

private:
    // Kahn's algorithm; fills parents; returned size < n means a cycle
    vector<int> topoOrder(int n, const vector<vector<int>>& grants, vector<vector<int>>& parents) {
        vector<vector<int>> children(n);          // role -> roles that inherit from it
        vector<int> indeg(n, 0);                  // role -> parents not yet resolved
        for (const auto& g : grants) {
            int u = g[0], v = g[1];
            children[u].push_back(v);
            parents[v].push_back(u);
            indeg[v]++;
        }
        std::queue<int> q;                        // roles whose parents are all resolved
        for (int r = 0; r < n; r++) {
            if (indeg[r] == 0) { q.push(r); }
        }
        vector<int> order;                        // roles in topological order
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            order.push_back(cur);
            for (int c : children[cur]) {
                indeg[c]--;
                if (indeg[c] == 0) { q.push(c); }
            }
        }
        return order;
    }
};

bool test(const string& label, int n, const vector<vector<int>>& grants, const vector<vector<string>>& own,
          const vector<vector<string>>& expected) {
    PrivilegeResolver pr;
    vector<vector<string>> got = pr.resolve(n, grants, own);
    if (got == expected) { return true; }
    cout << "FAIL sorted " << label << "\n";
    return false;
}

bool testOrdered(const string& label, int n, const vector<vector<int>>& grants, const vector<vector<string>>& own,
                 const vector<vector<string>>& expected) {
    PrivilegeResolver pr;
    vector<vector<string>> got = pr.resolveOrdered(n, grants, own);
    if (got == expected) { return true; }
    cout << "FAIL ordered " << label << "\n";
    return false;
}

int main() {
    bool ok = true;
    vector<vector<string>> own = {{"read"}, {"write"}, {"read"}, {"run"}};
    vector<vector<int>> diamond = {{0, 1}, {0, 2}, {1, 3}, {2, 3}};
    ok &= test("diamond", 4, diamond, own,
               {{"read"}, {"read", "write"}, {"read"}, {"read", "run", "write"}});   // target: read reaches 3 twice, kept once
    ok &= test("single dup own", 1, {}, {{"a", "a"}}, {{"a"}});                       // smallest input, duplicate own
    ok &= test("disconnected", 2, {}, {{"b"}, {"a"}}, {{"b"}, {"a"}});                 // no grants
    ok &= test("child before parent id", 3, {{2, 0}, {0, 1}}, {{"x"}, {"y"}, {"z"}},
               {{"x", "z"}, {"x", "y", "z"}, {"z"}});                                  // topo order differs from id order
    ok &= test("cycle", 2, {{0, 1}, {1, 0}}, {{"a"}, {"b"}}, {});                     // cycle contract
    ok &= test("cycle behind root", 3, {{0, 1}, {1, 2}, {2, 1}}, {{"a"}, {"b"}, {"c"}}, {});   // a free root must not hide it

    ok &= testOrdered("diamond", 4, diamond, own,
                      {{"read"}, {"read", "write"}, {"read"}, {"read", "write", "run"}});   // parents 1 then 2, then own
    ok &= testOrdered("grant order", 3, {{1, 2}, {0, 2}}, {{"read"}, {"write"}, {"read", "run"}},
                      {{"read"}, {"write"}, {"write", "read", "run"}});                // topo pops 0 first, grant order says 1 first
    ok &= testOrdered("own after parents", 2, {{0, 1}}, {{"b"}, {"a", "b"}}, {{"b"}, {"b", "a"}});   // inherited before own
    ok &= testOrdered("cycle", 2, {{0, 1}, {1, 0}}, {{"a"}, {"b"}}, {});              // cycle contract
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
