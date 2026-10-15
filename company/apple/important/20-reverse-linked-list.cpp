// LC 206: reverse a singly linked list and return the new head.
// Follow-up: do it recursively.
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

// Iterative: flip each next pointer while walking. T: O(n), S: O(1).
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;   // head of the already reversed part
    ListNode* cur = head;
    while (cur) {
        ListNode* nxt = cur->next;   // save before overwriting cur->next
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    return prev;
}

// follow-up: reverse the rest, then hang head after its old next. T: O(n), S: O(n) stack.
ListNode* reverseListRec(ListNode* head) {
    if (!head || !head->next) { return head; }
    ListNode* newHead = reverseListRec(head->next);
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}

void test(const string& label, ListNode* head, const vector<int>& want) {
    vector<int> got;
    for (ListNode* cur = reverseList(head); cur; cur = cur->next) {
        got.push_back(cur->val);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    ListNode n3{3, nullptr}, n2{2, &n3}, n1{1, &n2};
    test("three nodes", &n1, {3, 2, 1});
    ListNode single{1, nullptr};
    test("one node", &single, {1});
    test("empty", nullptr, {});
}
