#include <cassert>
#include <vector>

using std::vector;

class AncestorFinder {
private:
    void dfs(int src, int cur, const vector<vector<int>>& graph,
             vector<int>& seen, vector<vector<int>>& result) {
        for (int nxt : graph[cur]) {
            if (seen[nxt] == src) { continue; }
            seen[nxt] = src;
            result[nxt].push_back(src);
            dfs(src, nxt, graph, seen, result);
        }
    }

public:
    // T: O(V*(V+E)), S: O(V^2+E)
    vector<vector<int>> getAncestors(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (const auto& e : edges) {
            graph[e[0]].push_back(e[1]);
        }

        vector<int> seen(n, -1); // node -> last src visited it
        vector<vector<int>> result(n); // node -> sorted ancestors

        for (int src = 0; src < n; src++) {
            dfs(src, src, graph, seen, result);
        }
        return result;
    }
};

int main() {
    AncestorFinder finder;

    // Diamond + duplicate edge + isolated node: no duplicate ancestors.
    vector<vector<int>> expected = {{}, {0}, {0}, {0, 1, 2}, {}};
    assert(finder.getAncestors(
        5, {{0, 1}, {0, 1}, {0, 2}, {1, 3}, {2, 3}}) == expected);

    // Ancestors may have larger IDs; output must still be sorted.
    expected = {{1, 2}, {2}, {}};
    assert(finder.getAncestors(3, {{2, 1}, {1, 0}}) == expected);

    // Single isolated node has no ancestors.
    expected = {{}};
    assert(finder.getAncestors(1, {}) == expected);
}
