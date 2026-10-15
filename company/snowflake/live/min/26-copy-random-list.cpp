// I05 · LC 138: deep-copy a list with next + random pointers; no copy points into the original, original unchanged. Follow-up: O(1) extra space.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;

struct Node {
    int val;
    Node* next = nullptr;
    Node* random = nullptr;
    Node(int v) : val(v) {}
};

// original -> copy hash map, two passes, T: O(n), S: O(n)
Node* copyRandomList(Node* head) {
    unordered_map<Node*, Node*> copyOf;   // original node -> its copy (keyed by pointer, values may repeat)
    copyOf[nullptr] = nullptr;
    for (Node* cur = head; cur != nullptr; cur = cur->next) { copyOf[cur] = new Node(cur->val); }
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        copyOf[cur]->next = copyOf[cur->next];
        copyOf[cur]->random = copyOf[cur->random];
    }
    return copyOf[head];
}

// follow-up: interleave A -> A' -> B -> B', set randoms, then split and restore, T: O(n), S: O(1) extra
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

// (val, random index) per node; -1 = null, -2 = points outside this list
vector<pair<int, int>> dump(Node* head) {
    unordered_map<Node*, int> idx;   // node -> position
    int i = 0;
    for (Node* c = head; c != nullptr; c = c->next) { idx[c] = i++; }
    vector<pair<int, int>> out;
    for (Node* c = head; c != nullptr; c = c->next) {
        out.push_back({c->val, c->random == nullptr ? -1 : (idx.count(c->random) ? idx[c->random] : -2)});
    }
    return out;
}

void freeList(Node* head) {
    while (head != nullptr) {
        Node* nxt = head->next;
        delete head;
        head = nxt;
    }
}

void test(const string& label, Node* (*fn)(Node*), Node* head, const vector<pair<int, int>>& expect) {
    Node* got = fn(head);
    bool ok = dump(got) == expect && dump(head) == expect && (head == nullptr || got != head);   // copy right, original intact, new nodes
    freeList(got);
    cout << (ok ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    Node a(7), b(13), c(11), d(10), e(1);   // LC example 1: [[7,null],[13,0],[11,4],[10,2],[1,0]]
    a.next = &b; b.next = &c; c.next = &d; d.next = &e;
    b.random = &a; c.random = &e; d.random = &c; e.random = &a;
    Node x(3), y(3), z(3);                  // LC example 3: [[3,null],[3,0],[3,null]]
    x.next = &y; y.next = &z;
    y.random = &x;
    Node s(5);                              // random to itself
    s.random = &s;
    for (auto fn : {copyRandomList, copyRandomList2}) {
        test("example 1", fn, &a, {{7, -1}, {13, 0}, {11, 4}, {10, 2}, {1, 0}});   // forward and backward randoms
        test("duplicate values", fn, &x, {{3, -1}, {3, 0}, {3, -1}});            // a value-keyed map would break
        test("self random", fn, &s, {{5, 0}});                                   // one node
        test("empty", fn, nullptr, {});                                          // smallest input
    }
}
