// ByteDance / LC 103: zigzag level order of a binary tree: level 0 left to right,
// level 1 right to left, and so on alternating.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// BFS by level; place each value by index, mirrored on odd levels. T: O(n), S: O(n).
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> result;   // result[d] = values of level d in zigzag order
    if (!root) { return result; }
    queue<TreeNode*> q;           // nodes of the current level, left to right
    q.push(root);
    bool leftToRight = true;
    while (!q.empty()) {
        int sz = q.size();
        vector<int> level(sz);
        for (int i = 0; i < sz; i++) {
            TreeNode* cur = q.front();
            q.pop();
            level[leftToRight ? i : sz - 1 - i] = cur->val;
            if (cur->left) { q.push(cur->left); }
            if (cur->right) { q.push(cur->right); }
        }
        result.push_back(level);
        leftToRight = !leftToRight;
    }
    return result;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (zigzagLevelOrder(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n15{15, nullptr, nullptr}, n7{7, nullptr, nullptr};
    TreeNode n9{9, nullptr, nullptr}, n20{20, &n15, &n7}, n3{3, &n9, &n20};
    test("lc ex1", &n3, {{3}, {20, 9}, {15, 7}});
    TreeNode one{1, nullptr, nullptr};
    test("single", &one, {{1}});
    test("empty", nullptr, {});
}
