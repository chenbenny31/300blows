// pages and links are discovered only through getUrls(url)
// LC1236: return every page reachable from startUrl on the same host
// Q06 / J15972: fewest clicks from start to target, return the path, empty if unreachable

// same host only? (LC1236 yes, Q06 ask)
// URL normalization: trailing slash, #fragment, http vs https, case?
// start == target? unreachable return?
// getUrls can fail or be slow: retry, skip, or abort?
// return the click count, or the path too?

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

struct HtmlParser { // test double for the given links API
    std::unordered_map<string, vector<string>> links; // page -> links on that page

    vector<string> getUrls(const string& url) const {
        auto it = links.find(url);
        return it == links.end() ? vector<string>{} : it->second;
    }
};

class WebCrawler {
public:
    // BFS over pages on startUrl's host, T: O(V+E) getUrls calls and checks, S: O(V)
    vector<string> crawl(const string& startUrl, const HtmlParser& parser) {
        string host = hostOf(startUrl);
        std::unordered_set<string> seen = {startUrl}; // pages already queued
        std::queue<string> q;
        q.push(startUrl);
        vector<string> result;
        while (!q.empty()) {
            string cur = q.front();
            q.pop();
            result.push_back(cur);
            for (const string& nxt : parser.getUrls(cur)) {
                if (hostOf(nxt) == host && seen.insert(nxt).second) { // mark on push
                    q.push(nxt);
                }
            }
        }
        return result;
    }

    // BFS, each click costs 1 so the first time target is popped is the fewest clicks
    // T: O(V+E), S: O(V)
    vector<string> shortestPath(const string& start, const string& target, const HtmlParser& parser) {
        std::unordered_map<string, string> parent; // page -> page clicked from, doubles as visited
        parent[start] = start; // start points to itself, stops the walk back
        std::queue<string> q;
        q.push(start);
        while (!q.empty()) {
            string cur = q.front();
            q.pop();
            if (cur == target) {
                vector<string> path;
                for (string p = target; p != start; p = parent[p]) {
                    path.push_back(p);
                }
                path.push_back(start);
                std::reverse(path.begin(), path.end());
                return path;
            }
            for (const string& nxt : parser.getUrls(cur)) {
                if (parent.count(nxt) == 0) {
                    parent[nxt] = cur;
                    q.push(nxt);
                }
            }
        }
        return {};
    }

private:
    // "http://host/path" -> "host"
    string hostOf(const string& url) {
        size_t end = url.find('/', 7); // skip "http://"
        return url.substr(7, end - 7); // npos - 7 still clamps to the end
    }
};

int main() {
    WebCrawler wc;
    HtmlParser web;
    web.links = {{"http://a.com/", {"http://a.com/x", "http://b.com/"}},
                 {"http://a.com/x", {"http://a.com/", "http://a.com/y"}}};
    vector<string> want;

    want = {"http://a.com/", "http://a.com/x", "http://a.com/y"};
    vector<string> got = wc.crawl("http://a.com/", web);
    std::sort(got.begin(), got.end()); // any order is valid
    assert(got == want); // target: other host skipped, link back to root doesn't loop
    want = {"http://lonely.com/"};
    assert(wc.crawl("http://lonely.com/", web) == want); // null: page with no links

    want = {"http://a.com/", "http://a.com/x", "http://a.com/y"};
    assert(wc.shortestPath("http://a.com/", "http://a.com/y", web) == want); // target: 2 clicks
    want = {"http://a.com/"};
    assert(wc.shortestPath("http://a.com/", "http://a.com/", web) == want); // edge: start == target
    assert(wc.shortestPath("http://a.com/", "http://c.com/", web).empty()); // fault: unreachable

    std::cout << "all tests passed\n";
    return 0;
}
