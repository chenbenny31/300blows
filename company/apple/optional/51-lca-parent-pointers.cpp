// LC 1650: nodes have parent pointers (root's parent is null). Given p and q in the same
// tree, return their lowest common ancestor.
#include <iostream>
#include <string>
using std::cout;
using std::string;

struct Node {
    int val;
    Node* left;
    Node* right;
    Node* parent;
};

// Like list intersection: each pointer walks up, then restarts at the other node; both
// travel depth(p) + depth(q) and meet at the LCA. T: O(h), S: O(1).
Node* lowestCommonAncestor(Node* p, Node* q) {
    Node* a = p;
    Node* b = q;
    while (a != b) {
        a = a->parent ? a->parent : q;
        b = b->parent ? b->parent : p;
    }
    return a;
}

void test(const string& label, Node* p, Node* q, Node* want) {
    cout << label << (lowestCommonAncestor(p, q) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //      3
    //    5   1
    //   6 2
    Node n3{3, nullptr, nullptr, nullptr};
    Node n5{5, nullptr, nullptr, &n3};
    Node n1{1, nullptr, nullptr, &n3};
    Node n6{6, nullptr, nullptr, &n5};
    Node n2{2, nullptr, nullptr, &n5};
    n3.left = &n5;
    n3.right = &n1;
    n5.left = &n6;
    n5.right = &n2;
    test("diff depth", &n6, &n1, &n3);
    test("siblings", &n6, &n2, &n5);
    test("ancestor", &n5, &n2, &n5);
}
