// Apple (hack2hire) / LC 785: undirected graph, n nodes 0..n-1, edge list [u, v].
// Can the nodes be 2-colored so no edge joins two nodes of the same color?
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// BFS from every uncolored node (graph may be disconnected), neighbors get the other
// color. T: O(V+E), S: O(V+E).
bool isBipartite(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);   // graph[u] = neighbors of u
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }
    vector<int> color(n, -1);       // color[u] = 0 / 1, -1 = not visited yet
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) { continue; }
        color[s] = 0;
        queue<int> q;               // colored nodes whose neighbors are unchecked
        q.push(s);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (color[nxt] == -1) {
                    color[nxt] = 1 - color[cur];
                    q.push(nxt);
                } else if (color[nxt] == color[cur]) {
                    return false;
                }
            }
        }
    }
    return true;
}

void test(const string& label, int n, const vector<vector<int>>& edges, bool want) {
    cout << label << (isBipartite(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", 4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}, true);
    test("triangle", 3, {{0, 1}, {1, 2}, {2, 0}}, false);
    test("odd cycle far", 5, {{0, 1}, {2, 3}, {3, 4}, {4, 2}}, false);
    test("no edges", 1, {}, true);
}
