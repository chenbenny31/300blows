// LC 143: reorder L0 -> L1 -> ... -> Ln into L0 -> Ln -> L1 -> Ln-1 -> ... in place.
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

// Find the middle, reverse the second half, merge alternately. T: O(n), S: O(1).
void reorderList(ListNode* head) {
    if (!head) { return; }
    ListNode* slow = head;
    ListNode* fast = head;
    // slow stops at the end of the first half (first half keeps the extra node)
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* prev = nullptr;   // head of the reversed second half
    ListNode* cur = slow->next;
    slow->next = nullptr;       // cut the two halves apart
    while (cur) {
        ListNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    ListNode* first = head;
    ListNode* second = prev;
    // second half is never longer than the first
    while (second) {
        ListNode* n1 = first->next;
        ListNode* n2 = second->next;
        first->next = second;
        second->next = n1;
        first = n1;
        second = n2;
    }
}

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    vector<ListNode> nodes(vals.size());
    for (int i = 0; i < (int)vals.size(); i++) {
        nodes[i] = {vals[i], i + 1 < (int)vals.size() ? &nodes[i + 1] : nullptr};
    }
    ListNode* head = vals.empty() ? nullptr : &nodes[0];
    reorderList(head);
    vector<int> got;
    for (ListNode* cur = head; cur; cur = cur->next) { got.push_back(cur->val); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("even", {1, 2, 3, 4}, {1, 4, 2, 3});
    test("odd", {1, 2, 3, 4, 5}, {1, 5, 2, 4, 3});
    test("one node", {1}, {1});
}
