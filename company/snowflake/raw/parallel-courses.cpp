#include <cassert>
#include <queue>
#include <vector>

using std::vector;

// T: O(V+E), S: O(V+E)
int minSemesters(int n, const vector<vector<int>>& relations) {
    vector<vector<int>> graph(n);
    vector<int> indeg(n, 0);

    for (const auto& r : relations) {
        int from = r[0] - 1; // 1-indexed
        int to = r[1] - 1;
        graph[from].push_back(to);
        ++indeg[to];
    }

    std::queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    int semesters = 0;
    int taken = 0;
    while (!q.empty()) {
        int sz = q.size();
        semesters++;

        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            taken++;

            for (int nxt : graph[cur]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) { q.push(nxt); }
            }
        }
    }
    return taken == n ? semesters : -1;
}

int main() {
    assert(minSemesters(3, {{1, 3}, {2, 3}}) == 2); // take (1,2) then 3, 2 semesters
    // n = 4, disconnected course from graph: 3
    // n = 3, no deps, {}: 1
    // cycle case, n = 3, {{1, 2}, {2, 1}}: -1
    // single course: 1
    // no course: 0
}
