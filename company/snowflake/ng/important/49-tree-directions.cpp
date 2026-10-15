// LC tag · LC 2096: binary tree with unique values; return the shortest directions
// from node startValue to node destValue: 'L' / 'R' = left / right child, 'U' = parent.
#include <iostream>
#include <string>
using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Backtracking: path = moves from the root to node; false if target is not below node.
bool findPath(TreeNode* node, int target, string& path) {
    if (node == nullptr) { return false; }
    if (node->val == target) { return true; }
    path.push_back('L');
    if (findPath(node->left, target, path)) { return true; }
    // left failed: reuse the slot for 'R' before searching right
    path.back() = 'R';
    if (findPath(node->right, target, path)) { return true; }
    path.pop_back();
    return false;
}

// Root paths to both nodes, drop the common prefix (path to the LCA). T: O(n), S: O(n).
string getDirections(TreeNode* root, int startValue, int destValue) {
    string toStart;   // moves from root to start
    string toDest;    // moves from root to dest
    findPath(root, startValue, toStart);
    findPath(root, destValue, toDest);
    int common = 0;   // length of the shared prefix
    while (common < (int)toStart.size() && common < (int)toDest.size()
           && toStart[common] == toDest[common]) {
        common++;
    }
    // leftover start moves are climbed back up; leftover dest moves are walked down
    return string(toStart.size() - common, 'U') + toDest.substr(common);
}

void test(const string& label, TreeNode* root, int start, int dest, const string& want) {
    cout << label << (getDirections(root, start, dest) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n5(5), n1(1), n2(2), n3(3), n6(6), n4(4);
    n5.left = &n1;
    n5.right = &n2;
    n1.left = &n3;
    n2.left = &n6;
    n2.right = &n4;
    test("lc ex1", &n5, 3, 6, "UURL");
    test("start above", &n5, 5, 4, "RR");
    test("dest above", &n5, 6, 2, "U");
    test("same node", &n5, 4, 4, "");
}
