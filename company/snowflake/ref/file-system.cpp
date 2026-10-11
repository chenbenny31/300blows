// Design ls(path), mkdir(path), addContentToFile(path, content), readContentFromFile(path) over absolute paths
// ls on file returns filename, ls on dir return entries sorted, mkdir creates every missing parent

// repeated write append or overwrite?
// ls or read return on missing path?
// path rules: absolute or including ., ..?
// ls's display order, string order or file/dir group?
// concurrent access support?

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

// T: O(L*logC) per op, L: path components, C: max entries in one dir
// S: O(nodes + content bytes), nodes are never freed

struct Node {
    std::map<string, Node*> children; // name -> child, map keeps ls sorted
    string content; // file body, empty for a dir
    bool isFile = false;
};

class FileSystem {
public:
    vector<string> ls(const string& path) {
        Node* node = walk(path, false);
        if (node == nullptr) {
            return {};
        }
        if (node->isFile) {
            return {path.substr(path.rfind('/') + 1)}; // the filename
        }
        vector<string> names;
        for (const auto& entry : node->children) {
            names.push_back(entry.first);
        }
        return names;
    }

    void mkdir(const string& path) {
        walk(path, true);
    }

    void addContentToFile(const string& path,const string& content) {
        Node* node = walk(path, true);
        node->isFile = true;
        node->content += content; // append
    }

    string readContentFromFile(const string& path) {
        Node* node = walk(path, false);
        if (node == nullptr) {
            return "";
        }
        return node->content;
    }

private:
    Node root;

    // follow path from root; create missing nodes only when asked, else nullptr
    Node* walk(const string& path, bool create) {
        Node* cur = &root;
        size_t i = 1; // skip leading '/'
        while (i < path.size()) {
            size_t j = path.find('/', i);
            if (j == string::npos) {
                j = path.size();
            }
            string name = path.substr(i, j - i);

            auto it = cur->children.find(name); // use find, not [], to avoid read auto insertion
            if (it == cur->children.end()) {
                if (!create) {
                    return nullptr;
                }
                // emplace() returns std::pair<iterator, bool>
                it = cur->children.emplace(name, new Node()).first; // ~ insert(std::make_pair(name, new Node())) without temp copy
            }
            cur = it->second;
            i = j + 1;
        }
        return cur;
    }
};

int main() {
    FileSystem fs;
    vector<string> want;

    assert(fs.ls("/").empty()); // fresh root

    fs.mkdir("/a/b/c"); // creates missing parent
    fs.addContentToFile("/a/b/c/d", "hello");
    want = {"a"};
    assert(fs.ls("/") == want);
    want = {"d"};
    assert(fs.ls("/a/b/c") == want);
    assert(fs.ls("/a/b/c/d") == want); // ls on a file returns its own name
    assert(fs.readContentFromFile("/a/b/c/d") == "hello");

    fs.addContentToFile("/a/b/c/d", " world"); // repeated write appends
    assert(fs.readContentFromFile("/a/b/c/d") == "hello world");

    fs.mkdir("/a/b/e");
    fs.mkdir("/a/b/c"); // existing dir: no-op
    fs.addContentToFile("/a/b/aa", "x");
    want = {"aa", "c", "e"};
    assert(fs.ls("/a/b") == want); // sorted, mixed files and dirs
    assert(fs.readContentFromFile("/a/b/c/d") == "hello world");

    fs.addContentToFile("/x/y.txt", "z"); // missing parent is created
    want = {"y.txt"};
    assert(fs.ls("/x") == want);

    assert(fs.ls("/nope").empty()); // missing path, a read cannot create it
    want = {"a", "x"};
    assert(fs.ls("/") == want);

    std::cout << "all tests passed\n";
    return 0;
}
