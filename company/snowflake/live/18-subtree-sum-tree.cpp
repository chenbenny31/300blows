// A06 · cf. LC 1339 (different output): given an N-ary tree, return a new tree with the same shape where each node's value is the sum
// of the original node's subtree; the original tree must stay unchanged.

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

struct Node {
    int val;
    vector<Node*> children;   // child pointers in order
    Node(int v) : val(v) {}
};

// frees a tree returned by build / build2
void deleteTree(Node* node) {
    if (node == nullptr) { return; }
    for (Node* child : node->children) {
        deleteTree(child);
    }
    delete node;
}

// "val(child,child,...)", leaves print just the value, empty tree prints "null"
string serialize(const Node* node) {
    if (node == nullptr) { return "null"; }
    string s = std::to_string(node->val);
    if (node->children.empty()) { return s; }
    s += "(";
    for (int i = 0; i < (int)node->children.size(); i++) {
        if (i > 0) { s += ","; }
        s += serialize(node->children[i]);
    }
    return s + ")";
}

class SubtreeSumBuilder {
public:
    // postorder recursion: build the children first, then add their sums, T: O(N), S: O(N) new tree + O(H) recursion
    Node* build(const Node* root) {
        if (root == nullptr) { return nullptr; }
        Node* copy = new Node(root->val);
        for (const Node* child : root->children) {
            Node* childCopy = build(child);
            copy->children.push_back(childCopy);
            copy->val += childCopy->val;   // childCopy->val is already its whole subtree sum
        }
        return copy;
    }

    // explicit stack: copy top-down, then sum bottom-up in reverse preorder, T: O(N), S: O(N)
    Node* build2(const Node* root) {
        if (root == nullptr) { return nullptr; }
        Node* newRoot = new Node(root->val);
        vector<pair<const Node*, Node*>> stk = {{root, newRoot}};   // (original, its copy) whose children are not copied yet
        vector<Node*> order;                                        // copies in preorder, parents before children
        while (!stk.empty()) {
            auto [orig, copy] = stk.back();
            stk.pop_back();
            order.push_back(copy);
            for (const Node* child : orig->children) {
                Node* childCopy = new Node(child->val);
                copy->children.push_back(childCopy);
                stk.push_back({child, childCopy});
            }
        }
        for (int i = (int)order.size() - 1; i >= 0; i--) {   // reverse preorder: children are final before their parent
            for (const Node* child : order[i]->children) {
                order[i]->val += child->val;
            }
        }
        return newRoot;
    }
};

bool test(const string& label, const Node* root, const string& expected) {
    SubtreeSumBuilder sb;
    string before = serialize(root);
    Node* got = sb.build(root);
    Node* got2 = sb.build2(root);
    string s1 = serialize(got);
    string s2 = serialize(got2);
    bool unchanged = serialize(root) == before;
    deleteTree(got);
    deleteTree(got2);
    if (s1 == expected && s2 == expected && unchanged) { return true; }
    cout << "FAIL " << label << ": got " << s1 << " / " << s2 << ", expected " << expected
         << (unchanged ? "" : ", original was modified") << "\n";
    return false;
}

int main() {
    bool ok = true;

    // 1 -> {2 -> {4, 5}, 3}
    Node a(1), b(2), c(3), d(4), e(5);
    a.children = {&b, &c};
    b.children = {&d, &e};
    ok &= test("example", &a, "15(11(4,5),3)");    // given shape, original must stay 1(2(4,5),3)

    ok &= test("empty", nullptr, "null");           // empty tree

    Node single(5);
    ok &= test("single", &single, "5");             // smallest non-empty tree

    // 0 -> {-3, 3, 7}
    Node r(0), x(-3), y(3), z(7);
    r.children = {&x, &y, &z};
    ok &= test("wide with negatives", &r, "7(-3,3,7)");   // N-ary, negatives cancel

    // 1 -> 2 -> 3 -> 4
    Node p(1), q(2), s(3), t(4);
    p.children = {&q};
    q.children = {&s};
    s.children = {&t};
    ok &= test("chain", &p, "10(9(7(4)))");          // deepest shape, each level depends on the one below

    // 1 -> {2 -> {3 -> {4}}, 5 -> {6, 7}}
    Node m1(1), m2(2), m3(3), m4(4), m5(5), m6(6), m7(7);
    m1.children = {&m2, &m5};
    m2.children = {&m3};
    m3.children = {&m4};
    m5.children = {&m6, &m7};
    ok &= test("uneven branches", &m1, "28(9(7(4)),18(6,7))");   // child order and per-branch sums kept apart

    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
