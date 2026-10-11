#include <algorithm>
#include <cassert>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::vector;

struct HtmlParser {
    std::unordered_map<string, vector<string>> links;

    vector<string> getUrls(const string& url) const {
        auto it = links.find(url);
        return it == links.end() ? vector<string>{} : it->second;
    }
};

// T: O(V+E) expect, S: O(V+D), D=max-link-list-size
vector<string> shortestPath(const string& start, const string& target,
                            const HtmlParser& parser) {
    std::unordered_map<string, string> parent;
    parent[start] = start;
    std::queue<string> q;
    q.push(start);

    while (!q.empty()) {
        string cur = q.front();
        q.pop();

        if (cur == target) {
            vector<string> path;
            for (string p = target; p != start; p = parent.at(p)) {
                path.push_back(p);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const string& nxt : parser.getUrls(cur)) {
            if (parent.count(nxt) == 0) {
                parent[nxt] = cur; // mark visited before enqueue
                q.push(nxt);
            }
        }
    }
    return {};
}

int main() {
    HtmlParser parser;
    parser.links = {
        {"A", {"B", "C"}},
        {"B", {"A", "D"}},
        {"C", {"D"}},
    };

    vector<string> want = {"A", "B", "D"};
    assert(shortestPath("A", "D", parser) == want); // A->C->D also valid

    // start==target: path only start
    // unreachable: empty
}
