// B07 · LC 588: in-memory file system with ls, mkdir, addContentToFile (append),
// readContentFromFile. ls on a directory lists names sorted; on a file, just its name.
#include <iostream>
#include <vector>
#include <string>
#include <map>
using std::cout;
using std::string;
using std::vector;
using std::map;

class FileSystem {
    struct Node {
        map<string, int> children;   // name -> node index, sorted for ls
        bool isFile = false;
        string content;
    };
    // node pool, nodes[0] = root; indices stay valid when the vector grows
    vector<Node> nodes = {Node()};

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

    // Walk the path from root, creating missing nodes. T: O(depth * log fanout).
    int walk(const string& path) {
        int cur = 0;
        for (const string& name : split(path)) {
            auto it = nodes[cur].children.find(name);
            if (it != nodes[cur].children.end()) {
                cur = it->second;
                continue;
            }
            nodes.push_back(Node());
            int id = nodes.size() - 1;
            nodes[cur].children[name] = id;   // re-index nodes[cur] after push_back
            cur = id;
        }
        return cur;
    }

public:
    // T: O(depth + k) for k listed names.
    vector<string> ls(const string& path) {
        int cur = walk(path);
        if (nodes[cur].isFile) { return {split(path).back()}; }
        vector<string> names;
        for (const auto& [name, id] : nodes[cur].children) { names.push_back(name); }
        return names;
    }

    void mkdir(const string& path) { walk(path); }

    void addContentToFile(const string& path, const string& content) {
        int cur = walk(path);
        nodes[cur].isFile = true;
        nodes[cur].content += content;
    }

    string readContentFromFile(const string& path) { return nodes[walk(path)].content; }
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
