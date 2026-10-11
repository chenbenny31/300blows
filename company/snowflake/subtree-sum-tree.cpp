#include <cassert>
#include <vector>

using std::vector;

struct Node {
    long long val;
    vector<Node> children;

    // test helper for subtree comp
    bool operator==(const Node& other) const {
        return val == other.val && children == other.children;
    }
};

// T: O(n), S: O(n) + O(h) recursion
Node buildSums(const Node& cur) {
    Node node{cur.val, {}};
    for (const Node& child : cur.children) {
        node.children.push_back(buildSums(child));
        node.val += node.children.back().val;
    }
    return node;
}

int main() {
    Node tree{1, {{2, {}}, {3, {}}}}; // 1-2, 1-3
    Node want{6, {{2, {}}, {3, {}}}};

    Node orig = tree;
    assert(buildSums(tree) == want);
    assert(orig == tree); // orig unchanged

    // single node tree
    // chain tree
    // negative val node
    // large val, overflow int
}
