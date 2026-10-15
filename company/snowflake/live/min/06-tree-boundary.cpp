// A14 · LC 545: anticlockwise boundary: root, left side top-down (no leaves), leaves left to right, right side bottom-up (no leaves). Follow-up C11: boundary sum.
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

bool isLeaf(TreeNode* node) { return node->left == nullptr && node->right == nullptr; }

// preorder visits leaves left to right, T: O(n), S: O(h)
void addLeaves(TreeNode* node, vector<int>& out) {
    if (node == nullptr) { return; }
    if (isLeaf(node)) {
        out.push_back(node->val);
        return;
    }
    addLeaves(node->left, out);
    addLeaves(node->right, out);
}

// three walks: left side, leaves by DFS, right side reversed, T: O(n), S: O(h) + O(n) output
vector<int> boundary(TreeNode* root) {
    if (root == nullptr) { return {}; }
    vector<int> out = {root->val};
    if (isLeaf(root)) { return out; }   // a lone root is not also a leaf
    // stop before the leaf so addLeaves adds it once
    for (TreeNode* cur = root->left; cur != nullptr && !isLeaf(cur); cur = cur->left ? cur->left : cur->right) {
        out.push_back(cur->val);
    }
    addLeaves(root, out);
    vector<int> right;                  // right side top-down, appended reversed
    for (TreeNode* cur = root->right; cur != nullptr && !isLeaf(cur); cur = cur->right ? cur->right : cur->left) {
        right.push_back(cur->val);
    }
    out.insert(out.end(), right.rbegin(), right.rend());
    return out;
}

// follow-up: sum the boundary list (each node appears once), T: O(n), S: O(n)
long long boundarySum(TreeNode* root) {
    long long sum = 0;
    for (int v : boundary(root)) { sum += v; }
    return sum;
}

void test(const string& label, TreeNode* root, const vector<int>& expect) {
    vector<int> got = boundary(root);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

void testSum(TreeNode* root, long long expect) {
    long long got = boundarySum(root);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    TreeNode a1(1), a2(2), a3(3), a4(4), a5(5), a6(6), a7(7);   // perfect: 1 -> (2, 3), 2 -> (4, 5), 3 -> (6, 7)
    a1.left = &a2; a1.right = &a3;
    a2.left = &a4; a2.right = &a5;
    a3.left = &a6; a3.right = &a7;
    TreeNode b1(1), b2(2), b3(3), b4(4), b5(5), b6(6), b7(7), b8(8), b9(9), b10(10);   // LC 545 example 2
    b1.left = &b2; b1.right = &b3;
    b2.left = &b4; b2.right = &b5;
    b5.left = &b7; b5.right = &b8;
    b3.left = &b6;
    b6.left = &b9; b6.right = &b10;
    TreeNode c1(1), c2(2), c3(3), c4(4);                        // LC 545 example 1: root has only a right subtree
    c1.right = &c2;
    c2.left = &c3; c2.right = &c4;
    TreeNode solo(5);

    test("perfect", &a1, {1, 2, 4, 5, 6, 7, 3});                // target shape
    test("lc ex2", &b1, {1, 2, 4, 7, 8, 9, 10, 6, 3});          // inner nodes skipped, right side falls back to left child
    test("lc ex1", &c1, {1, 3, 4, 2});                          // no left child -> no left boundary
    test("single", &solo, {5});                                 // root only, not also a leaf
    test("empty", nullptr, {});                                 // null tree

    testSum(&a1, 28);                                           // every node of the perfect tree once
    testSum(&b1, 50);                                           // inner node 5 excluded
    testSum(&solo, 5);                                          // root not double counted
}
