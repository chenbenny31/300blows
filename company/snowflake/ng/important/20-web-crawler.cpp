// A01 (cf. LC 1236 / 127): fewest clicks from start to target, where graph[page] = pages
// it links to; 0 if start == target, -1 if unreachable. Follow-up: one shortest path.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::queue;
using std::reverse;

// BFS; graph[page] = links on page (edge = one click). T: O(V+E), S: O(V).
int minClicks(const unordered_map<string, vector<string>>& graph,
              const string& start, const string& target) {
    if (start == target) { return 0; }
    unordered_map<string, int> dist;   // dist[page] = fewest clicks to page; also visited
    queue<string> q;                   // pages reached whose links are not read yet
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        auto it = graph.find(cur);
        if (it == graph.end()) { continue; }   // a page with no entry has no links
        for (const string& nxt : it->second) {
            if (dist.count(nxt)) { continue; }
            // mark when pushed, so no page is read twice
            dist[nxt] = dist[cur] + 1;
            if (nxt == target) { return dist[nxt]; }
            q.push(nxt);
        }
    }
    return -1;
}

// follow-up: same BFS storing each page's parent, then walk back from target.
// T: O(V+E), S: O(V).
vector<string> shortestPath(const unordered_map<string, vector<string>>& graph,
                            const string& start, const string& target) {
    // parent[page] = page we first reached it from; also visited
    unordered_map<string, string> parent;
    queue<string> q;
    parent[start] = start;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        if (cur == target) { break; }
        auto it = graph.find(cur);
        if (it == graph.end()) { continue; }
        for (const string& nxt : it->second) {
            if (parent.count(nxt)) { continue; }
            parent[nxt] = cur;
            q.push(nxt);
        }
    }
    if (!parent.count(target)) { return {}; }
    vector<string> path;   // pages from target back to start, reversed at the end
    for (string cur = target; cur != start; cur = parent[cur]) { path.push_back(cur); }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

void test(const string& label, const unordered_map<string, vector<string>>& graph,
          const string& start, const string& target, int want) {
    cout << label << (minClicks(graph, start, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    unordered_map<string, vector<string>> graph = {{"home", {"about", "blog"}},
                                                   {"about", {"team"}},
                                                   {"team", {"jobs"}},
                                                   {"blog", {"jobs"}},
                                                   {"jobs", {"home"}}};
    test("shortest", graph, "home", "jobs", 2);
    test("same page", graph, "home", "home", 0);
    test("unreachable in cycle", graph, "home", "login", -1);
}
