// ByteDance (hack2hire): find all distinct simple cycles of a directed graph given as
// graph[u] = vertices u points to. Each cycle starts at its smallest vertex and repeats
// it at the end, e.g. {0, 1, 2, 0}.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// extend path from cur using only vertices > s; an edge back to s closes a cycle
void dfs(const vector<vector<int>>& graph, int s, int cur, vector<bool>& onPath,
         vector<int>& path, vector<vector<int>>& result) {
    onPath[cur] = true;
    path.push_back(cur);
    for (int nxt : graph[cur]) {
        if (nxt == s) {
            result.push_back(path);
            result.back().push_back(s);
        } else if (nxt > s && !onPath[nxt]) {
            dfs(graph, s, nxt, onPath, path, result);
        }
    }
    path.pop_back();
    onPath[cur] = false;
}

// For each start s, DFS through vertices > s only: s is the cycle's smallest vertex, so
// each cycle is found exactly once, already rotated. T: O(V! * V) worst, S: O(V).
vector<vector<int>> allCycles(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<int>> result;
    vector<int> path;                // vertices from s to the current one
    vector<bool> onPath(n, false);   // onPath[v] = v is on path
    for (int s = 0; s < n; s++) { dfs(graph, s, s, onPath, path, result); }
    return result;
}

void test(const string& label, const vector<vector<int>>& graph,
          const vector<vector<int>>& want) {
    cout << label << (allCycles(graph) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one cycle", {{1}, {2}, {0, 3}, {}}, {{0, 1, 2, 0}});
    test("shared node", {{1}, {0, 2}, {1}}, {{0, 1, 0}, {1, 2, 1}});
    test("self loop", {{0}}, {{0, 0}});
}
