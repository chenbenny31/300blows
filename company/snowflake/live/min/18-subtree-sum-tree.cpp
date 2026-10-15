// A06 · cf. LC 1339: N-ary tree -> new tree of the same shape, each value = original subtree sum; original unchanged. Follow-up: iterative.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

struct Node {
    int val;
    vector<Node*> children;   // child pointers in order
    Node(int v) : val(v) {}
};

// postorder recursion: build children first, then add their sums, T: O(N), S: O(N) new tree + O(H) stack
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

// follow-up (very deep tree): explicit stack copy top-down, then sum in reverse preorder, T: O(N), S: O(N)
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
    for (int i = (int)order.size() - 1; i >= 0; i--) {   // reverse preorder: children final before parent
        for (const Node* child : order[i]->children) { order[i]->val += child->val; }
    }
    return newRoot;
}

void freeTree(Node* node) {
    if (node == nullptr) { return; }
    for (Node* child : node->children) { freeTree(child); }
    delete node;
}

// "val(child,...)", leaf = "val", empty = "null"
string show(const Node* node) {
    if (node == nullptr) { return "null"; }
    string s = std::to_string(node->val);
    if (node->children.empty()) { return s; }
    for (int i = 0; i < (int)node->children.size(); i++) { s += (i == 0 ? "(" : ",") + show(node->children[i]); }
    return s + ")";
}

void test(Node* (*fn)(const Node*), const Node* root, const string& expect) {
    string before = show(root);
    Node* copy = fn(root);
    string got = show(copy);
    freeTree(copy);
    cout << (got == expect && show(root) == before ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    Node a(1), b(2), c(3), d(4), e(5);   // 1(2(4,5),3)
    a.children = {&b, &c};
    b.children = {&d, &e};
    Node p(1), q(2), s(3);               // chain 1 -> 2 -> 3
    p.children = {&q};
    q.children = {&s};
    Node r(0), x(-3), y(3);              // 0(-3,3)
    r.children = {&x, &y};
    for (auto fn : {build, build2}) {
        test(fn, &a, "15(11(4,5),3)");   // example, original must stay unchanged
        test(fn, nullptr, "null");       // empty tree
        test(fn, &p, "6(5(3))");         // chain: each level uses the one below
        test(fn, &r, "0(-3,3)");         // negatives cancel
    }
}
