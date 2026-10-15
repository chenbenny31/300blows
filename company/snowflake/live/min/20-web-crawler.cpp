// A01 · cf. LC 1236 / 127: fewest clicks from start to target using getAllLinks(page), -1 if unreachable. Follow-up: one shortest click path.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <functional>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using Links = std::function<vector<string>(const string&)>;

// BFS, every click costs 1, T: O(V+E), S: O(V)
int minClicks(const string& start, const string& target, const Links& getAllLinks) {
    if (start == target) { return 0; }
    unordered_map<string, int> dist;   // page -> clicks from start, also the visited set
    std::queue<string> q;              // pages discovered but not fetched yet
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        for (const string& nxt : getAllLinks(cur)) {
            if (dist.count(nxt)) { continue; }
            dist[nxt] = dist[cur] + 1;
            if (nxt == target) { return dist[nxt]; }   // check on discovery: skips fetching the rest of the level
            q.push(nxt);
        }
    }
    return -1;
}

// follow-up: BFS with parent links, walk back from target, T: O(V+E), S: O(V)
vector<string> shortestPath(const string& start, const string& target, const Links& getAllLinks) {
    unordered_map<string, string> parent;   // page -> page it was first reached from, also the visited set
    std::queue<string> q;
    parent[start] = start;
    q.push(start);
    while (!q.empty() && !parent.count(target)) {
        string cur = q.front();
        q.pop();
        for (const string& nxt : getAllLinks(cur)) {
            if (parent.count(nxt)) { continue; }
            parent[nxt] = cur;
            if (nxt == target) { break; }
            q.push(nxt);
        }
    }
    if (!parent.count(target)) { return {}; }
    vector<string> path = {target};
    while (path.back() != start) {
        string prev = parent[path.back()];   // copy before push_back may reallocate
        path.push_back(prev);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

Links fromMap(const unordered_map<string, vector<string>>& web) {
    return [&web](const string& page) { auto it = web.find(page); return it == web.end() ? vector<string>{} : it->second; };
}

void test(const unordered_map<string, vector<string>>& web, const string& s, const string& t, int expect) {
    int got = minClicks(s, t, fromMap(web));
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testPath(const string& label, const unordered_map<string, vector<string>>& web, const string& s, const string& t,
              const vector<string>& expect) {
    vector<string> got = shortestPath(s, t, fromMap(web));
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    unordered_map<string, vector<string>> web = {{"a", {"b", "c"}}, {"b", {"d"}}, {"d", {"e"}}, {"e", {"t"}}, {"c", {"t"}}};
    unordered_map<string, vector<string>> loop = {{"a", {"a", "b", "b"}}, {"b", {"a", "c"}}, {"c", {"b"}}};
    test(web, "a", "t", 2);                          // DFS down the first link would find 4
    test(web, "a", "a", 0);                          // start == target
    test(web, "b", "c", -1);                         // unreachable
    test(loop, "a", "z", -1);                        // self link + cycle: visited set terminates

    testPath("short path", web, "a", "t", {"a", "c", "t"});        // unique shortest path
    testPath("same page", web, "a", "a", {"a"});                   // [start]
    testPath("through cycle", loop, "a", "c", {"a", "b", "c"});    // duplicates and back edges
    testPath("unreachable", web, "b", "c", {});                    // empty
}
