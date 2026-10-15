// LC 382 Linked List Random Node: getRandom returns a uniformly random node value of a
// singly linked list of unknown length.
// Trick: reservoir sampling - replace the pick with the i-th node with probability 1/i
// Recognize: uniform pick from a stream / list whose length is unknown, O(1) space
// Follow-up LC 398: random index of a target in an array with duplicates.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::rand;

struct ListNode {
    int val;
    ListNode* next;
};

class Solution {
    ListNode* head;   // list to sample from; not owned

public:
    Solution(ListNode* h) : head(h) {}

    // Node i survives: 1/i * i/(i+1) * ... * (n-1)/n = 1/n. T: O(n), S: O(1).
    int getRandom() {
        int pick = 0;
        int i = 0;   // nodes seen so far
        for (ListNode* cur = head; cur; cur = cur->next) {
            i++;
            if (rand() % i == 0) { pick = cur->val; }
        }
        return pick;
    }
};

// follow-up LC 398: reservoir over the indices holding target.
class PickIndex {
    vector<int> nums;   // the array, may contain duplicates

public:
    PickIndex(const vector<int>& n) : nums(n) {}

    // The cnt-th match replaces the pick with probability 1/cnt. T: O(n), S: O(1).
    int pick(int target) {
        int result = -1;
        int cnt = 0;   // matches of target seen so far
        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] != target) { continue; }
            cnt++;
            if (rand() % cnt == 0) { result = i; }
        }
        return result;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ListNode c{3, nullptr}, b{2, &c}, a{1, &b};
    Solution s(&a);
    bool ok = true;
    for (int t = 0; t < 50; t++) {
        int r = s.getRandom();
        if (r < 1 || r > 3) { ok = false; }
    }
    check(ok, "always a member");
    ListNode one{7, nullptr};
    Solution single(&one);
    check(single.getRandom() == 7, "single node");
}
