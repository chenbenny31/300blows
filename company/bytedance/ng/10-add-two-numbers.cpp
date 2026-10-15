// ByteDance / LC 2: two non-empty lists store non-negative numbers with digits in
// reverse order (ones digit first). Return their sum as a list in the same order.
// Follow-up LC 445: digits are stored most significant first.
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

// Walk both lists with a carry behind a dummy head. T: O(m+n), S: O(1) extra.
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy{0, nullptr};
    ListNode* tail = &dummy;
    int carry = 0;
    // carry check: a final carry adds one more digit
    while (l1 || l2 || carry) {
        int sum = carry;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }
        carry = sum / 10;
        tail->next = new ListNode{sum % 10, nullptr};
        tail = tail->next;
    }
    return dummy.next;
}

// follow-up: push digits on two stacks, add from the top, prepend each digit.
// T: O(m+n), S: O(m+n).
ListNode* addTwoNumbersII(ListNode* l1, ListNode* l2) {
    vector<int> s1, s2;   // digits, ones digit on top
    for (; l1; l1 = l1->next) { s1.push_back(l1->val); }
    for (; l2; l2 = l2->next) { s2.push_back(l2->val); }
    ListNode* head = nullptr;   // result built from the ones digit up
    int carry = 0;
    while (!s1.empty() || !s2.empty() || carry) {
        int sum = carry;
        if (!s1.empty()) {
            sum += s1.back();
            s1.pop_back();
        }
        if (!s2.empty()) {
            sum += s2.back();
            s2.pop_back();
        }
        carry = sum / 10;
        head = new ListNode{sum % 10, head};
    }
    return head;
}

// nodes live in the vector; link them in order
ListNode* link(vector<ListNode>& nodes) {
    int n = nodes.size();
    for (int i = 0; i + 1 < n; i++) { nodes[i].next = &nodes[i + 1]; }
    return &nodes[0];
}

void test(const string& label, const vector<int>& a, const vector<int>& b,
          const vector<int>& want) {
    vector<ListNode> na, nb;
    for (int d : a) { na.push_back({d, nullptr}); }
    for (int d : b) { nb.push_back({d, nullptr}); }
    ListNode* cur = addTwoNumbers(link(na), link(nb));
    vector<int> got;
    while (cur) {
        got.push_back(cur->val);
        ListNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 4, 3}, {5, 6, 4}, {7, 0, 8});
    test("final carry", {9, 9, 9, 9}, {9, 9}, {8, 9, 0, 0, 1});
    test("zeros", {0}, {0}, {0});
}
