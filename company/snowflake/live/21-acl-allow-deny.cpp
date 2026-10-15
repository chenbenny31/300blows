// A18 (LC 210 / 2192 prototypes): multiple-inheritance DAG, each node has local allow / deny sets;
// effective(v) = (union of parents' effective + localAllow(v)) - localDeny(v); any cycle returns {}.

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
using std::queue;
using std::set;

class AclResolver {
public:
    // Kahn topological order, a node resolves once all its parents are final
    // T: O(V + E*A*logA + L*logA), S: O(V*A), A: distinct permissions, L: total local entries
    vector<vector<string>> effective(const vector<vector<int>>& parents, const vector<vector<string>>& allow,
                                     const vector<vector<string>>& deny) {
        int n = parents.size();
        vector<vector<int>> children(n);   // node -> nodes that inherit from it
        vector<int> indeg(n, 0);           // node -> parents not yet resolved
        for (int v = 0; v < n; v++) {
            for (int p : parents[v]) {
                children[p].push_back(v);
                indeg[v]++;
            }
        }
        queue<int> ready;                  // nodes whose parents are all resolved
        for (int v = 0; v < n; v++) {
            if (indeg[v] == 0) { ready.push(v); }
        }
        vector<set<string>> eff(n);        // node -> effective permissions
        int done = 0;
        while (!ready.empty()) {
            int v = ready.front();
            ready.pop();
            done++;
            resolve(v, parents, allow, deny, eff);
            for (int c : children[v]) {
                indeg[c]--;
                if (indeg[c] == 0) { ready.push(c); }
            }
        }
        if (done < n) { return {}; }       // nodes on or below a cycle never became ready
        return toLists(eff);
    }

    // DFS post-order with 3 colors, a back edge means a cycle
    // T: O(V + E*A*logA + L*logA), S: O(V*A + V) incl. recursion
    vector<vector<string>> effective2(const vector<vector<int>>& parents, const vector<vector<string>>& allow,
                                      const vector<vector<string>>& deny) {
        int n = parents.size();
        vector<int> state(n, 0);           // node -> 0 unvisited, 1 on the DFS path, 2 resolved
        vector<set<string>> eff(n);        // node -> effective permissions
        for (int v = 0; v < n; v++) {
            if (state[v] == 0 && !visit(v, parents, allow, deny, state, eff)) { return {}; }
        }
        return toLists(eff);
    }

private:
    // parents of v are final here
    void resolve(int v, const vector<vector<int>>& parents, const vector<vector<string>>& allow,
                 const vector<vector<string>>& deny, vector<set<string>>& eff) {
        for (int p : parents[v]) { eff[v].insert(eff[p].begin(), eff[p].end()); }
        eff[v].insert(allow[v].begin(), allow[v].end());
        for (const auto& perm : deny[v]) { eff[v].erase(perm); }   // deny last: beats inherited and same-node allow
    }

    bool visit(int v, const vector<vector<int>>& parents, const vector<vector<string>>& allow,
               const vector<vector<string>>& deny, vector<int>& state, vector<set<string>>& eff) {
        state[v] = 1;
        for (int p : parents[v]) {
            if (state[p] == 1) { return false; }
            if (state[p] == 0 && !visit(p, parents, allow, deny, state, eff)) { return false; }
        }
        resolve(v, parents, allow, deny, eff);
        state[v] = 2;
        return true;
    }

    vector<vector<string>> toLists(const vector<set<string>>& eff) {
        vector<vector<string>> result;
        for (const auto& s : eff) { result.push_back(vector<string>(s.begin(), s.end())); }   // set iterates sorted
        return result;
    }
};

bool test(const string& label, const vector<vector<int>>& parents, const vector<vector<string>>& allow,
          const vector<vector<string>>& deny, const vector<vector<string>>& expected) {
    AclResolver acl;
    vector<vector<string>> got = acl.effective(parents, allow, deny);
    vector<vector<string>> got2 = acl.effective2(parents, allow, deny);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << "\n";
    return false;
}

int main() {
    bool ok = true;
    // 0 root; 1, 2 under 0; 3 under 1 and 2; 4 under 1; 5 under 3
    ok &= test("diamond + regrant + same-node deny",
               {{}, {0}, {0}, {1, 2}, {1}, {3}},
               {{"read", "write"}, {"exec"}, {}, {}, {"write"}, {"admin"}},
               {{}, {"write"}, {}, {}, {}, {"admin", "exec"}},
               {{"read", "write"}, {"exec", "read"}, {"read", "write"}, {"exec", "read", "write"},
                {"exec", "read", "write"}, {"read", "write"}});   // 3 gets write via 2; 4 re-grants; 5 deny beats allow
    ok &= test("single node", {{}}, {{"a"}}, {{}}, {{"a"}});                         // smallest input
    ok &= test("allow and deny same node", {{}}, {{"a"}}, {{"a"}}, {{}});              // deny wins on one node
    ok &= test("empty graph", {}, {}, {}, {});                                       // no nodes
    ok &= test("parent has larger index",
               {{2}, {0}, {}}, {{"x"}, {"y"}, {"z"}}, {{}, {"z"}, {}},
               {{"x", "z"}, {"x", "y"}, {"z"}});                                     // index order is not topo order
    ok &= test("whole graph is a cycle",
               {{2}, {0}, {1}}, {{"a"}, {}, {}}, {{}, {}, {}}, {});                  // cycle -> empty
    ok &= test("cycle below a valid root",
               {{}, {0, 2}, {1}}, {{"a"}, {}, {}}, {{}, {}, {}}, {});                // partial cycle still -> empty
    ok &= test("duplicate parent entry",
               {{}, {0, 0}}, {{"a"}, {}}, {{}, {}}, {{"a"}, {"a"}});                 // indegree counted twice, freed twice
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
