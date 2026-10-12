#include <cassert>
#include <vector>

using std::vector;

// T: O(V+E), S: O(V+E)
vector<int> findOrder(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n); // parent -> children
    vector<int> indeg(n, 0); // course -> remain pre-requisites
    for (const auto& p : prereqs) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }

    vector<int> order; // topo-order, also proc queue
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            order.push_back(i);
        }
    }

    for (int i = 0; i < static_cast<int>(order.size()); i++) {
        int cur = order[i];
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) {
                order.push_back(nxt);
            }
        }
    }

    if (static_cast<int>(order.size()) != n) { return {}; }
    return order;
}

int main() {
    // Diamond: course 3 must wait for BOTH 1 and 2.
    auto order = findOrder(4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}});
    vector<int> a = {0, 1, 2, 3};
    vector<int> b = {0, 2, 1, 3};
    assert(order == a || order == b);

    // Disconnected courses with no prerequisites.
    vector<int> expected = {0, 1};
    assert(findOrder(2, {}) == expected);

    // Cycle plus a free course: processing some courses isn't enough.
    assert(findOrder(3, {{1, 0}, {0, 1}}).empty());

    // A self-loop is also a cycle.
    assert(findOrder(1, {{0, 0}}).empty());
}
