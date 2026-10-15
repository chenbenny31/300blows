// ByteDance (hack2hire): directed graph as an adjacency list, graph[u] = nodes u points
// to (may contain cycles). Return all simple paths (no repeated node) from start
// to target.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// path = nodes from start to cur; onPath[v] = v is on path
void dfs(const vector<vector<int>>& graph, int cur, int target, vector<bool>& onPath,
         vector<int>& path, vector<vector<int>>& result) {
    onPath[cur] = true;
    path.push_back(cur);
    if (cur == target) {
        result.push_back(path);
    } else {
        for (int nxt : graph[cur]) {
            if (!onPath[nxt]) { dfs(graph, nxt, target, onPath, path, result); }
        }
    }
    // unset on return: a different path may still pass through cur
    path.pop_back();
    onPath[cur] = false;
}

// Backtracking DFS over every simple path from start. T: O(V! * V) worst, S: O(V).
vector<vector<int>> allPaths(const vector<vector<int>>& graph, int start, int target) {
    vector<vector<int>> result;
    vector<int> path;
    vector<bool> onPath(graph.size(), false);
    dfs(graph, start, target, onPath, path, result);
    return result;
}

void test(const string& label, const vector<vector<int>>& graph, int start, int target,
          const vector<vector<int>>& want) {
    cout << label << (allPaths(graph, start, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{1, 2}, {3}, {3}, {}}, 0, 3, {{0, 1, 3}, {0, 2, 3}});
    test("cycle", {{1}, {0, 2}, {}}, 0, 2, {{0, 1, 2}});
}
