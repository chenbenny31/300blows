#include <cassert>
#include <queue>
#include <vector>

using std::queue;
using std::vector;

// T: O(V+E), S: O(V+E)
bool possibleBipartition(int n, const vector<vector<int>>& dislikes) {
    vector<vector<int>> graph(n); // person -> people been disliked
    for (const auto& d : dislikes) {
        int u = d[0] - 1; // 1-indexed
        int v = d[1] - 1;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> color(n, -1); // group by 0/1, -1 default un-assigned
    queue<int> q;

    for (int start = 0; start < n; start++) {
        if (color[start] != -1) { continue; }
        color[start] = 0;
        q.push(start);

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

int main() {
    // Even cycle: groups {1, 3} and {2, 4}.
    assert(possibleBipartition(
        4, {{1, 2}, {2, 3}, {3, 4}, {4, 1}}));

    // Odd cycle in a separate component: must check every component.
    assert(!possibleBipartition(
        5, {{1, 2}, {3, 4}, {4, 5}, {5, 3}}));

    assert(possibleBipartition(1, {})); // Isolated person.
    assert(!possibleBipartition(1, {{1, 1}})); // Self-dislike.
}
