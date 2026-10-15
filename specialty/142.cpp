// LC 142 Linked List Cycle II: return the node where the list's cycle begins, or
// nullptr if the list has no cycle.
// Trick: Floyd; after meeting, step one from head and one from the meet: they hit entry
// Recognize: find a cycle's start in O(1) space (also LC 287).
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

// Floyd: a = head->entry, b = entry->meet, c = cycle length; fast walked twice as far,
// so a + b = k*c, i.e. a = k*c - b: a steps from the meet land on the entry.
// T: O(n), S: O(1).
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            ListNode* p = head;
            while (p != slow) {
                p = p->next;
                slow = slow->next;
            }
            return p;
        }
    }
    return nullptr;
}

void test(const string& label, const vector<int>& vals, int pos) {
    int n = vals.size();
    vector<ListNode> nodes(n);
    for (int i = 0; i < n; i++) { nodes[i] = {vals[i], nullptr}; }
    for (int i = 0; i + 1 < n; i++) { nodes[i].next = &nodes[i + 1]; }
    if (pos >= 0) { nodes[n - 1].next = &nodes[pos]; }
    ListNode* want = pos >= 0 ? &nodes[pos] : nullptr;
    cout << label << (detectCycle(&nodes[0]) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {3, 2, 0, -4}, 1);
    test("cycle at head", {1, 2}, 0);
    test("no cycle", {1}, -1);
}
