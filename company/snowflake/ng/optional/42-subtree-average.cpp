// B23 (no direct LC; 2265 sum/count pattern): take every node's integer subtree average
// (sum / count, truncated), then the integer average of those n averages; return whether
// it equals root->val. Empty tree -> false.
#include <iostream>
#include <string>
#include <utility>
using std::cout;
using std::string;
using std::pair;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(int v) : val(v) {}
};

// Postorder: returns {sum, count} of node's subtree and adds its average to avgSum.
pair<long long, long long> dfs(TreeNode* node, long long& avgSum) {
    if (!node) { return {0, 0}; }
    auto [leftSum, leftCount] = dfs(node->left, avgSum);
    auto [rightSum, rightCount] = dfs(node->right, avgSum);
    long long sum = leftSum + rightSum + node->val;
    long long count = leftCount + rightCount + 1;
    avgSum += sum / count;
    return {sum, count};
}

// One postorder pass; long long since sums of int values overflow. T: O(n), S: O(h).
bool rootEqualsAvgOfAvgs(TreeNode* root) {
    if (!root) { return false; }
    long long avgSum = 0;   // sum of every subtree's average
    long long n = dfs(root, avgSum).second;
    return root->val == avgSum / n;
}

void test(const string& label, TreeNode* root, bool want) {
    cout << label << (rootEqualsAvgOfAvgs(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a2(2), a1(1), a3(3);
    a2.left = &a1; a2.right = &a3;
    test("balanced", &a2, true);

    TreeNode b1(1), b0(0), b4(4);
    b1.left = &b0;
    b0.left = &b4;
    test("not whole-tree avg", &b1, false);

    TreeNode c5(5);
    test("single", &c5, true);
    test("empty", nullptr, false);
}
