// build a new tree with the same shape, each new node's value = sum of the original node's subtree
// original tree must stay unchanged (Q45, J10558 NG); follow-up: huge tree, bounded thread pool

// binary or n-ary? node type given?
// sums can overflow int? negative values?
// empty tree return?
// who owns the new tree? (value-type children here, nothing to free)
// recursion depth OK, or very deep chains?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

struct Node {
    long long val; // wide so subtree sums don't overflow
    vector<Node> children;

    bool operator==(const Node& o) const {
        return val == o.val && children == o.children; // whole-tree compare for tests
    }
};

class SubtreeSumBuilder {
public:
    // postorder: build children first, a node's sum = its value + its new children's values
    // T: O(n), S: O(n) new tree + O(h) recursion, input not modified
    Node buildSums_1(const Node& cur) {
        Node copy{cur.val, {}};
        for (const Node& child : cur.children) {
            copy.children.push_back(buildSums_1(child));
            copy.val += copy.children.back().val; // child's new value is already its subtree sum
        }
        return copy;
    }

    // baseline: rescan each node's subtree, T: O(n*h), O(n^2) on a chain, S: O(n) + O(h)
    Node buildSums_2(const Node& cur) {
        Node copy{subtreeSum(cur), {}};
        for (const Node& child : cur.children) {
            copy.children.push_back(buildSums_2(child));
        }
        return copy;
    }

private:
    long long subtreeSum(const Node& cur) {
        long long sum = cur.val;
        for (const Node& child : cur.children) {
            sum += subtreeSum(child);
        }
        return sum;
    }
};

int main() {
    SubtreeSumBuilder sb;

    Node single{5, {}};
    assert(sb.buildSums_1(single) == single); // single node keeps its value
    assert(sb.buildSums_2(single) == single);

    // 1 -> {2 -> {4, 5}, 3}
    Node tree{1, {{2, {{4, {}}, {5, {}}}}, {3, {}}}};
    Node want{15, {{11, {{4, {}}, {5, {}}}}, {3, {}}}};
    Node before = tree;
    assert(sb.buildSums_1(tree) == want);
    assert(sb.buildSums_2(tree) == want);
    assert(tree == before); // original unchanged

    Node chain{1, {{2, {{3, {}}}}}}; // 1 -> 2 -> 3
    Node wantChain{6, {{5, {{3, {}}}}}};
    assert(sb.buildSums_1(chain) == wantChain);
    assert(sb.buildSums_2(chain) == wantChain);

    Node neg{0, {{-3, {}}, {3, {}}}}; // negatives cancel at the root
    assert(sb.buildSums_1(neg) == neg);
    assert(sb.buildSums_2(neg) == neg);

    Node big{2000000000, {{2000000000, {}}}}; // sum passes INT_MAX
    Node wantBig{4000000000LL, {{2000000000, {}}}};
    assert(sb.buildSums_1(big) == wantBig);
    assert(sb.buildSums_2(big) == wantBig);

    std::cout << "all tests passed\n";
    return 0;
}
