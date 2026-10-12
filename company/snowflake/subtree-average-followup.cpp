#include <cassert>
#include <utility>

using std::pair;

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
};

class SubtreeAverage {
private:
    // T: O(1), S: O(1)
    long long floorDiv(long long sum, int count) {
        long long avg = sum / count;
        if (sum < 0 && sum % count != 0) { avg--; }
        return avg;
    }

    // T: O(n), S: O(h)
    pair<long long, int> dfs(Node* node, long long& avgSum) {
        if (!node) { return {0, 0}; }

        auto [leftSum, leftCount] = dfs(node->left, avgSum);
        auto [rightSum, rightCount] = dfs(node->right, avgSum);

        long long sum = leftSum + rightSum + node->val;
        int count = leftCount + rightCount + 1;
        avgSum += floorDiv(sum, count);
        return {sum, count};
    }

public:
    // T: O(n), S: O(h)
    bool rootEqualsAverageOfAverage(Node* root) {
        if (!root) { return false; }
        long long avgSum = 0;
        auto [sum, count] = dfs(root, avgSum);
        return root->val == floorDiv(avgSum, count);
    }
};

int main() {
    SubtreeAverage solver;

    // Subtree averages: 1, 2, 4.
    // Their floor average is 2, not root value 1.
    Node a{1}, b{0}, c{4};
    a.left = &b;
    b.left = &c;
    assert(!solver.rootEqualsAverageOfAverage(&a));

    // Subtree averages: -1, 0.
    // Tests negative floor division at both stages.
    Node d{-1}, e{0};
    d.right = &e;
    assert(solver.rootEqualsAverageOfAverage(&d));

    assert(solver.rootEqualsAverageOfAverage(&c)); // Single node.
    assert(!solver.rootEqualsAverageOfAverage(nullptr));
}
