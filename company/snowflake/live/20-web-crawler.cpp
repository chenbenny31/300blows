// A01 · cf. LC 1236 / 127: given start, target and getAllLinks(page), return the fewest clicks from start to target (-1 if unreachable);
// follow-up: return one shortest click path (empty if unreachable).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

class WebCrawler {
public:
    // BFS, every click costs 1, T: O(V+E) link fetches and lookups, S: O(V)
    int minClicks(const string& start, const string& target,
                  const std::function<vector<string>(const string&)>& getAllLinks) {
        if (start == target) { return 0; }
        unordered_map<string, int> dist;   // page -> clicks from start, also the visited set
        dist[start] = 0;
        std::queue<string> q;              // pages discovered but not fetched yet
        q.push(start);
        while (!q.empty()) {
            string cur = q.front();
            q.pop();
            for (const string& nxt : getAllLinks(cur)) {
                if (dist.count(nxt) != 0) { continue; }
                dist[nxt] = dist[cur] + 1;
                if (nxt == target) { return dist[nxt]; }   // check on discovery: skips fetching the rest of the level
                q.push(nxt);
            }
        }
        return -1;
    }

    // BFS with parent links, walk back from target, T: O(V+E), S: O(V)
    vector<string> shortestPath(const string& start, const string& target,
                                const std::function<vector<string>(const string&)>& getAllLinks) {
        if (start == target) { return {start}; }
        unordered_map<string, string> parent;   // page -> page it was first reached from, also the visited set
        parent[start] = start;
        std::queue<string> q;                   // pages discovered but not fetched yet
        q.push(start);
        while (!q.empty()) {
            string cur = q.front();
            q.pop();
            for (const string& nxt : getAllLinks(cur)) {
                if (parent.count(nxt) != 0) { continue; }
                parent[nxt] = cur;
                if (nxt == target) { return buildPath(parent, start, target); }
                q.push(nxt);
            }
        }
        return {};
    }

private:
    vector<string> buildPath(const unordered_map<string, string>& parent, const string& start, const string& target) {
        vector<string> path = {target};
        while (path.back() != start) {
            string prev = parent.find(path.back())->second;   // copy before push_back may reallocate
            path.push_back(prev);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
};

// path starts at start, ends at target, has clicks + 1 pages, and every step is a real link
bool isValidPath(const unordered_map<string, vector<string>>& web, const string& start, const string& target,
                 int clicks, const vector<string>& path) {
    if (clicks == -1) { return path.empty(); }
    if ((int)path.size() != clicks + 1 || path.front() != start || path.back() != target) { return false; }
    for (int i = 0; i + 1 < (int)path.size(); i++) {
        auto it = web.find(path[i]);
        if (it == web.end()) { return false; }
        if (std::find(it->second.begin(), it->second.end(), path[i + 1]) == it->second.end()) { return false; }
    }
    return true;
}

bool fetchedAtMostOnce(const unordered_map<string, int>& fetches) {
    for (const auto& [page, times] : fetches) {
        if (times > 1) { return false; }
    }
    return true;
}

bool test(const string& label, const unordered_map<string, vector<string>>& web,
          const string& start, const string& target, int expected) {
    unordered_map<string, int> fetches;   // page -> getAllLinks calls on it
    std::function<vector<string>(const string&)> getAllLinks = [&](const string& page) {
        fetches[page]++;
        auto it = web.find(page);
        return it == web.end() ? vector<string>{} : it->second;
    };
    WebCrawler wc;
    int got = wc.minClicks(start, target, getAllLinks);
    bool once = fetchedAtMostOnce(fetches);
    fetches.clear();
    vector<string> path = wc.shortestPath(start, target, getAllLinks);
    once = once && fetchedAtMostOnce(fetches);
    bool pathOk = isValidPath(web, start, target, expected, path);
    if (got == expected && pathOk && once) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected
         << ", path size " << path.size() << (pathOk ? "" : " (invalid)")
         << (once ? "" : ", a page was fetched twice") << "\n";
    return false;
}

int main() {
    bool ok = true;
    unordered_map<string, vector<string>> web = {
        {"a", {"b", "c"}}, {"b", {"d"}}, {"d", {"e"}}, {"e", {"t"}}, {"c", {"t"}},
    };
    ok &= test("short beats first-link dive", web, "a", "t", 2);   // DFS along the first link finds 4
    ok &= test("start is target", web, "a", "a", 0);               // zero clicks, path is [start]
    ok &= test("direct link", web, "c", "t", 1);                   // one click
    ok &= test("unreachable", web, "b", "c", -1);                  // target exists, only reachable from elsewhere

    unordered_map<string, vector<string>> loop = {
        {"a", {"a", "b", "b"}}, {"b", {"a", "c"}}, {"c", {"b"}},
    };
    ok &= test("self link, duplicates, cycle", loop, "a", "c", 2);   // visited set stops the a <-> b loop
    ok &= test("cycle, missing target", loop, "a", "z", -1);         // explores the whole cycle once, then gives up
    ok &= test("start without links", loop, "x", "a", -1);           // getAllLinks returns empty for an unknown page
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
