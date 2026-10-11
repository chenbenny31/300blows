// n-ary tree: maxDepth (LC559), then the fewest deletions so depth <= k (J12962)
// deleting a node removes its WHOLE subtree (not promotion); the root can't be deleted; depth counts nodes

// deletion removes the subtree, or promotes the children? (promotion = Q02, a different problem)
// minimize the number of deletions chosen, or the nodes removed?
// depth in nodes or edges? root alone = 1?
// k < 1: impossible, return -1?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

struct Node {
    int val;
    vector<Node> children;
};

class DepthLimiter {
public:
    // T: O(n), S: O(h)
    int maxDepth(const Node& node) {
        int best = 0; // tallest child
        for (const Node& c : node.children) {
            best = std::max(best, maxDepth(c));
        }
        return best + 1;
    }

    // each root child whose subtree goes deeper than k needs at least one deletion inside it,
    // and deleting that child itself is one deletion that clears all of it -> count those children
    // T: O(n), S: O(h)
    int minDeletions(const Node& root, int k) {
        if (k < 1) {
            return -1; // root alone already has depth 1 and can't be deleted
        }
        int deletions = 0;
        for (const Node& c : root.children) {
            if (1 + maxDepth(c) > k) {
                deletions++;
            }
        }
        return deletions;
    }
};

int main() {
    DepthLimiter dl;
    Node tree{1, {{2, {{4, {{5, {}}}}}}, {3, {}}}}; // 1 -> {2 -> 4 -> 5, 3}

    assert(dl.maxDepth(tree) == 4); // LC559 part
    assert(dl.minDeletions(tree, 2) == 1); // target: only child 2's subtree is too deep
    assert(dl.minDeletions(tree, 4) == 0); // edge: already within k
    assert(dl.minDeletions(tree, 1) == 2); // edge: k = 1 keeps only the root
    assert(dl.minDeletions(tree, 0) == -1); // fault: impossible bound
    assert(dl.minDeletions(Node{7, {}}, 1) == 0); // null: single node

    std::cout << "all tests passed\n";
    return 0;
}
