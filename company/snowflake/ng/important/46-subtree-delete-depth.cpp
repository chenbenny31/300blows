// J12962 (cf. LC 559): n-ary tree, depth counts nodes (root alone = 1). Deleting a
// non-root node removes its WHOLE subtree. Return the fewest deletions so the depth
// is <= k, or -1 if impossible (k < 1).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

struct Node {
    int val;
    vector<Node> children;
};

// LC 559 DFS. T: O(n), S: O(h).
int maxDepth(const Node& node) {
    int depth = 1;
    for (const Node& child : node.children) { depth = max(depth, 1 + maxDepth(child)); }
    return depth;
}

// Count root children whose subtree is too deep. T: O(n), S: O(h).
int minDeletions(const Node& root, int k) {
    // the root can't be deleted and alone already has depth 1
    if (k < 1) { return -1; }
    int deletions = 0;
    for (const Node& child : root.children) {
        // too deep: needs >= 1 deletion inside; deleting the child itself clears it all
        if (1 + maxDepth(child) > k) { deletions++; }
    }
    return deletions;
}

void test(const string& label, const Node& root, int k, int want) {
    cout << label << (minDeletions(root, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    Node root{1, {{2, {{4, {}}}}, {3, {}}}};
    test("one too deep", root, 2, 1);
    test("already ok", root, 3, 0);
    test("only root", root, 1, 2);
    test("impossible", root, 0, -1);
}
