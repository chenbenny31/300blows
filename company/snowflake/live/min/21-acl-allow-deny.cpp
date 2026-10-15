// A18 · LC 210 / 2192 prototypes: inheritance DAG, effective(v) = (union of parents' effective + allow[v]) - deny[v], sorted; any cycle -> {}.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using std::cout;
using std::string;
using std::vector;
using std::set;

// Kahn topo order, a node resolves once all its parents are final, T: O(V + E*A*logA + L*logA), S: O(V*A)
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
    std::queue<int> ready;             // nodes whose parents are all resolved
    for (int v = 0; v < n; v++) {
        if (indeg[v] == 0) { ready.push(v); }
    }
    vector<set<string>> eff(n);        // node -> effective permissions
    int done = 0;
    while (!ready.empty()) {
        int v = ready.front();
        ready.pop();
        done++;
        for (int p : parents[v]) { eff[v].insert(eff[p].begin(), eff[p].end()); }
        eff[v].insert(allow[v].begin(), allow[v].end());
        for (const auto& perm : deny[v]) { eff[v].erase(perm); }   // deny last: beats inherited and same-node allow
        for (int c : children[v]) {
            if (--indeg[c] == 0) { ready.push(c); }
        }
    }
    if (done < n) { return {}; }       // nodes on or below a cycle never became ready
    vector<vector<string>> result;
    for (const auto& s : eff) { result.push_back(vector<string>(s.begin(), s.end())); }   // set iterates sorted
    return result;
}

void test(const string& label, const vector<vector<int>>& parents, const vector<vector<string>>& allow,
          const vector<vector<string>>& deny, const vector<vector<string>>& expect) {
    vector<vector<string>> got = effective(parents, allow, deny);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    // 0 root; 1, 2 under 0; 3 under 1 and 2; 4 under 1; 5 under 3
    test("diamond", {{}, {0}, {0}, {1, 2}, {1}, {3}},
         {{"read", "write"}, {"exec"}, {}, {}, {"write"}, {"admin"}},
         {{}, {"write"}, {}, {}, {}, {"admin", "exec"}},
         {{"read", "write"}, {"exec", "read"}, {"read", "write"}, {"exec", "read", "write"},
          {"exec", "read", "write"}, {"read", "write"}});                                       // 3 gets write via 2, 4 re-grants, 5 deny beats allow
    test("empty", {}, {}, {}, {});                                                              // no nodes
    test("same-node deny", {{}}, {{"a"}}, {{"a"}}, {{}});                                       // deny wins on one node
    test("parent larger index", {{2}, {0}, {}}, {{"x"}, {"y"}, {"z"}}, {{}, {"z"}, {}},
         {{"x", "z"}, {"x", "y"}, {"z"}});                                                      // index order != topo order
    test("cycle below root", {{}, {0, 2}, {1}}, {{"a"}, {}, {}}, {{}, {}, {}}, {});             // partial cycle -> empty
}
