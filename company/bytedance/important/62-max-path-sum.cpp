// ByteDance (hack2hire) / LC 124: a path is any sequence of adjacent nodes, each used at
// most once, not necessarily through the root. Return the max path sum (tree non-empty).
// Follow-up: return the node values on the best path, in path order.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::reverse;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// returns the best downward chain sum starting at node; best = max path seen so far
int gain(TreeNode* node, int& best) {
    if (!node) { return 0; }
    int l = max(gain(node->left, best), 0);    // a negative branch is better dropped
    int r = max(gain(node->right, best), 0);
    best = max(best, node->val + l + r);       // path bending at node
    return node->val + max(l, r);              // a parent can extend only one side
}

// Postorder gains with a global best. T: O(n), S: O(h).
int maxPathSum(TreeNode* root) {
    int best = root->val;
    gain(root, best);
    return best;
}

// like gain, but down = the best downward chain (node first), bestPath = best path
int chain(TreeNode* node, vector<int>& down, int& best, vector<int>& bestPath) {
    down.clear();
    if (!node) { return 0; }
    vector<int> l, r;   // best downward chains of the children
    int lg = chain(node->left, l, best, bestPath);
    int rg = chain(node->right, r, best, bestPath);
    if (lg < 0) {
        lg = 0;
        l.clear();
    }
    if (rg < 0) {
        rg = 0;
        r.clear();
    }
    if (node->val + lg + rg > best) {
        best = node->val + lg + rg;
        // left chain runs upward to node, then down the right chain
        bestPath.assign(l.rbegin(), l.rend());
        bestPath.push_back(node->val);
        bestPath.insert(bestPath.end(), r.begin(), r.end());
    }
    down.push_back(node->val);
    const vector<int>& longer = lg >= rg ? l : r;
    down.insert(down.end(), longer.begin(), longer.end());
    return node->val + max(lg, rg);
}

// follow-up: same postorder, carrying chains to rebuild the path. T: O(n*h), S: O(n*h).
vector<int> maxPathNodes(TreeNode* root) {
    int best = root->val;
    vector<int> bestPath{root->val};   // node values on the best path so far
    vector<int> down;
    chain(root, down, best, bestPath);
    return bestPath;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (maxPathSum(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a2{2, nullptr, nullptr};
    TreeNode a3{3, nullptr, nullptr};
    TreeNode a1{1, &a2, &a3};
    test("through root", &a1, 6);
    //   -10
    //  9    20
    //     15   7
    TreeNode b15{15, nullptr, nullptr};
    TreeNode b7{7, nullptr, nullptr};
    TreeNode b20{20, &b15, &b7};
    TreeNode b9{9, nullptr, nullptr};
    TreeNode b10{-10, &b9, &b20};
    test("skip root", &b10, 42);
    TreeNode c{-3, nullptr, nullptr};
    test("single negative", &c, -3);
}
