#include <cassert>
#include <string>
#include <unordered_set>
#include <vector>

using std::string;
using std::vector;

// T: O((V+E)*P), S: O(V+E+V*P), P: max limit privs of one role
vector<vector<string>> resolvedOrdered(int n, const vector<vector<int>>& grants,
                                       const vector<vector<string>>& own) {
    vector<vector<int>> children(n);
    vector<vector<int>> parents(n);
    vector<int> indeg(n, 0);

    for (const auto& g : grants) {
        int from = g[0]; 
        int to = g[1];
        children[from].push_back(to);
        parents[to].push_back(from);
        indeg[to]++;
    }

    vector<int> order;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { order.push_back(i); }
    }

    for (int i = 0; i < static_cast<int>(order.size()); i++) { // cycle lead order.size() < n
        int cur = order[i];
        for (int nxt : children[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { order.push_back(nxt); }
        }
    }

    if (static_cast<int>(order.size()) != n) { return {}; }

    vector<vector<string>> eff(n);
    for (int cur : order) {
        std::unordered_set<string> seen; // one hash-set for one eff[i]

        for (int parent : parents[cur]) { // parents preserves the grant order
            for (const string& priv : eff[parent]) {
                if (seen.insert(priv).second) { // insert -> {iterator, bool}
                    eff[cur].push_back(priv);
                }
            }
        }

        for (const string& priv : own[cur]) {
            if (seen.insert(priv).second) {
                eff[cur].push_back(priv);
            }
        }
    }
    return eff;
}

int main() {
    vector<vector<string>> own = {
        {"r"}, {"w"}, {"r", "x"}
    };

    vector<vector<string>> want = {
        {"r"}, {"w"}, {"w", "r", "x"}
    };

    assert(resolvedOrdered(3, {{1, 2}, {0, 2}}, own) == want);

    // rev grant order change output order: {"r", "w", "x"}
    // dup local privs: keep 1st occur
    // inherit and local dups: appear once
    // cycle: {}
}
