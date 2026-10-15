// B23 (no direct LC; 2265 sum/count pattern): take every node's integer subtree average (sum / count), then the
// integer average of those n averages; return whether it equals root->val. Integer division truncates (C++ /).

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
using std::pair;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// floor division for b > 0; plain / truncates toward zero
long long floorDiv(long long a, long long b) {
    long long q = a / b;
    if (a % b != 0 && a < 0) { q--; }
    return q;
}

class SubtreeAverager {
public:
    // postorder returns (sum, count) per subtree, truncating averages, T: O(n), S: O(h)
    bool rootEqualsAverageOfAverages(TreeNode* root) {
        if (root == nullptr) { return false; }
        long long avgSum = 0;   // sum of every subtree's average
        long long nodes = 0;    // n
        collect(root, false, avgSum, nodes);
        return root->val == avgSum / nodes;
    }

    // follow-up: same postorder, both averages rounded down (floor), T: O(n), S: O(h)
    bool rootEqualsAverageOfAveragesFloor(TreeNode* root) {
        if (root == nullptr) { return false; }
        long long avgSum = 0;   // sum of every subtree's floored average
        long long nodes = 0;    // n
        collect(root, true, avgSum, nodes);
        return root->val == floorDiv(avgSum, nodes);
    }

private:
    // returns (sum, count) of node's subtree; adds its average into avgSum
    pair<long long, long long> collect(TreeNode* node, bool useFloor, long long& avgSum, long long& nodes) {
        if (node == nullptr) { return {0, 0}; }
        auto [leftSum, leftCount] = collect(node->left, useFloor, avgSum, nodes);
        auto [rightSum, rightCount] = collect(node->right, useFloor, avgSum, nodes);
        long long sum = leftSum + rightSum + node->val;
        long long count = leftCount + rightCount + 1;
        avgSum += useFloor ? floorDiv(sum, count) : sum / count;
        nodes++;
        return {sum, count};
    }
};

bool test(const string& label, TreeNode* root, bool expected, bool expectedFloor) {
    SubtreeAverager sa;
    bool got = sa.rootEqualsAverageOfAverages(root);
    bool gotFloor = sa.rootEqualsAverageOfAveragesFloor(root);
    if (got == expected && gotFloor == expectedFloor) { return true; }
    cout << "FAIL " << label << ": got " << got << " / " << gotFloor << ", expected " << expected << " / "
         << expectedFloor << "\n";
    return false;
}

int main() {
    bool ok = true;

    TreeNode a1(1), a0(0), a4(4);   // chain 1 -> 0 -> 4: averages 4, 2, 1 -> 7 / 3 = 2, root 1
    a1.left = &a0;
    a0.left = &a4;
    ok &= test("chain 1-0-4", &a1, false, false);    // root == whole-tree average (5/3 = 1) but not ours

    TreeNode b2(2), b4(4), b0(0);   // chain 2 -> 4 -> 0: averages 0, 2, 2 -> 4 / 3 = 1, root 2
    b2.left = &b4;
    b4.left = &b0;
    ok &= test("chain 2-4-0", &b2, false, false);    // whole-tree average is 2 == root; double averaging differs

    TreeNode c2(2), c1(1), c3(3);   // 2 with children 1, 3: averages 1, 3, 2 -> 6 / 3 = 2
    c2.left = &c1;
    c2.right = &c3;
    ok &= test("balanced 2-1-3", &c2, true, true);   // true case

    TreeNode d5(5);
    ok &= test("single", &d5, true, true);           // smallest tree: average of itself
    ok &= test("empty", nullptr, false, false);      // empty tree -> false by contract

    TreeNode e1(-1), e0(0);         // -1 -> 0: trunc averages 0, 0 -> 0; floor 0, -1 -> -1
    e1.left = &e0;
    ok &= test("neg -1,0", &e1, false, true);        // truncation and floor disagree

    TreeNode f2(-2), f3(-3);        // -2 -> -3: trunc averages -3, -2 -> -2; floor -3, -3 -> -3
    f2.left = &f3;
    ok &= test("neg -2,-3", &f2, true, false);       // disagreement in the other direction

    TreeNode g1(INT_MAX), g2(INT_MAX), g3(INT_MAX);
    g1.left = &g2;
    g1.right = &g3;
    ok &= test("INT_MAX", &g1, true, true);          // subtree sum 3 * INT_MAX needs long long

    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
