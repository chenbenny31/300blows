// n courses, [a, b] means take b before a, return any order finishes all courses

// acyclic graph? cycle detection return?
// dup edge? self-loop?
// validate order (any?)

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class CourseScheduler {
public:
    // Kahn's topological sort, T: O(V+E), S: O(V+E)
    vector<int> findOrder_1(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> graph(n); // from -> [to,]
        vector<int> indeg(n, 0); // course -> its remaining deps cnt
        for (const auto& p : prereqs) {
            int to = p[0];
            int from = p[1];
            graph[from].push_back(to);
            indeg[to]++;
        }

        vector<int> order;
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

    // follow-up: Kahn by level, T: O(V+E), S: O(V+E)
    vector<vector<int>> findLevels(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> graph(n);
        vector<int> indeg(n, 0);
        for (const auto& p : prereqs) {
            int to = p[0];
            int from = p[1];
            graph[from].push_back(to);
            indeg[to]++;
        }

        vector<vector<int>> levels; // levels[k] = courses startable when [0..k) are done
        vector<int> frontier;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                frontier.push_back(i);
            }
        }

        int done = 0;
        while (!frontier.empty()) {
            done += static_cast<int>(frontier.size());
            vector<int> next;
            for (int cur : frontier) {
                for (int nxt : graph[cur]) {
                    indeg[nxt]--;
                    if (indeg[nxt] == 0) {
                        next.push_back(nxt);
                    }
                }
            }
            levels.push_back(frontier);
            frontier = next;
        }

        if (done != n) { return {}; }
        return levels;
    }

    // DFS with cycle detection, T: O(V+E), S: O(V+E)
    vector<int> findOrder_2(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> graph(n); // from -> [to,] 
        for (const auto& p : prereqs) {
            int to = p[0];
            int from = p[1];
            graph[from].push_back(to);
        }

        vector<int> state(n, 0); // 0: un-visited, 1: on-path, 2: visited
        vector<int> order; // post-order, reverse at end
        for (int i = 0; i < n; i++) {
            if (state[i] == 0 && !dfs(i, graph, state, order)) {
                return {};
            }
        }
        std::reverse(order.begin(), order.end());
        return order;
    }

private:
    bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state, vector<int>& order) {
        state[cur] = 1;
        for (int nxt : graph[cur]) {
            if (state[nxt] == 1) {
                return false;
            }
            if (state[nxt] == 0 && !dfs(nxt, graph, state, order)) {
                return false;
            }
        }
        state[cur] = 2;
        order.push_back(cur); // post-order, need reverse
        return true;
    }
};

bool isValid(int n, const vector<vector<int>>& prereqs, const vector<int>& order) {
    if (static_cast<int>(order.size()) != n) {
        return false;
    }
    vector<int> pos(n); // course -> its final idx in order
    for (int i = 0; i < n; i++) {
        pos[order[i]] = i;
    }
    for (const auto& p : prereqs) {
        int to = p[0];
        int from = p[1];
        if (pos[from] > pos[to]) {
            return false;
        }
    }
    return true;
}

int main() {
    CourseScheduler cs;
    vector<vector<int>> diamond = {{1,0}, {2,0}, {3,1}, {3,2}};
    vector<vector<int>> cycle = {{1,0}, {0, 1}};

    assert(isValid(4, diamond, cs.findOrder_1(4, diamond)));
    assert(isValid(4, diamond, cs.findOrder_2(4, diamond)));
    assert(isValid(1, {}, cs.findOrder_1(1, {}))); // no edges
    assert(isValid(1, {}, cs.findOrder_2(1, {})));
    assert(cs.findOrder_1(2, cycle).empty());
    assert(cs.findOrder_1(3, cycle).empty()); // cycle plus a free node
    assert(cs.findOrder_2(2, cycle).empty());
    assert(cs.findOrder_2(3, cycle).empty());

    vector<vector<int>> wantLevels = {{0}, {1, 2}, {3}};
    assert(cs.findLevels(4, diamond) == wantLevels);
    assert(cs.findLevels(3, cycle).empty());
    std::cout << "all tests passed\n";
    return 0;
}
