#include <cassert>
#include <vector>

using std::vector;

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
};

// T: O(1), S: O(1)
bool isLeaf(const Node* node) {
    return !node->left && !node->right;
}

// T: O(n), S: O(h) recursion
void addLeaves(const Node* node, vector<int>& result) {
    if (!node) { return; }
    if (isLeaf(node)) {
        result.push_back(node->val);
        return;
    }
    addLeaves(node->left, result);
    addLeaves(node->right, result);
}

// T: O(n), S: O(n)
vector<int> boundary(const Node* root) {
    if (!root) { return {}; }
    vector<int> result = {root->val};
    if (isLeaf(root)) { return result; }

    const Node* cur = root->left;
    while (cur && !isLeaf(cur)) {
        result.push_back(cur->val);
        cur = cur->left ? cur->left : cur->right;
    }

    addLeaves(root, result);

    vector<int> right;
    cur = root->right;
    while (cur && !isLeaf(cur)) {
        right.push_back(cur->val);
        cur = cur->right ? cur->right : cur->left;
    }
    result.insert(result.end(), right.rbegin(), right.rend());
    return result;
}

// T: O(n), S: O(n)
long long boundarySum(const Node* root) {
    long long sum = 0;
    for (int value : boundary(root)) { sum += value; }
    return sum;
}

int main() {
    // Tree: 1 -> (2,3), 2 -> right 4, 3 -> left 5.
    Node a{1}, b{2}, c{3}, d{4}, e{5};
    a.left = &b;
    a.right = &c;
    b.right = &d;
    c.left = &e;

    vector<int> expected = {1, 2, 4, 5, 3};
    assert(boundary(&a) == expected);
    // Both side fallbacks; leaves counted once; right side reversed.

    a.left = nullptr;
    expected = {1, 5, 3};
    assert(boundary(&a) == expected); // No left boundary.

    expected = {4};
    assert(boundary(&d) == expected); // Root is also a leaf.
    assert(boundary(nullptr).empty());
}
