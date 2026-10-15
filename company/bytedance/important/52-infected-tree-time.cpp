// ByteDance / LC 2385: an infection starts at the node with value start (values unique);
// each minute it spreads to every uninfected neighbor (parent or child). Return the
// minutes until the whole tree is infected.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// adds an undirected edge between each node and each of its children
void build(TreeNode* node, unordered_map<int, vector<int>>& graph) {
    if (!node) { return; }
    for (TreeNode* child : {node->left, node->right}) {
        if (!child) { continue; }
        graph[node->val].push_back(child->val);
        graph[child->val].push_back(node->val);
        build(child, graph);
    }
}

// Tree -> undirected graph, then BFS layers from start. T: O(n), S: O(n).
int amountOfTime(TreeNode* root, int start) {
    // graph[v] = values of v's parent and children (undirected tree edges)
    unordered_map<int, vector<int>> graph;
    build(root, graph);
    unordered_set<int> seen{start};          // values already infected
    queue<int> q;
    q.push(start);
    int minutes = -1;   // the start layer itself takes no time
    while (!q.empty()) {
        minutes++;
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (seen.insert(nxt).second) { q.push(nxt); }
            }
        }
    }
    return minutes;
}

void test(const string& label, TreeNode* root, int start, int want) {
    cout << label << (amountOfTime(root, start) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //       1
    //     5   3
    //      4 10 6
    //     9 2
    TreeNode a9{9, nullptr, nullptr}, a2{2, nullptr, nullptr};
    TreeNode a4{4, &a9, &a2}, a10{10, nullptr, nullptr}, a6{6, nullptr, nullptr};
    TreeNode a5{5, nullptr, &a4}, a3{3, &a10, &a6}, a1{1, &a5, &a3};
    test("lc ex1", &a1, 3, 4);
    TreeNode b1{1, nullptr, nullptr};
    test("single", &b1, 1, 0);
}
