// I05 (LC 138): deep-copy a linked list whose nodes have next and an arbitrary random pointer;
// no copied node may point into the original, and the original must be unchanged afterwards.

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
using std::unordered_map;
using std::unordered_set;

struct Node {
    int val;
    Node* next;
    Node* random;
    Node(int v) : val(v), next(nullptr), random(nullptr) {}
};

void freeList(Node* head) {
    while (head != nullptr) {
        Node* nxt = head->next;
        delete head;
        head = nxt;
    }
}

// list has exactly these vals / random indices (-1 = null) and shares no node with banned
bool matches(Node* head, const vector<int>& vals, const vector<int>& randomIdx, const unordered_set<Node*>& banned) {
    int n = vals.size();
    vector<Node*> nodes;   // list nodes in next order
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (banned.count(cur) || (int)nodes.size() == n) { return false; }   // shared node, or too long / cyclic
        nodes.push_back(cur);
    }
    if ((int)nodes.size() != n) { return false; }
    unordered_map<Node*, int> index;   // node -> position in this list
    for (int i = 0; i < n; i++) { index[nodes[i]] = i; }
    for (int i = 0; i < n; i++) {
        if (nodes[i]->val != vals[i]) { return false; }
        Node* r = nodes[i]->random;
        if (randomIdx[i] == -1) {
            if (r != nullptr) { return false; }
            continue;
        }
        auto it = index.find(r);   // random must land inside this same list
        if (it == index.end() || it->second != randomIdx[i]) { return false; }
    }
    return true;
}

class ListCopier {
public:
    // original -> copy hash map, two passes, T: O(n), S: O(n)
    Node* copyRandomList(Node* head) {
        unordered_map<Node*, Node*> copyOf;   // original node -> its copy
        for (Node* cur = head; cur != nullptr; cur = cur->next) { copyOf[cur] = new Node(cur->val); }
        for (Node* cur = head; cur != nullptr; cur = cur->next) {
            copyOf[cur]->next = cur->next == nullptr ? nullptr : copyOf[cur->next];
            copyOf[cur]->random = cur->random == nullptr ? nullptr : copyOf[cur->random];
        }
        return head == nullptr ? nullptr : copyOf[head];
    }

    // interleave copies A -> A' -> B -> B', then split and restore, T: O(n), S: O(1) extra
    Node* copyRandomList2(Node* head) {
        if (head == nullptr) { return nullptr; }
        for (Node* cur = head; cur != nullptr; cur = cur->next->next) {
            Node* copy = new Node(cur->val);
            copy->next = cur->next;
            cur->next = copy;
        }
        for (Node* cur = head; cur != nullptr; cur = cur->next->next) {
            if (cur->random != nullptr) { cur->next->random = cur->random->next; }   // random's copy sits right after it
        }
        Node* copyHead = head->next;
        for (Node* cur = head; cur != nullptr; cur = cur->next) {
            Node* copy = cur->next;
            cur->next = copy->next;   // restore the original link first
            copy->next = cur->next == nullptr ? nullptr : cur->next->next;
        }
        return copyHead;
    }
};

bool test(const string& label, Node* head, const vector<int>& vals, const vector<int>& randomIdx) {
    unordered_set<Node*> original;   // nodes of the input list
    for (Node* cur = head; cur != nullptr; cur = cur->next) { original.insert(cur); }
    unordered_set<Node*> none;       // nothing banned when re-checking the original
    ListCopier lc;
    Node* got = lc.copyRandomList(head);
    Node* got2 = lc.copyRandomList2(head);
    bool pass = matches(got, vals, randomIdx, original) && matches(got2, vals, randomIdx, original)
                && matches(head, vals, randomIdx, none);   // original restored and unchanged
    freeList(got);
    freeList(got2);
    if (!pass) { cout << "FAIL " << label << "\n"; }
    return pass;
}

bool testExample1() {
    // [[7,null],[13,0],[11,4],[10,2],[1,0]]
    Node a(7), b(13), c(11), d(10), e(1);
    a.next = &b; b.next = &c; c.next = &d; d.next = &e;
    b.random = &a; c.random = &e; d.random = &c; e.random = &a;
    return test("LC example 1", &a, {7, 13, 11, 10, 1}, {-1, 0, 4, 2, 0});
}

bool testExample2() {
    // [[1,1],[2,1]]
    Node a(1), b(2);
    a.next = &b;
    a.random = &b; b.random = &b;
    return test("LC example 2", &a, {1, 2}, {1, 1});
}

bool testDuplicateValues() {
    // [[3,null],[3,0],[3,null]]
    Node a(3), b(3), c(3);
    a.next = &b; b.next = &c;
    b.random = &a;
    return test("LC example 3 duplicate values", &a, {3, 3, 3}, {-1, 0, -1});
}

bool testSelfRandom() {
    Node a(5);
    a.random = &a;
    return test("single node random to itself", &a, {5}, {0});
}

bool testAllToTail() {
    Node a(1), b(2), c(3);
    a.next = &b; b.next = &c;
    a.random = &c; b.random = &c; c.random = &c;
    return test("every random to the tail", &a, {1, 2, 3}, {2, 2, 2});
}

int main() {
    bool ok = true;
    ok &= testExample1();                             // LC example 1
    ok &= testExample2();                             // LC example 2: random points forward and to self
    ok &= testDuplicateValues();                      // LC example 3: a value-keyed map would break
    ok &= test("empty list", nullptr, {}, {});        // smallest input
    ok &= testSelfRandom();                           // one node, random is itself
    ok &= testAllToTail();                            // many randoms share one target, the tail
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
