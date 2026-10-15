#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

class FileSystem {
private:
    struct Node {
        map<string, Node> children; // sorted key for ls
        bool isFile = false;
        string content;
    };

    Node root;

    // "/a/b/c" -> {"a", "b", "c"}
    vector<string> split(const string& path) {
        vector<string> parts;
        string name;

        for (char c : path) {
            if (c != '/') {
                name += c;
                continue;
            }
            if (!name.empty()) { parts.push_back(name); }
            name.clear();
        }
        if (!name.empty()) { parts.push_back(name); }
        return parts;
    }

    // walk from root, [] to create missing nodes
    Node* walk(const string& path) {
        Node* cur = &root;
        for (const string& name : split(path)) {
            cur = &cur->children[name];
        }
        return cur;
    }

public:
    vector<string> ls(const string& path) {
        Node* cur = walk(path);
        if (cur->isFile) {
            return {split(path).back()};
        }

        vector<string> names;
        for (const auto& [name, child] : cur->children) {
            names.push_back(name);
        }
        return names;
    }

    void mkdir(const string& path) {
        walk(path);
    }

    void addContentToFile(const string& path, const string& content) {
        Node* cur = walk(path);
        cur->isFile = true;
        cur->content += content; // append
    }

    string readContentFromFile(const string& path) {
        return walk(path)->content;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    FileSystem fs;
    fs.mkdir("/home/user");
    fs.addContentToFile("/home/user/notes.txt", "hi");
    check(fs.ls("/") == vector<string>{"home"}, "ls root");
    check(fs.ls("/home/user/notes.txt") == vector<string>{"notes.txt"}, "ls file");
    fs.addContentToFile("/home/user/notes.txt", " there");
    check(fs.readContentFromFile("/home/user/notes.txt") == "hi there", "append");
    fs.mkdir("/home/admin");
    check(fs.ls("/home") == vector<string>{"admin", "user"}, "ls sorted");
}
