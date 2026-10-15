// ByteDance (hack2hire) / LC 105: build the binary tree from its preorder and inorder
// traversals; all values are distinct.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// builds the subtree whose inorder is in[inL..inR]; pre = next root in preorder
TreeNode* build(const vector<int>& preorder, int& pre, int inL, int inR,
                unordered_map<int, int>& inIdx) {
    if (inL > inR) { return nullptr; }
    TreeNode* root = new TreeNode{preorder[pre++], nullptr, nullptr};
    int mid = inIdx[root->val];
    // preorder is root, left, right: the left subtree must consume pre first
    root->left = build(preorder, pre, inL, mid - 1, inIdx);
    root->right = build(preorder, pre, mid + 1, inR, inIdx);
    return root;
}

// Recursion with an inorder index map and a preorder cursor. T: O(n), S: O(n).
TreeNode* buildTree(const vector<int>& preorder, const vector<int>& inorder) {
    unordered_map<int, int> inIdx;   // inIdx[v] = index of v in inorder
    int n = inorder.size();
    for (int i = 0; i < n; i++) { inIdx[inorder[i]] = i; }
    int pre = 0;
    return build(preorder, pre, 0, n - 1, inIdx);
}

// level order with -1 for a missing child, trailing -1s trimmed
vector<int> levelOrder(TreeNode* root) {
    vector<int> out;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* cur = q.front();
        q.pop();
        out.push_back(cur ? cur->val : -1);
        if (!cur) { continue; }
        q.push(cur->left);
        q.push(cur->right);
    }
    while (!out.empty() && out.back() == -1) { out.pop_back(); }
    return out;
}

void freeTree(TreeNode* node) {
    if (!node) { return; }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void test(const string& label, const vector<int>& pre, const vector<int>& in,
          const vector<int>& want) {
    TreeNode* root = buildTree(pre, in);
    cout << label << (levelOrder(root) == want ? " PASS\n" : " FAIL\n");
    freeTree(root);
}

int main() {
    test("lc ex1", {3, 9, 20, 15, 7}, {9, 3, 15, 20, 7}, {3, 9, 20, -1, -1, 15, 7});
    test("left chain", {1, 2, 3}, {3, 2, 1}, {1, 2, -1, 3});
    test("single", {1}, {1}, {1});
}
