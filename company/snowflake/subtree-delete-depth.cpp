#include <algorithm>
#include <cassert>
#include <vector>

using std::vector;

struct Node {
    int val;
    vector<Node> children;
};

// T: O(n), S: O(h)
int maxDepth(const Node& node) {
    int depth = 1;
    for (const Node& child : node.children) {
        depth = std::max(depth, 1 + maxDepth(child));
    }
    return depth;
}

// T: O(n), S: O(h)
int minDeletions(const Node& root, int k) {
    if (k < 1) { return -1; }

    int dels = 0;
    for (const Node& child : root.children) {
        if (1 + maxDepth(child) > k) {
            dels++;
        }
    }
    return dels;
}

int main() {
    // Root 1 has children 2 and 3; node 2 has child 4.
    Node root{1, {{2, {{4, {}}}}, {3, {}}}};

    assert(maxDepth(root) == 3);
    assert(minDeletions(root, 2) == 1); // Delete subtree rooted at 2.
    assert(minDeletions(root, 3) == 0); // Already shallow enough.
    assert(minDeletions(root, 1) == 2); // Remove both root-child subtrees.
    assert(minDeletions(root, 0) == -1); // Cannot remove root.
    assert(minDeletions(Node{1, {}}, 1) == 0); // Single node.
}
