// nodes of a DAG, edges [from, to], return for each node its ancestors in increasing order

// acyclic or cycle case?
// dup edges or self-loop?
// output sorted?
// query for all or single node?
// nodes' id range?
// root's ancestor?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

class AncestorFinder {
public:
    // DFS from every nodes as src, T: O(V*(V+E)), S: O(V+E)
    vector<vector<int>> getAncestors_1(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> graph(n); // from -> [to,]
        for (const auto& e : edges) {
            int from = e[0];
            int to = e[1];
            graph[from].push_back(to);
        }

        vector<int> visitedBy(n, -1); // node -> last node reaches it, -1 means none
        vector<vector<int>> ancestors(n); // node -> its ancestors in increasing order
        for (int src = 0; src < n; src++) {
            dfs(src, src, graph, visitedBy, ancestors);
        }
        return ancestors;
    }

private:
    void dfs(int src, int cur, const vector<vector<int>>& graph,
             vector<int>& visitedBy, vector<vector<int>>& ancestors) {
        for (int nxt : graph[cur]) {
            if (visitedBy[nxt] == src) {
                continue;
            }
            visitedBy[nxt] = src;
            ancestors[nxt].push_back(src);
            dfs(src, nxt, graph, visitedBy, ancestors);
        }
    }
    
public:
    // Kahn's order, child ORs in parent's final set, T: O(V*E+V^2), S: O(V^2)
    vector<vector<int>> getAncestors_2(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        vector<int> indeg(n, 0); // node -> parents cnt not processed
        for (const auto& e : edges) {
            int from = e[0];
            int to = e[1];
            graph[from].push_back(to);
            indeg[to]++;
        }

        vector<vector<int>> reach(n, vector<int>(n, 0)); // reach[i][j]=1 iff j is ancestor of i
        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                order.push_back(i);
            }
        }

        for (int i = 0; i < static_cast<int>(order.size()); i++) {
            int cur = order[i]; // all cur's parents are done, reach[cur] is final
            for (int nxt : graph[cur]) {
                reach[nxt][cur] = 1;
                for (int src = 0; src < n; src++) {
                    if (reach[cur][src] == 1) {
                        reach[nxt][src] = 1;
                    }
                }
                indeg[nxt]--;
                if (indeg[nxt] == 0) {
                    order.push_back(nxt);
                }
            }
        }

        vector<vector<int>> ancestors(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) { // ascending scan yields sorted order
                if (reach[i][j] == 1) {
                    ancestors[i].push_back(j);
                }
            }
        }
        return ancestors;
    }
};

int main() {
    AncestorFinder af;
    vector<vector<int>> want;

    // n = 1, no edges
    want = {{}};
    assert(af.getAncestors_1(1, {}) == want);
    assert(af.getAncestors_2(1, {}) == want);

    // diamond: 0 reaches 3 via two paths, listed once
    want = {{}, {0}, {0}, {0,1,2}};
    assert(af.getAncestors_1(4, {{0,1},{0,2},{1,3},{2,3}}) == want);
    assert(af.getAncestors_2(4, {{0,1},{0,2},{1,3},{2,3}}) == want);

    // chain 2 -> 1 -> 0: ancestors larger than the node, need sorted
    want = {{1,2}, {2}, {}};
    assert(af.getAncestors_1(3, {{2,1},{1,0}}) == want);
    assert(af.getAncestors_2(3, {{2,1},{1,0}}) == want);

    // dup edge: ancestors listed once
    want = {{}, {0}};
    assert(af.getAncestors_1(2, {{0,1},{0,1}}) == want);
    assert(af.getAncestors_2(2, {{0,1},{0,1}}) == want);

    std::cout << "all tests passed\n";
    return 0;
}
