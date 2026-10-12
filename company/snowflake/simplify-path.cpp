#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;

// T: O(n), S: O(n)
string simplifyPath(const string& path) {
    vector<string> parts; // stack from root to cur loc
    int n = static_cast<int>(path.size());

    int i = 0;
    while (i < n) {
        while (i < n && path[i] == '/') { i++; }

        int start = i;
        while (i < n && path[i] != '/') { i++; }

        string name = path.substr(start, i - start);
        if (name == "..") {
            if (!parts.empty()) {
                parts.pop_back();
            }
        } else if (!name.empty() && name != ".") {
            parts.push_back(name);
        }
    }

    string result;
    for (const string& name : parts) {
        result += "/";
        result += name;
    }
    return result.empty() ? "/" : result;
}

int main() {
    // Repeated slashes, ".", "..", ordinary "..." name, trailing slash.
    assert(simplifyPath("/a//./b/../.../") == "/a/...");

    // Cannot move above root.
    assert(simplifyPath("/a/../../") == "/");

    assert(simplifyPath("/") == "/"); // Root itself.
}
