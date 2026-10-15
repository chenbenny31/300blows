// LC 889: build any binary tree matching distinct-valued preorder and postorder.
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

// builds pre[preL..preR] whose postorder starts at postL
TreeNode* build(const vector<int>& pre, int preL, int preR, int postL,
                unordered_map<int, int>& postIdx) {
    if (preL > preR) { return nullptr; }
    TreeNode* root = new TreeNode{pre[preL], nullptr, nullptr};
    if (preL == preR) { return root; }
    // pre[preL + 1] is the left child; it ends the left subtree in postorder
    int leftSize = postIdx[pre[preL + 1]] - postL + 1;
    root->left = build(pre, preL + 1, preL + leftSize, postL, postIdx);
    root->right = build(pre, preL + leftSize + 1, preR, postL + leftSize, postIdx);
    return root;
}

// Recursion with an index map of postorder. T: O(n), S: O(n).
TreeNode* constructFromPrePost(const vector<int>& pre, const vector<int>& post) {
    unordered_map<int, int> postIdx;   // postIdx[v] = index of v in post
    for (int i = 0; i < (int)post.size(); i++) { postIdx[post[i]] = i; }
    return build(pre, 0, (int)pre.size() - 1, 0, postIdx);
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

void test(const string& label, const vector<int>& pre, const vector<int>& post,
          const vector<int>& want) {
    TreeNode* root = constructFromPrePost(pre, post);
    cout << label << (levelOrder(root) == want ? " PASS\n" : " FAIL\n");
    freeTree(root);
}

int main() {
    test("lc ex1", {1, 2, 4, 5, 3, 6, 7}, {4, 5, 2, 6, 7, 3, 1}, {1, 2, 3, 4, 5, 6, 7});
    test("one child", {1, 2}, {2, 1}, {1, 2});
    test("single", {1}, {1}, {1});
}
