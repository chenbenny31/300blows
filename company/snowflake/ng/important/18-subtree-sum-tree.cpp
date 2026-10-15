// A06 (cf. LC 1339): N-ary tree. Return a NEW tree of the same shape where each value is
// the sum of the original node's subtree; the original stays unchanged.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct Node {
    int val;
    vector<Node> children;
};

// Postorder: build each child's new subtree first; its root value is already that
// child's subtree sum. T: O(N), S: O(N) new tree + O(H) recursion.
Node subtreeSum(const Node& root) {
    Node copy{root.val, {}};
    for (const Node& child : root.children) {
        copy.children.push_back(subtreeSum(child));
        copy.val += copy.children.back().val;
    }
    return copy;
}

// same values and same shape, child order included
bool sameTree(const Node& a, const Node& b) {
    if (a.val != b.val || a.children.size() != b.children.size()) { return false; }
    for (int i = 0; i < (int)a.children.size(); i++) {
        if (!sameTree(a.children[i], b.children[i])) { return false; }
    }
    return true;
}

void test(const string& label, const Node& root, const Node& want) {
    cout << label << (sameTree(subtreeSum(root), want) ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1 -> {2 -> {4, 5}, 3}
    Node root{1, {{2, {{4, {}}, {5, {}}}}, {3, {}}}};
    test("two levels", root, Node{15, {{11, {{4, {}}, {5, {}}}}, {3, {}}}});
    // 1 -> 2 -> 3
    test("chain", Node{1, {{2, {{3, {}}}}}}, Node{6, {{5, {{3, {}}}}}});
    test("single", Node{5, {}}, Node{5, {}});
}
