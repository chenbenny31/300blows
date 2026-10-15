// A14 · LC 545: anticlockwise boundary of a binary tree: root, left boundary top-down (no leaves),
// all leaves left to right, right boundary bottom-up (no leaves); each node once.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

bool isLeaf(TreeNode* node) { return node->left == nullptr && node->right == nullptr; }

class TreeBoundary {
public:
    // three walks: left side, leaves by DFS, right side reversed, T: O(n), S: O(h) stack + O(n) output
    vector<int> boundary(TreeNode* root) {
        if (root == nullptr) { return {}; }
        vector<int> out = {root->val};
        if (isLeaf(root)) { return out; }   // a lone root is not also a leaf
        // left side: prefer left child, stop before the leaf so it is added once, by addLeaves
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

    // C11 variant: sum of the boundary values, T: O(n), S: O(n)
    long long boundarySum(TreeNode* root) {
        long long sum = 0;
        for (int v : boundary(root)) { sum += v; }
        return sum;
    }

private:
    // preorder visits leaves left to right
    void addLeaves(TreeNode* node, vector<int>& out) {
        if (node == nullptr) { return; }
        if (isLeaf(node)) {
            out.push_back(node->val);
            return;
        }
        addLeaves(node->left, out);
        addLeaves(node->right, out);
    }
};

bool test(const string& label, TreeNode* root, const vector<int>& expected) {
    TreeBoundary tb;
    vector<int> got = tb.boundary(root);
    long long sum = tb.boundarySum(root);
    long long expectedSum = 0;
    for (int v : expected) { expectedSum += v; }
    if (got == expected && sum == expectedSum) { return true; }
    cout << "FAIL " << label << ": got size " << got.size() << ", expected size " << expected.size() << "\n";
    return false;
}

int main() {
    bool ok = true;

    // full, complete, balanced (the platform's version): 1 -> (2, 3), 2 -> (4, 5), 3 -> (6, 7)
    TreeNode a1(1), a2(2), a3(3), a4(4), a5(5), a6(6), a7(7);
    a1.left = &a2; a1.right = &a3;
    a2.left = &a4; a2.right = &a5;
    a3.left = &a6; a3.right = &a7;
    ok &= test("perfect", &a1, {1, 2, 4, 5, 6, 7, 3});                // target shape

    // LC 545 example 2: inner node 5 has leaves 7, 8; right side 3 -> 6 -> 10
    TreeNode b1(1), b2(2), b3(3), b4(4), b5(5), b6(6), b7(7), b8(8), b9(9), b10(10);
    b1.left = &b2; b1.right = &b3;
    b2.left = &b4; b2.right = &b5;
    b5.left = &b7; b5.right = &b8;
    b3.left = &b6;
    b6.left = &b9; b6.right = &b10;
    ok &= test("lc ex2", &b1, {1, 2, 4, 7, 8, 9, 10, 6, 3});          // inner nodes skipped, right side uses left child

    // LC 545 example 1: root with only a right subtree, 2 -> (3, 4)
    TreeNode c1(1), c2(2), c3(3), c4(4);
    c1.right = &c2;
    c2.left = &c3; c2.right = &c4;
    ok &= test("lc ex1", &c1, {1, 3, 4, 2});                          // no left child -> no left boundary

    TreeNode d1(1), d2(2), d3(3);                                     // left chain 1 -> 2 -> 3
    d1.left = &d2;
    d2.left = &d3;
    ok &= test("left chain", &d1, {1, 2, 3});                         // bottom of the chain counted once, as a leaf

    TreeNode solo(5);
    ok &= test("single", &solo, {5});                                 // root only, not also a leaf
    ok &= test("empty", nullptr, {});                                 // null tree
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
