// roles 0..n-1, grant [u, v] means role v inherits role u; own[r] = role r's own privileges
// J9549: each role's privileges = own + every ancestor's, as a set (sorted for output)
// J12349: ordered: merge parents' results in grant input order, then own in input order, keep first occurrence
// (NG phone screens; Q04/Q05 allow/deny versions are follow-ups)

// [u, v] = v inherits u?
// inherited first or own first? output order: sorted, or first occurrence?
// cycle: return empty, error, or merge the cycle?
// duplicate grants or duplicate privileges?
// deny rules? does a deny keep propagating, can a child re-grant?

#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class PrivilegeResolver {
public:
    // J9549: Kahn order, a role pulls from its parents once they are final
    // T: O(V + (E*P + own) * logP), S: O(V*P), P: max privileges of one role
    vector<vector<string>> resolveUnion(int n, const vector<vector<int>>& grants, const vector<vector<string>>& own) {
        vector<vector<int>> parents(n); // role -> roles it inherits, in grant input order
        vector<int> order = topoOrder(n, grants, parents);
        if (static_cast<int>(order.size()) != n) {
            return {}; // cycle
        }

        vector<std::set<string>> eff(n); // role -> effective privileges
        for (int v : order) {
            eff[v].insert(own[v].begin(), own[v].end());
            for (int p : parents[v]) {
                eff[v].insert(eff[p].begin(), eff[p].end());
            }
        }

        vector<vector<string>> result;
        for (int r = 0; r < n; r++) {
            result.push_back(vector<string>(eff[r].begin(), eff[r].end())); // set iterates sorted
        }
        return result;
    }

    // J12349: same order, but merge sequence follows grant input order, not topo order
    // T: O(V + E*P + own) average, S: O(V*P)
    vector<vector<string>> resolveOrdered(int n, const vector<vector<int>>& grants, const vector<vector<string>>& own) {
        vector<vector<int>> parents(n); // role -> roles it inherits, in grant input order
        vector<int> order = topoOrder(n, grants, parents);
        if (static_cast<int>(order.size()) != n) {
            return {}; // cycle
        }

        vector<vector<string>> eff(n); // role -> effective privileges, first-occurrence order
        for (int v : order) {
            std::unordered_set<string> seen; // privileges already in eff[v]
            for (int p : parents[v]) { // grant input order, eff[p] is final
                for (const string& s : eff[p]) {
                    if (seen.insert(s).second) {
                        eff[v].push_back(s);
                    }
                }
            }
            for (const string& s : own[v]) {
                if (seen.insert(s).second) {
                    eff[v].push_back(s);
                }
            }
        }
        return eff;
    }

private:
    // Kahn; fills parents in grant input order; order.size() < n means a cycle
    vector<int> topoOrder(int n, const vector<vector<int>>& grants, vector<vector<int>>& parents) {
        vector<vector<int>> children(n); // role -> roles inheriting from it
        vector<int> indeg(n, 0); // role -> parents not yet resolved
        for (const auto& g : grants) {
            children[g[0]].push_back(g[1]);
            parents[g[1]].push_back(g[0]);
            indeg[g[1]]++;
        }

        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                order.push_back(i);
            }
        }
        for (int i = 0; i < static_cast<int>(order.size()); i++) {
            for (int c : children[order[i]]) {
                indeg[c]--;
                if (indeg[c] == 0) {
                    order.push_back(c);
                }
            }
        }
        return order;
    }
};

int main() {
    PrivilegeResolver pr;
    vector<vector<string>> own = {{"read", "write"}, {"exec", "read"}, {"audit"}, {"write", "deploy"}};
    vector<vector<string>> want;

    // 2 inherits 0 then 1, 3 inherits 2
    want = {{"read", "write"}, {"exec", "read"}, {"audit", "exec", "read", "write"},
            {"audit", "deploy", "exec", "read", "write"}};
    assert(pr.resolveUnion(4, {{0, 2}, {1, 2}, {2, 3}}, own) == want);

    want = {{"read", "write"}, {"exec", "read"}, {"read", "write", "exec", "audit"},
            {"read", "write", "exec", "audit", "deploy"}};
    assert(pr.resolveOrdered(4, {{0, 2}, {1, 2}, {2, 3}}, own) == want); // dup "read" / "write" keep first spot

    want = {{"read", "write"}, {"exec", "read"}, {"exec", "read", "write", "audit"},
            {"exec", "read", "write", "audit", "deploy"}};
    assert(pr.resolveOrdered(4, {{1, 2}, {0, 2}, {2, 3}}, own) == want); // grant order, not topo order

    want = {{"a"}};
    assert(pr.resolveOrdered(1, {}, {{"a", "a"}}) == want); // single role, duplicate own privilege
    assert(pr.resolveUnion(1, {}, {{"a", "a"}}) == want);

    assert(pr.resolveOrdered(2, {{0, 1}, {1, 0}}, {{"a"}, {"b"}}).empty()); // cycle
    assert(pr.resolveUnion(2, {{0, 1}, {1, 0}}, {{"a"}, {"b"}}).empty());

    std::cout << "all tests passed\n";
    return 0;
}
