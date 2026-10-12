#include <cassert>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class FileSystem {
private:
    struct Node {
        map<string, Node> children;
        bool isFile = false;
        string content;
    };

    Node root;

    // T: O(LlogC), S: O(l)
    Node* walk(const string& path, bool create) {
        Node* cur = &root;
        int n = static_cast<int>(path.size());
        int i = 1; // skip leading '/'

        while (i < n) {
            int start = i;
            while (i < n && path[i] != '/') {
                i++;
            }
            string name = path.substr(start, i - start);

            if (create) {
                cur = &cur->children[name];
            } else {
                auto it = cur->children.find(name);
                if (it == cur->children.end()) { return nullptr; }
                cur = &it->second;
            }
            i++;
        }
        return cur;
    }

public:
    // T: O(L log C + C), excluding output string copies.
    vector<string> ls(const string& path) {
        Node* node = walk(path, false);
        if (!node) {
            return {};
        }
        if (node->isFile) {
            return {path.substr(path.rfind('/') + 1)};
        }

        vector<string> names;
        for (const auto& entry : node->children) {
            names.push_back(entry.first);
        }
        return names;
    }

    // T: O(L log C).
    void mkdir(const string& path) {
        walk(path, true);
    }

    // T: O(L log C + A) amortized; A = appended content length.
    void addContentToFile(const string& path, const string& content) {
        Node* node = walk(path, true);
        node->isFile = true;
        node->content += content;
    }

    // T: O(L log C + F); F = file content length.
    string readContentFromFile(const string& path) {
        Node* node = walk(path, false);
        return node ? node->content : "";
    }
};


int main() {
    FileSystem fs;
    assert(fs.ls("/").empty());

    fs.mkdir("/a/b");
    fs.addContentToFile("/a/z", "hello");
    fs.addContentToFile("/a/z", " world");
    assert(fs.readContentFromFile("/a/z") == "hello world");

    vector<string> expected = {"b", "z"};
    assert(fs.ls("/a") == expected); // Sorted listing.

    expected = {"z"};
    assert(fs.ls("/a/z") == expected); // File listing.

    assert(fs.ls("/missing").empty());
    expected = {"a"};
    assert(fs.ls("/") == expected); // Read didn't create a directory.
}
