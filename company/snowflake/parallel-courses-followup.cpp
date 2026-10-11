#include <algorithm>
#include <cassert>
#include <queue>
#include <vector>

using std::vector;

// T: O(V+E), S: O(V+E)
long long minMonths(int n, const vector<vector<int>>& relations,
                    const vector<int>& duration) {
    vector<vector<int>> graph(n);
    vector<int> indeg(n, 0);

    for (const auto& r : relations) {
        int from = r[0] - 1; // 1-indexed
        int to = r[1] - 1;
        graph[from].push_back(to);
        indeg[to]++;
    }

    std::queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    vector<long long> start(n, 0); // each course's start time
    long long total = 0; // overall finish time
    int taken = 0;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        taken++;

        long long finish = start[cur] + duration[cur];
        total = std::max(total, finish);

        for (int nxt : graph[cur]) {
            start[nxt] = std::max(start[nxt], finish);
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return taken == n ? total : -1;
}

int main() {
    assert(minMonths(3, {{1, 3}, {2, 3}}, {3, 2, 5}) == 8);

    // n=2, {}, {3, 7}: 7
    // n=3, 1->2->3, {2, 3, 4}: 9
    // n=3, 1->2, {2, 3, 10}: 10
    // cycle, n=2, 1->2 + 2->1, {3, 4}: -1
    // n=1, {}, {4}: 4
    // n=0, {}, {}: 0

}
