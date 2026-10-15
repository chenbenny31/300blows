// ByteDance (hack2hire) / LC 337: houses form a binary tree; robbing two directly
// linked houses alerts the police. Return the max amount robbed.
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
using std::cout;
using std::string;
using std::pair;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// returns {best if node is robbed, best if node is skipped} for node's subtree
pair<int, int> dfs(TreeNode* node) {
    if (!node) { return {0, 0}; }
    auto [robL, skipL] = dfs(node->left);
    auto [robR, skipR] = dfs(node->right);
    // robbing node forces both children to be skipped
    int rob = node->val + skipL + skipR;
    int skip = max(robL, skipL) + max(robR, skipR);
    return {rob, skip};
}

// Postorder tree DP with (rob, skip) per node. T: O(n), S: O(h).
int rob(TreeNode* root) {
    auto [robRoot, skipRoot] = dfs(root);
    return max(robRoot, skipRoot);
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (rob(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //     3
    //   2   3
    //    3    1
    TreeNode a4{3, nullptr, nullptr}, a5{1, nullptr, nullptr};
    TreeNode a2{2, nullptr, &a4}, a3{3, nullptr, &a5};
    TreeNode a1{3, &a2, &a3};
    test("lc ex1", &a1, 7);
    //     3
    //   4   5
    //  1 3    1
    TreeNode b4{1, nullptr, nullptr}, b5{3, nullptr, nullptr}, b6{1, nullptr, nullptr};
    TreeNode b2{4, &b4, &b5}, b3{5, nullptr, &b6};
    TreeNode b1{3, &b2, &b3};
    test("lc ex2", &b1, 9);
}
