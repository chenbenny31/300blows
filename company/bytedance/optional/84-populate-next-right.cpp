// ByteDance (hack2hire) / LC 116: in a perfect binary tree, point each node's next to
// its right neighbor on the same level (nullptr at the level's end), O(1) extra space.
// Follow-up LC 117: the tree is any binary tree, not necessarily perfect.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct Node {
    int val;
    Node* left;
    Node* right;
    Node* next;
};

// Wire level d+1 by walking level d through its next chain. T: O(n), S: O(1).
Node* connect(Node* root) {
    Node* leftmost = root;   // first node of the level already wired
    while (leftmost && leftmost->left) {
        for (Node* cur = leftmost; cur; cur = cur->next) {
            cur->left->next = cur->right;
            // the right child's neighbor is the left child of cur's neighbor
            if (cur->next) { cur->right->next = cur->next->left; }
        }
        leftmost = leftmost->left;
    }
    return root;
}

// follow-up: append each child of level d to a dummy-headed list for level d+1.
// T: O(n), S: O(1).
Node* connectAny(Node* root) {
    Node* level = root;   // first node of the level already wired
    while (level) {
        Node dummy{0, nullptr, nullptr, nullptr};   // dummy.next = first node below
        Node* tail = &dummy;                        // last node wired below
        for (Node* cur = level; cur; cur = cur->next) {
            if (cur->left) { tail = tail->next = cur->left; }
            if (cur->right) { tail = tail->next = cur->right; }
        }
        level = dummy.next;
    }
    return root;
}

// values along the next chain starting at node
vector<int> chain(Node* node) {
    vector<int> out;
    for (; node; node = node->next) { out.push_back(node->val); }
    return out;
}

void test(const string& label, Node* start, const vector<int>& want) {
    cout << label << (chain(start) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //        1
    //    2       3
    //  4   5   6   7
    Node n4{4, nullptr, nullptr, nullptr}, n5{5, nullptr, nullptr, nullptr};
    Node n6{6, nullptr, nullptr, nullptr}, n7{7, nullptr, nullptr, nullptr};
    Node n2{2, &n4, &n5, nullptr}, n3{3, &n6, &n7, nullptr};
    Node n1{1, &n2, &n3, nullptr};
    connect(&n1);
    test("level 0", &n1, {1});
    test("level 1", &n2, {2, 3});
    test("level 2", &n4, {4, 5, 6, 7});
}
