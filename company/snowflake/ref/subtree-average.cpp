// LC2265: count nodes whose value equals the floor average of their own subtree
// Q46 (J9933): take every subtree's floor average, then the floor average of those n averages,
// and check whether it equals root's value (not the same as root == whole-tree average)

// both averages floored? (C++ / truncates toward zero, differs on negatives)
// values can be negative?
// empty tree: 0 / false?
// sums beyond int?

#include <utility>
#include <cassert>
#include <iostream>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class SubtreeAverage {
private:
    int matches = 0; // LC2265 answer
    long long avgSum = 0; // Q46: sum of every subtree's floor average
    long long nodes = 0; // Q46: n

public:
    // postorder returns (sum, count) of each subtree, T: O(n), S: O(h)
    int averageOfSubtree(TreeNode* root) {
        matches = 0;
        dfs(root);
        return matches;
    }

    // Q46, same postorder, T: O(n), S: O(h)
    bool rootEqualsAverageOfAverages(TreeNode* root) {
        if (root == nullptr) {
            return false;
        }
        avgSum = 0;
        nodes = 0;
        dfs(root);
        return root->val == floorDiv(avgSum, nodes);
    }

private:
    std::pair<long long, long long> dfs(TreeNode* node) {
        if (node == nullptr) {
            return {0, 0};
        }
        std::pair<long long, long long> l = dfs(node->left);
        std::pair<long long, long long> r = dfs(node->right);
        long long sum = l.first + r.first + node->val;
        long long count = l.second + r.second + 1;
        long long avg = floorDiv(sum, count);
        if (avg == node->val) {
            matches++;
        }
        avgSum += avg;
        nodes++;
        return {sum, count};
    }

    // floor division; plain / rounds toward zero, which is wrong for negative sums
    long long floorDiv(long long a, long long b) {
        long long q = a / b;
        if (a % b != 0 && (a < 0) != (b < 0)) {
            q--;
        }
        return q;
    }
};

int main() {
    SubtreeAverage sa;

    TreeNode a1(1), a0(0), a4(4); // chain 1 -> 0 -> 4
    a1.left = &a0;
    a0.left = &a4;
    assert(sa.averageOfSubtree(&a1) == 2); // target: 4 == 4 and 1 == 5/3, 0 != 2
    assert(!sa.rootEqualsAverageOfAverages(&a1)); // Q46 target: averages 1, 2, 4 -> 2, root is 1

    TreeNode n1(-1), n2(0); // -1 -> 0
    n1.left = &n2;
    assert(sa.averageOfSubtree(&n1) == 2); // edge: floor(-1/2) = -1, truncation would give 0
    assert(sa.rootEqualsAverageOfAverages(&n1)); // averages -1, 0 -> floor(-1/2) = -1 == root

    assert(sa.averageOfSubtree(nullptr) == 0); // null
    assert(!sa.rootEqualsAverageOfAverages(nullptr));

    std::cout << "all tests passed\n";
    return 0;
}
