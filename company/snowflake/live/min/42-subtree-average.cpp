// B23 · LC 2265 sum/count pattern: integer average (truncating /) of every node's subtree average; equals root->val? Follow-up: floor instead of truncation.
#include <iostream>
#include <functional>
#include <climits>
using std::cout;
using std::pair;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// postorder returns (sum, count) per subtree, T: O(n), S: O(h)
bool rootEqualsAverageOfAverages(TreeNode* root) {
    if (root == nullptr) { return false; }
    long long avgSum = 0, nodes = 0;   // sum of subtree averages, n
    std::function<pair<long long, long long>(TreeNode*)> dfs = [&](TreeNode* node) -> pair<long long, long long> {
        if (node == nullptr) { return {0, 0}; }
        auto [ls, lc] = dfs(node->left);
        auto [rs, rc] = dfs(node->right);
        long long sum = ls + rs + node->val, count = lc + rc + 1;
        avgSum += sum / count;
        nodes++;
        return {sum, count};
    };
    dfs(root);
    return root->val == avgSum / nodes;
}

// floor division for b > 0; plain / truncates toward zero
long long floorDiv(long long a, long long b) {
    return a / b - (a % b != 0 && a < 0 ? 1 : 0);
}

// follow-up: same postorder, both averages floored, T: O(n), S: O(h)
bool rootEqualsAverageOfAveragesFloor(TreeNode* root) {
    if (root == nullptr) { return false; }
    long long avgSum = 0, nodes = 0;   // sum of floored subtree averages, n
    std::function<pair<long long, long long>(TreeNode*)> dfs = [&](TreeNode* node) -> pair<long long, long long> {
        if (node == nullptr) { return {0, 0}; }
        auto [ls, lc] = dfs(node->left);
        auto [rs, rc] = dfs(node->right);
        long long sum = ls + rs + node->val, count = lc + rc + 1;
        avgSum += floorDiv(sum, count);
        nodes++;
        return {sum, count};
    };
    dfs(root);
    return root->val == floorDiv(avgSum, nodes);
}

void test(TreeNode* root, bool expect) {
    bool got = rootEqualsAverageOfAverages(root);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testFloor(TreeNode* root, bool expect) {
    bool got = rootEqualsAverageOfAveragesFloor(root);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    TreeNode a1(1), a0(0), a4(4);           // chain 1-0-4: averages 4, 2, 1 -> 7/3 = 2
    a1.left = &a0;
    a0.left = &a4;
    TreeNode c2(2), c1(1), c3(3);           // 2 with children 1, 3: averages 1, 3, 2 -> 2
    c2.left = &c1;
    c2.right = &c3;
    TreeNode m1(INT_MAX), m2(INT_MAX), m3(INT_MAX);
    m1.left = &m2;
    m1.right = &m3;
    TreeNode e1(-1), e0(0);                 // trunc averages 0, 0 -> 0; floor 0, -1 -> -1
    e1.left = &e0;
    TreeNode f2(-2), f3(-3);                // trunc averages -3, -2 -> -2; floor -3, -3 -> -3
    f2.left = &f3;

    test(&c2, true);                        // target true case
    test(&a1, false);                       // root == whole-tree average (5/3 = 1) but not ours
    test(nullptr, false);                   // empty tree
    test(&m1, true);                        // subtree sum 3 * INT_MAX needs long long

    testFloor(&c2, true);                   // same as truncation on non-negatives
    testFloor(&e1, true);                   // trunc says false, floor true
    testFloor(&f2, false);                  // trunc says true, floor false
}
