// B07 / LC 588: in-memory file system with ls, mkdir, addContentToFile (append), readContentFromFile.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

struct Node {
    map<string, Node*> children;   // entry name -> child, sorted for ls
    bool isFile = false;
    string content;                // file body; empty for a directory
};

class FileSystem {
public:
    ~FileSystem() { freeChildren(&root); }

    // walk the trie; dir -> sorted child names, file -> its own name, T: O(p log c + c), S: O(c)
    vector<string> ls(const string& path) {
        Node* node = walk(path, false);
        if (node == nullptr) { return {}; }
        if (node->isFile) { return {path.substr(path.rfind('/') + 1)}; }
        vector<string> names;
        for (const auto& [name, child] : node->children) { names.push_back(name); }
        return names;
    }

    // create every missing component, T: O(p log c), S: O(p) new nodes
    void mkdir(const string& path) { walk(path, true); }

    // create file and missing parents, then append, T: O(p log c + len), S: O(len)
    void addContentToFile(const string& filePath, const string& content) {
        Node* node = walk(filePath, true);
        node->isFile = true;
        node->content += content;
    }

    // missing file -> "", T: O(p log c + len), S: O(len)
    string readContentFromFile(const string& filePath) {
        Node* node = walk(filePath, false);
        if (node == nullptr) { return ""; }
        return node->content;
    }

private:
    Node root;   // "/"

    // follow components from root; missing component -> create or nullptr, T: O(p log c), S: O(1)
    Node* walk(const string& path, bool create) {
        Node* cur = &root;
        int n = path.size();
        int i = 1;   // skip the leading '/'; "/" itself never enters the loop
        while (i < n) {
            int j = i;
            while (j < n && path[j] != '/') { j++; }
            string name = path.substr(i, j - i);
            auto it = cur->children.find(name);   // find, not [], so reads never create nodes
            if (it == cur->children.end()) {
                if (!create) { return nullptr; }
                it = cur->children.insert({name, new Node()}).first;
            }
            cur = it->second;
            i = j + 1;
        }
        return cur;
    }

    // post-order delete of every node below `node`, T: O(nodes), S: O(depth)
    void freeChildren(Node* node) {
        for (auto& [name, child] : node->children) {
            freeChildren(child);
            delete child;
        }
    }
};

bool check(const vector<string>& got, const vector<string>& expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got.size() << " names, expected " << expected.size() << "\n";
    return false;
}

bool check(const string& got, const string& expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got \"" << got << "\", expected \"" << expected << "\"\n";
    return false;
}

bool testLeetCodeExample() {
    bool ok = true;
    FileSystem fs;
    ok &= check(fs.ls("/"), {}, "ls / on fresh fs");
    fs.mkdir("/a/b/c");
    fs.addContentToFile("/a/b/c/d", "hello");
    ok &= check(fs.ls("/"), {"a"}, "ls / after mkdir");
    ok &= check(fs.readContentFromFile("/a/b/c/d"), "hello", "read d");
    return ok;
}

bool testFileAndAppend() {
    bool ok = true;
    FileSystem fs;
    fs.addContentToFile("/x/y/f.txt", "ab");   // parents created on demand
    ok &= check(fs.ls("/x/y/f.txt"), {"f.txt"}, "ls on file -> own name");
    ok &= check(fs.ls("/x"), {"y"}, "implicit parent dir");
    fs.addContentToFile("/x/y/f.txt", "");     // empty append
    fs.addContentToFile("/x/y/f.txt", "cd");
    ok &= check(fs.readContentFromFile("/x/y/f.txt"), "abcd", "appends, not overwrite");
    fs.mkdir("/x/y");                          // existing dir: no-op
    ok &= check(fs.ls("/x/y"), {"f.txt"}, "mkdir existing keeps contents");
    fs.addContentToFile("/top", "t");          // file directly under root
    ok &= check(fs.ls("/top"), {"top"}, "ls on root-level file");
    ok &= check(fs.ls("/"), {"top", "x"}, "root lists dir and file");
    return ok;
}

bool testSortedAndMissing() {
    bool ok = true;
    FileSystem fs;
    fs.mkdir("/d/b");
    fs.addContentToFile("/d/a10", "1");
    fs.mkdir("/d/a2");
    fs.addContentToFile("/d/B", "2");
    ok &= check(fs.ls("/d"), {"B", "a10", "a2", "b"}, "byte-wise order, files and dirs mixed");
    ok &= check(fs.ls("/d/a2"), {}, "empty dir");
    ok &= check(fs.ls("/nope"), {}, "ls missing path");
    ok &= check(fs.readContentFromFile("/d/zz"), "", "read missing file");
    ok &= check(fs.ls("/"), {"d"}, "reads did not create nope or zz");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testLeetCodeExample();   // LC example, fresh root
    ok &= testFileAndAppend();     // ls on file, append, implicit parents, mkdir no-op, root-level file
    ok &= testSortedAndMissing();  // lexicographic ls, empty dir, missing paths never created
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
