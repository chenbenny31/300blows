// LC 160 Intersection of Two Linked Lists: return the node where lists A and B merge,
// or nullptr if they never meet.
// Trick: two pointers jump to the other list's head at null; both walk a+c+b and meet
// Recognize: align two paths of different lengths without measuring them.
#include <iostream>
#include <string>
using std::cout;
using std::string;

struct ListNode {
    int val;
    ListNode* next;
};

// Each pointer walks its own list then the other: a + c + b steps for both, so they
// reach the merge node together, or nullptr together when c = 0. T: O(a+b), S: O(1).
ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
    ListNode* p = headA;
    ListNode* q = headB;
    while (p != q) {
        p = p ? p->next : headB;
        q = q ? q->next : headA;
    }
    return p;
}

void test(const string& label, ListNode* headA, ListNode* headB, ListNode* want) {
    cout << label << (getIntersectionNode(headA, headB) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    ListNode n5{5, nullptr};
    ListNode n8{8, &n5};
    ListNode n1{1, &n8};
    ListNode n4{4, &n1};
    ListNode n6{6, &n8};
    ListNode x2{2, nullptr};
    ListNode x1{1, &x2};
    test("intersect", &n4, &n6, &n8);
    test("disjoint", &n4, &x1, nullptr);
    test("same head", &n4, &n4, &n4);
}
