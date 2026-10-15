// LC 886: people 1..n, dislikes [a, b]. Can everyone be split into two groups so that
// no two people in the same group dislike each other?
// Follow-up LC 785: same check on a 0-indexed adjacency list -> isBipartite directly.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// BFS 2-coloring from every uncolored node (graph may be disconnected).
// T: O(V+E), S: O(V).
bool isBipartite(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> color(n, -1);   // color[u] = 0 / 1, -1 = not visited yet
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) { continue; }
        color[s] = 0;
        queue<int> q;           // colored nodes whose neighbors are unchecked
        q.push(s);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (color[nxt] == -1) {
                    color[nxt] = 1 - color[cur];
                    q.push(nxt);
                } else if (color[nxt] == color[cur]) {
                    return false;   // odd cycle
                }
            }
        }
    }
    return true;
}

// Build the undirected dislike graph, then 2-color it. T: O(V+E), S: O(V+E).
bool possibleBipartition(int n, const vector<vector<int>>& dislikes) {
    vector<vector<int>> graph(n);   // graph[u] = people u can't share a group with
    for (const auto& d : dislikes) {
        // 1-indexed input; a dislike works both ways
        graph[d[0] - 1].push_back(d[1] - 1);
        graph[d[1] - 1].push_back(d[0] - 1);
    }
    return isBipartite(graph);
}

void test(const string& label, int n, const vector<vector<int>>& dislikes, bool want) {
    cout << label << (possibleBipartition(n, dislikes) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", 4, {{1, 2}, {2, 3}, {3, 4}, {4, 1}}, true);
    test("triangle", 3, {{1, 2}, {2, 3}, {3, 1}}, false);
    test("odd cycle far", 5, {{1, 2}, {3, 4}, {4, 5}, {5, 3}}, false);
    test("no dislikes", 1, {}, true);
}
