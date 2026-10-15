// ByteDance (hack2hire) / LC 662: max width of a binary tree = max over levels of
// (rightmost pos - leftmost pos + 1), counting the null slots between the two ends.
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::queue;
using std::pair;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// BFS level by level with heap positions (children 2p, 2p+1). T: O(n), S: O(n).
int widthOfBinaryTree(TreeNode* root) {
    if (!root) { return 0; }
    queue<pair<TreeNode*, long long>> q;   // (node, position within its level)
    q.push({root, 0});
    int best = 0;
    while (!q.empty()) {
        int sz = q.size();
        long long first = q.front().second;
        long long last = 0;
        for (int i = 0; i < sz; i++) {
            auto [cur, pos] = q.front();
            q.pop();
            // rebase so the level's leftmost is 0: positions never double past the width
            pos -= first;
            last = pos;
            if (cur->left) { q.push({cur->left, 2 * pos}); }
            if (cur->right) { q.push({cur->right, 2 * pos + 1}); }
        }
        best = max(best, (int)last + 1);
    }
    return best;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (widthOfBinaryTree(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a5{5, nullptr, nullptr}, a3r{3, nullptr, nullptr}, a9{9, nullptr, nullptr};
    TreeNode a3{3, &a5, &a3r}, a2{2, nullptr, &a9}, a1{1, &a3, &a2};
    test("lc ex1", &a1, 4);

    TreeNode b6{6, nullptr, nullptr}, b7{7, nullptr, nullptr};
    TreeNode b5{5, &b6, nullptr}, b9{9, &b7, nullptr};
    TreeNode b3{3, &b5, nullptr}, b2{2, nullptr, &b9}, b1{1, &b3, &b2};
    test("null gap", &b1, 7);
}
