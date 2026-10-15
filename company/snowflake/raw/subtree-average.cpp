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
    // T: O(n), S: O(h) recursion
    pair<long long, int> dfs(Node* node, int& matches) {
        if (!node) { return {0, 0}; }

        auto [leftSum, leftCount] = dfs(node->left, matches);
        auto [rightSum, rightCount] = dfs(node->right, matches);

        long long sum = leftSum + rightSum + node->val;
        int count = leftCount + rightCount + 1;

        if (sum / count == node->val) { matches++; }
        return {sum, count};
    }

public:
    // T: O(n), S: O(h)
    int averageOfSubtree(Node* root) {
        int matches = 0;
        dfs(root, matches);
        return matches;
    }
};


int main() {
    // Chain 1 -> 0 -> 4.
    // Subtree averages: 1, 2, 4; nodes 1 and 4 match.
    Node a{1}, b{0}, c{4};
    a.left = &b;
    b.left = &c;

    SubtreeAverage solver;
    assert(solver.averageOfSubtree(&a) == 2);
    assert(solver.averageOfSubtree(&c) == 1); // Leaf; repeated call.
    assert(solver.averageOfSubtree(nullptr) == 0);
}
