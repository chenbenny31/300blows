// binary tree with unique values; shortest directions from node startValue to node destValue
// 'L' = go to left child, 'R' = right child, 'U' = parent (LC2096)

// values unique? both start and dest always present?
// start == dest: empty string?
// any shortest path, or this exact letter format?
// how deep can the tree get?

#include <string>
#include <cassert>
#include <iostream>

using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class TreeNavigator {
public:
    // root->start and root->dest as L/R strings; drop the shared prefix (path down to the LCA)
    // what is left of the start path becomes 'U's, what is left of the dest path is walked as is
    // T: O(n), S: O(h) recursion + paths
    string getDirections(TreeNode* root, int startValue, int destValue) {
        string toStart;
        string toDest;
        findPath(root, startValue, toStart);
        findPath(root, destValue, toDest);

        int common = 0; // length of the shared prefix
        while (common < static_cast<int>(toStart.size()) && common < static_cast<int>(toDest.size()) &&
               toStart[common] == toDest[common]) {
            common++;
        }
        return string(toStart.size() - common, 'U') + toDest.substr(common);
    }

private:
    // backtracking: path holds the moves from root to node; false means target is not below node
    bool findPath(TreeNode* node, int target, string& path) {
        if (node == nullptr) {
            return false;
        }
        if (node->val == target) {
            return true;
        }
        path.push_back('L');
        if (findPath(node->left, target, path)) {
            return true;
        }
        path.back() = 'R';
        if (findPath(node->right, target, path)) {
            return true;
        }
        path.pop_back();
        return false;
    }
};

int main() {
    TreeNavigator tn;

    // 5 -> (1, 2), 1 -> left 3, 2 -> (6, 4)
    TreeNode n5(5), n1(1), n2(2), n3(3), n6(6), n4(4); // stack nodes, nothing to free
    n5.left = &n1;
    n5.right = &n2;
    n1.left = &n3;
    n2.left = &n6;
    n2.right = &n4;

    assert(tn.getDirections(&n5, 3, 6) == "UURL"); // target: LC example, up to root then down
    assert(tn.getDirections(&n5, 5, 4) == "RR"); // edge: start is an ancestor, no 'U'
    assert(tn.getDirections(&n5, 6, 2) == "U"); // edge: dest is an ancestor, only 'U'
    assert(tn.getDirections(&n5, 4, 4) == ""); // null: start == dest

    std::cout << "all tests passed\n";
    return 0;
}
