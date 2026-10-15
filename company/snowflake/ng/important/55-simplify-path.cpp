// LC tag · LC 71: absolute Unix path -> canonical path. "." stays, ".." goes up one
// level (never above root), repeated '/' count as one, no trailing '/'; "..." is a name.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Split on '/' with a char loop; stack of directory names. T: O(n), S: O(n).
string simplifyPath(const string& path) {
    vector<string> parts;   // directory names from root to the current dir
    string name;            // name being read between two slashes
    // trailing '/' flushes the last name
    for (char c : path + "/") {
        if (c != '/') {
            name += c;
            continue;
        }
        if (name == "..") {
            if (!parts.empty()) { parts.pop_back(); }   // ".." at root stays at root
        } else if (!name.empty() && name != ".") {
            parts.push_back(name);
        }
        name.clear();
    }
    string result;
    for (const string& p : parts) { result += "/" + p; }
    return result.empty() ? "/" : result;
}

void test(const string& label, const string& path, const string& want) {
    cout << label << (simplifyPath(path) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dot dotdot", "/a/./b/../../c/", "/c");
    test("double slash", "/home//foo/", "/home/foo");
    test("above root", "/../", "/");
    test("three dots", "/.../a", "/.../a");
}
