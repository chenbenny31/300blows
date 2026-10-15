// ByteDance / LC 148: sort a singly linked list in ascending order and return the head.
// Follow-up: O(n log n) time with O(1) extra space.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;
};

// Splice two sorted lists behind a dummy. T: O(a+b), S: O(1).
ListNode* merge(ListNode* a, ListNode* b) {
    ListNode dummy{0, nullptr};
    ListNode* tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

// Top-down merge sort; slow/fast finds the middle. T: O(n log n), S: O(log n) stack.
ListNode* sortList(ListNode* head) {
    if (!head || !head->next) { return head; }
    // fast starts one ahead so slow stops at the end of the left half
    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* right = slow->next;
    slow->next = nullptr;
    return merge(sortList(head), sortList(right));
}

// cut off the first k nodes of head; return the head of the rest.
ListNode* split(ListNode* head, int k) {
    for (int i = 1; head && i < k; i++) { head = head->next; }
    if (!head) { return nullptr; }
    ListNode* rest = head->next;
    head->next = nullptr;
    return rest;
}

// follow-up: bottom-up merge sort, runs of size 1, 2, 4, ... T: O(n log n), S: O(1).
ListNode* sortListBottomUp(ListNode* head) {
    int n = 0;
    for (ListNode* cur = head; cur; cur = cur->next) { n++; }
    ListNode dummy{0, head};
    for (int size = 1; size < n; size *= 2) {
        ListNode* tail = &dummy;   // last node of the merged part of this pass
        ListNode* cur = dummy.next;
        while (cur) {
            ListNode* left = cur;
            ListNode* right = split(left, size);
            cur = split(right, size);
            tail->next = merge(left, right);
            while (tail->next) { tail = tail->next; }
        }
    }
    return dummy.next;
}

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    vector<ListNode> nodes;   // the list; link only after all pushes, so addresses hold
    for (int v : vals) { nodes.push_back({v, nullptr}); }
    int n = nodes.size();
    for (int i = 0; i + 1 < n; i++) { nodes[i].next = &nodes[i + 1]; }
    vector<int> got;
    for (ListNode* cur = sortList(n ? &nodes[0] : nullptr); cur; cur = cur->next) {
        got.push_back(cur->val);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {4, 2, 1, 3}, {1, 2, 3, 4});
    test("negatives", {-1, 5, 3, 4, 0}, {-1, 0, 3, 4, 5});
    test("empty", {}, {});
}
