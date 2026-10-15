#include <cassert>
#include <set>
#include <string>
#include <vector>

using std::vector;
using std::string;

// T: O((V+E)*P*log(P+1)), S: O(V+E+V*P), P: max limit privs per role
vector<vector<string>> resolvePriviledges(int n, const vector<vector<int>>& grants,
                                          const vector<vector<string>>& own) {
    vector<vector<int>> children(n); // children for graph traverse
    vector<vector<int>> parents(n); // parents for previledges collect
    vector<int> indeg(n, 0);

    for (const auto& g : grants) {
        int from = g[0], to = g[1];
        children[from].push_back(to);
        parents[to].push_back(from);
        indeg[to]++;
    }

    vector<int> order; // serving both queue and topo-order
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { order.push_back(i); }
    }

    for (int i = 0; i < static_cast<int>(order.size()); i++) {
        int cur = order[i];
        for (int nxt : children[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { order.push_back(nxt); }
        }
    }

    if (static_cast<int>(order.size()) != n) { return {}; }

    vector<std::set<string>> eff(n); // std::set provides stable order
    for (int cur : order) {
        eff[cur].insert(own[cur].begin(), own[cur].end());

        for (int parent : parents[cur]) {
            eff[cur].insert(eff[parent].begin(), eff[parent].end());
        }
    }

    vector<vector<string>> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = vector<string>(eff[i].begin(), eff[i].end());
    }
    return result;
}

int main() {
    vector<vector<string>> own = {
        {"r"}, {"w"}, {"r"}, {"x"}
    };
    vector<vector<int>> grants = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}
    };
    vector<vector<string>> want = {
        {"r"},
        {"r", "w"},
        {"r"},
        {"r", "w", "x"}
    };
    assert(resolvePriviledges(4, grants, own) == want);

    // dup privileges
    // dis-conn roles retain own privileges
    // cycle prevents valid topo order
}
