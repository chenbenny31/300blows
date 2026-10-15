#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's, T: O(V+E), S: O(V+E)
vector<int> findOrder(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n);
    vector<int> indeg(n, 0);
    for (const auto& p : prereqs) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    vector<int> order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }

    if (static_cast<int>(order.size()) != n) { return {}; }
    return order;
}

// 3-color DFS, postorder to collect dep path, 0: un-seen, 1: on-path, 2: seen
bool dfs(int cur, const vector<vector<int>>& graph,
         vector<int>& state, vector<int>& order) {
    if (state[cur] == 2) { return true; }
    if (state[cur] == 1) { return false; } // cycle detect

    state[cur] = 1;
    for (int nxt : graph[cur]) {
        if (!dfs(nxt, graph, state, order)) { return false; }
    }
    state[cur] = 2;
    order.push_back(cur); // post-order
    return true;
}

// follow-up: DFS from target
vector<int> findOrderFromTarget(int n, const vector<vector<int>>& prereqs, int target) {
    vector<vector<int>> graph(n);
    for (const auto& p : prereqs) {
        graph[p[0]].push_back(p[1]);
    }
    vector<int> state(n, 0);
    vector<int> order;
    if (!dfs(target, graph, state, order)) { return {}; }
    return order;
}

void test(const string& label, int n, const vector<vector<int>>& prereqs,
          const vector<int>& want) {
    cout << label << (findOrder(n, prereqs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unique order", 3, {{2, 0}, {2, 1}, {1, 0}}, {0, 1, 2});
    test("cycle", 3, {{1, 0}, {2, 1}, {1, 2}}, {});
}
