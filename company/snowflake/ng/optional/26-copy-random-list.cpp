// I05 · LC 138: deep-copy a list whose nodes have next and a random pointer; no copied
// pointer may point into the original. Follow-up: O(1) extra space, original restored.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

struct Node {
    int val;
    Node* next = nullptr;
    Node* random = nullptr;
    Node(int v) : val(v) {}
};

// Two passes with an original -> copy map. T: O(n), S: O(n).
Node* copyRandomList(Node* head) {
    unordered_map<Node*, Node*> copyOf;   // original node -> its copy; nullptr -> nullptr
    copyOf[nullptr] = nullptr;
    for (Node* cur = head; cur; cur = cur->next) { copyOf[cur] = new Node(cur->val); }
    for (Node* cur = head; cur; cur = cur->next) {
        copyOf[cur]->next = copyOf[cur->next];
        copyOf[cur]->random = copyOf[cur->random];
    }
    return copyOf[head];
}

// follow-up: interleave A -> A' -> B -> B', so X.random's copy is X.random->next;
// then split, restoring the original. T: O(n), S: O(1) extra.
Node* copyRandomListInPlace(Node* head) {
    if (!head) { return nullptr; }
    for (Node* cur = head; cur; cur = cur->next->next) {
        Node* copy = new Node(cur->val);
        copy->next = cur->next;
        cur->next = copy;
    }
    for (Node* cur = head; cur; cur = cur->next->next) {
        if (cur->random) { cur->next->random = cur->random->next; }
    }
    Node* copyHead = head->next;
    for (Node* cur = head; cur; cur = cur->next) {
        Node* copy = cur->next;
        cur->next = copy->next;   // restore the original link first
        copy->next = cur->next ? cur->next->next : nullptr;
    }
    return copyHead;
}

// list -> LC format {{val, random index or -1}, ...}
vector<vector<int>> serialize(Node* head) {
    unordered_map<Node*, int> index;   // node -> position in this list
    int i = 0;
    for (Node* cur = head; cur; cur = cur->next) { index[cur] = i++; }
    vector<vector<int>> out;
    for (Node* cur = head; cur; cur = cur->next) {
        out.push_back({cur->val, cur->random ? index[cur->random] : -1});
    }
    return out;
}

void freeList(Node* head) {
    while (head) {
        Node* nxt = head->next;
        delete head;
        head = nxt;
    }
}

void test(const string& label, Node* head, const vector<vector<int>>& want) {
    unordered_set<Node*> original;   // nodes of the input list
    for (Node* cur = head; cur; cur = cur->next) { original.insert(cur); }
    Node* copy = copyRandomList(head);
    bool fresh = true;               // no copy node or copy random is an original node
    for (Node* cur = copy; cur; cur = cur->next) {
        if (original.count(cur) || original.count(cur->random)) { fresh = false; }
    }
    bool ok = fresh && serialize(copy) == want;
    freeList(copy);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Node a1(1), a2(2);
    a1.next = &a2;
    a1.random = &a2; a2.random = &a2;
    test("forward and self", &a1, {{1, 1}, {2, 1}});

    Node b1(3), b2(3), b3(3);
    b1.next = &b2; b2.next = &b3;
    b2.random = &b1;
    test("duplicate values", &b1, {{3, -1}, {3, 0}, {3, -1}});

    test("empty", nullptr, {});
}
