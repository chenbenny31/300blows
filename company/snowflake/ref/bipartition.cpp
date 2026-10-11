// LC886: n people 1..n, dislikes [a, b]: can everyone be split into two groups with no dislike inside a group?
// LC785: is an undirected graph (adjacency list, 0-indexed) bipartite? same check

// people 1-indexed? dislikes symmetric?
// duplicate pairs? someone disliking themselves?
// graph may be disconnected?
// return only true/false, or the two groups?

#include <vector>
#include <queue>
#include <cassert>
#include <iostream>

using std::vector;

class GraphColoring {
public:
    // LC785: BFS 2-coloring, a neighbor must get the other color, T: O(V+E), S: O(V)
    bool isBipartite(const vector<vector<int>>& graph) {
        int n = static_cast<int>(graph.size());
        vector<int> color(n, -1); // node -> 0 / 1, -1 = not colored yet
        for (int s = 0; s < n; s++) { // every component, not just the one holding node 0
            if (color[s] != -1) {
                continue;
            }
            color[s] = 0;
            std::queue<int> q;
            q.push(s);
            while (!q.empty()) {
                int cur = q.front();
                q.pop();
                for (int nxt : graph[cur]) {
                    if (color[nxt] == -1) {
                        color[nxt] = 1 - color[cur];
                        q.push(nxt);
                    } else if (color[nxt] == color[cur]) {
                        return false; // odd cycle
                    }
                }
            }
        }
        return true;
    }

    // LC886: build the undirected graph, then the same check, T: O(V+E), S: O(V+E)
    bool possibleBipartition(int n, const vector<vector<int>>& dislikes) {
        vector<vector<int>> graph(n); // person -> people they can't share a group with
        for (const auto& d : dislikes) {
            graph[d[0] - 1].push_back(d[1] - 1); // 1-indexed input
            graph[d[1] - 1].push_back(d[0] - 1); // dislike works both ways
        }
        return isBipartite(graph);
    }
};

int main() {
    GraphColoring gc;

    assert(gc.possibleBipartition(4, {{1, 2}, {1, 3}, {2, 4}})); // target: {1, 4} vs {2, 3}
    assert(!gc.possibleBipartition(3, {{1, 2}, {1, 3}, {2, 3}})); // fault: odd cycle
    assert(!gc.possibleBipartition(5, {{1, 2}, {3, 4}, {4, 5}, {3, 5}})); // edge: odd cycle in a second component
    assert(gc.possibleBipartition(1, {})); // null: one person, no dislikes
    assert(!gc.isBipartite({{1, 2, 3}, {0, 2}, {0, 1, 3}, {0, 2}})); // LC785 example

    std::cout << "all tests passed\n";
    return 0;
}
