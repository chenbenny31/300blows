// ByteDance (hack2hire) / LC 113: return every root-to-leaf path of a binary tree whose
// node values sum to targetSum.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

// path = values from the root down to node; remain = target minus the sum above node
void dfs(TreeNode* node, int remain, vector<int>& path, vector<vector<int>>& result) {
    if (!node) { return; }
    path.push_back(node->val);
    remain -= node->val;
    // only a leaf may end a path
    if (!node->left && !node->right && remain == 0) { result.push_back(path); }
    dfs(node->left, remain, path, result);
    dfs(node->right, remain, path, result);
    path.pop_back();
}

// DFS backtracking; each hit copies a path of length <= h. T: O(n*h), S: O(h) + output.
vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(root, targetSum, path, result);
    return result;
}

void test(const string& label, TreeNode* root, int targetSum,
          const vector<vector<int>>& want) {
    cout << label << (pathSum(root, targetSum) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n7{7}, n2{2}, n13{13}, n5{5}, n1{1};
    TreeNode n11{11, &n7, &n2};
    TreeNode n4b{4, &n5, &n1};
    TreeNode n4{4, &n11};
    TreeNode n8{8, &n13, &n4b};
    TreeNode root{5, &n4, &n8};
    test("lc ex1", &root, 22, {{5, 4, 11, 2}, {5, 8, 4, 5}});
    TreeNode one{1};
    test("single node", &one, 1, {{1}});
}
