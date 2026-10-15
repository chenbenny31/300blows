// ByteDance / LC 1171: repeatedly delete consecutive runs of nodes that sum to 0 from a
// singly linked list until none remain; return the head of the final list.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

struct ListNode {
    int val;
    ListNode* next;
};

// Prefix sums: a repeated sum means the nodes between sum to 0. T: O(n), S: O(n).
ListNode* removeZeroSumSublists(ListNode* head) {
    ListNode dummy{0, head};
    unordered_map<int, ListNode*> last;   // last[sum] = last node with that prefix sum
    int prefix = 0;
    for (ListNode* cur = &dummy; cur; cur = cur->next) {
        prefix += cur->val;
        last[prefix] = cur;
    }
    prefix = 0;
    for (ListNode* cur = &dummy; cur; cur = cur->next) {
        prefix += cur->val;
        // jump to the last node with the same sum, skipping the zero-sum run
        cur->next = last[prefix]->next;
    }
    return dummy.next;
}

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    vector<ListNode> nodes;   // the list; link only after all pushes, so addresses hold
    for (int v : vals) { nodes.push_back({v, nullptr}); }
    int n = nodes.size();
    for (int i = 0; i + 1 < n; i++) { nodes[i].next = &nodes[i + 1]; }
    vector<int> got;
    for (ListNode* cur = removeZeroSumSublists(&nodes[0]); cur; cur = cur->next) {
        got.push_back(cur->val);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, -3, 3, 1}, {3, 1});
    test("lc ex2", {1, 2, 3, -3, 4}, {1, 2, 4});
    test("lc ex3", {1, 2, 3, -3, -2}, {1});
}
