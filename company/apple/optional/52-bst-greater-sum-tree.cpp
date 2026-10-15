// LC 1038: turn a BST into a greater-sum tree: each node's val becomes its original val
// plus the sum of all larger vals.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// reverse inorder (right, node, left) visits vals in descending order
void visit(TreeNode* node, int& sum) {
    if (!node) { return; }
    visit(node->right, sum);
    sum += node->val;   // sum = all vals >= this node's
    node->val = sum;
    visit(node->left, sum);
}

// Reverse inorder with a running sum. T: O(n), S: O(h).
TreeNode* bstToGst(TreeNode* root) {
    int sum = 0;
    visit(root, sum);
    return root;
}

void inorder(TreeNode* node, vector<int>& out) {
    if (!node) { return; }
    inorder(node->left, out);
    out.push_back(node->val);
    inorder(node->right, out);
}

void test(const string& label, TreeNode* root, const vector<int>& want) {
    vector<int> got;
    inorder(bstToGst(root), got);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //     2
    //   1   3
    TreeNode n1{1, nullptr, nullptr};
    TreeNode n3{3, nullptr, nullptr};
    TreeNode n2{2, &n1, &n3};
    test("balanced", &n2, {6, 5, 3});
    // 1 -> 2 (right chain)
    TreeNode c2{2, nullptr, nullptr};
    TreeNode c1{1, nullptr, &c2};
    test("right chain", &c1, {3, 2});
    test("empty", nullptr, {});
}
