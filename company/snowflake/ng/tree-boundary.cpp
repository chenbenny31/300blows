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
    TreeNode(int v) : val(v) {}
};

bool isLeaf(TreeNode* node) {
    return !node->left && !node->right;
}

// preorder visit leaves, left to right
void addLeaves(TreeNode* node, vector<int>& leaves) {
    if (!node) { return; }
    if (isLeaf(node)) {
        leaves.push_back(node->val);
        return;
    }

    addLeaves(node->left, leaves);
    addLeaves(node->right, leaves);
}

// collect root,leftmost-side, leaves, rightmost-side, T: O(n), S: O(h)
vector<int> boundary(TreeNode* root) {
    if (!root) { return {}; }
    vector<int> result = {root->val};
    if (isLeaf(root)) { return result; }

    TreeNode* cur = root->left;
    while (cur && !isLeaf(cur)) {
        result.push_back(cur->val);
        cur = cur->left ? cur->left : cur->right;
    }

    addLeaves(root, result);

    vector<int> right; // need to reverse before merge
    cur = root->right;
    while (cur && !isLeaf(cur)) {
        right.push_back(cur->val);
        cur = cur->right ? cur->right : cur->left;
    }

    result.insert(result.end(), right.rbegin(), right.rend());
    return result;
}

// follow-up: boundary-sum
long long boundarySum(TreeNode* root) {
    long long sum = 0;
    for (int val : boundary(root)) {
        sum += val;
    }
    return sum;
}

void test(const string& label, TreeNode* root, const vector<int>& want) {
    cout << label << (boundary(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a1(1), a2(2), a3(3), a4(4), a5(5), a6(6), a7(7);
    a1.left = &a2; a1.right = &a3;
    a2.left = &a4; a2.right = &a5;
    a3.left = &a6; a3.right = &a7;
    test("perfect", &a1, {1, 2, 4, 5, 6, 7, 3});

    TreeNode b1(1), b2(2), b3(3), b4(4);
    b1.right = &b2;
    b2.left = &b3; b2.right = &b4;
    test("no left child", &b1, {1, 3, 4, 2});

    TreeNode c1(1);
    test("single", &c1, {1});
}

