#include <algorithm>
#include <cassert>
#include <queue>
#include <string>
#include <unordered_set>
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

// assume "http://hostname" optionally followed by "/path"
// T: O(L), S: O(L), L=url-len
string hostOf(const string& url) {
    const string sep = "://"; // skip "http://" or "https://"
    auto start = url.find(sep) + sep.size();
    auto end = url.find_first_of("/?#", start); // any of /?#, path/query/fragment
    return url.substr(start, end == string::npos ? string::npos : end - start);
}

// T: O(V+E) expect, S: O(V+D), D: largest link list
vector<string> crawl(const string& start, const HtmlParser& parser) {
    string host = hostOf(start);
    std::unordered_set<string> seen = {start};
    std::queue<string> q;
    q.push(start);

    vector<string> result;
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        result.push_back(cur);

        for (const string& nxt : parser.getUrls(cur)) {
            if (hostOf(nxt) == host && seen.insert(nxt).second) {
                q.push(nxt);
            }
        }
    }
    return result;
}

int main() {
    HtmlParser parser;
    parser.links = {
        {"http://a.com/", {"http://a.com/x", "http://b.com/"}},
        {"http://a.com/x", {"http://a.com/", "http://a.com/y"}}
    };

    vector<string> want = {
        "http://a.com/", "http://a.com/x", "http://a.com/y"
    };

    auto result = crawl("http://a.com/", parser);
    std::sort(result.begin(), result.end());
    assert(result == want); // cycle handed, exclude other host

    // no outgoing links: return start itself
    // no path of start
}
