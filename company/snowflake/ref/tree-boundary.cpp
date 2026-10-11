// binary tree boundary, counter-clockwise from the root: root, left boundary top-down,
// leaves left to right, right boundary bottom-up; every node at most once (LC545)
// Q25 variant: return the sum of the boundary values instead of the list

// order counter-clockwise starting at root?
// root with no left child: no left boundary at all (LC rule)?
// a leaf that is also on a side: counted once, as a leaf?
// complete / triangle tree guaranteed, or any shape?
// return the list or the sum? values can overflow int?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class TreeBoundary {
public:
    // T: O(n), S: O(h) recursion for the leaves + O(n) output
    vector<int> boundary(TreeNode* root) {
        vector<int> out;
        if (root == nullptr) {
            return out;
        }
        out.push_back(root->val);
        if (isLeaf(root)) {
            return out; // single node: root is not also a leaf
        }

        // left side top-down, stop before the leaf so it isn't counted twice
        for (TreeNode* cur = root->left; cur != nullptr && !isLeaf(cur); cur = cur->left ? cur->left : cur->right) {
            out.push_back(cur->val);
        }
        addLeaves(root, out);
        vector<int> right; // right side top-down, appended reversed
        for (TreeNode* cur = root->right; cur != nullptr && !isLeaf(cur); cur = cur->right ? cur->right : cur->left) {
            right.push_back(cur->val);
        }
        out.insert(out.end(), right.rbegin(), right.rend());
        return out;
    }

    // Q25: each boundary node appears once in the list, so summing it can't double count
    long long boundarySum(TreeNode* root) {
        long long sum = 0;
        for (int v : boundary(root)) {
            sum += v;
        }
        return sum;
    }

private:
    bool isLeaf(TreeNode* node) {
        return node->left == nullptr && node->right == nullptr;
    }

    void addLeaves(TreeNode* node, vector<int>& out) { // preorder: leaves left to right
        if (node == nullptr) {
            return;
        }
        if (isLeaf(node)) {
            out.push_back(node->val);
            return;
        }
        addLeaves(node->left, out);
        addLeaves(node->right, out);
    }
};

int main() {
    TreeBoundary tb;
    vector<int> want;

    // 1 -> (2, 3), 2 -> (4, 5), 5 -> left 7, 3 -> right 6
    TreeNode n1(1), n2(2), n3(3), n4(4), n5(5), n6(6), n7(7); // stack nodes, nothing to free
    n1.left = &n2;
    n1.right = &n3;
    n2.left = &n4;
    n2.right = &n5;
    n5.left = &n7;
    n3.right = &n6;
    want = {1, 2, 4, 7, 6, 3};
    assert(tb.boundary(&n1) == want); // target: inner node 5 skipped, leaves 4 7 6
    assert(tb.boundarySum(&n1) == 23);

    TreeNode c1(1), c2(2), c3(3); // 1 -> right 2 -> right 3
    c1.right = &c2;
    c2.right = &c3;
    want = {1, 3, 2};
    assert(tb.boundary(&c1) == want); // edge: no left child, leaf 3 on the right side counted once

    TreeNode solo(5);
    want = {5};
    assert(tb.boundary(&solo) == want); // edge: single node is root only, not also a leaf

    assert(tb.boundary(nullptr).empty()); // null: empty tree
    assert(tb.boundarySum(nullptr) == 0);

    std::cout << "all tests passed\n";
    return 0;
}
