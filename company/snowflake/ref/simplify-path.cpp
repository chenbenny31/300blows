// absolute Unix path -> canonical path (LC71)
// "." stays, ".." goes up one level (never above root), repeated '/' count as one, no trailing '/'

// input always absolute (starts with '/')?
// ".." at the root: stay at root?
// names like "..." are plain names?
// symlinks? (no, purely textual)

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class PathSimplifier {
public:
    // split on '/', keep a stack of names, T: O(n), S: O(n)
    string simplifyPath(const string& path) {
        vector<string> parts; // directory names from root to current
        int n = static_cast<int>(path.size());
        int i = 0;
        while (i < n) {
            while (i < n && path[i] == '/') {
                i++; // skip any run of slashes
            }
            int j = i;
            while (j < n && path[j] != '/') {
                j++;
            }
            string name = path.substr(i, j - i);
            if (name == "..") {
                if (!parts.empty()) {
                    parts.pop_back(); // at root, ".." stays at root
                }
            } else if (!name.empty() && name != ".") {
                parts.push_back(name);
            }
            i = j;
        }

        string out;
        for (const string& p : parts) {
            out += "/" + p;
        }
        return out.empty() ? "/" : out;
    }
};

int main() {
    PathSimplifier ps;

    assert(ps.simplifyPath("/a/./b/../../c/") == "/c"); // target: . and .. together
    assert(ps.simplifyPath("/home//foo/") == "/home/foo"); // edge: double slash, trailing slash
    assert(ps.simplifyPath("/../") == "/"); // fault: .. above root
    assert(ps.simplifyPath("/...") == "/..."); // edge: "..." is a plain name
    assert(ps.simplifyPath("/") == "/"); // null: root only

    std::cout << "all tests passed\n";
    return 0;
}
