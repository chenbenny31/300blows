#include <cassert>
#include <string>

using std::string;

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
};

class TreeNavigator {
private:
    bool dfs(Node* node, int target, string& path) {
        if (!node) { return false; }
        if (node->val == target) { return true; }

        path.push_back('L');
        if (dfs(node->left, target, path)) { return true; }

        path.back() = 'R';
        if (dfs(node->right, target, path)) { return true; }

        path.pop_back();
        return false;
    }

public:
    // T: O(n), S: O(n)
    string getDirections(Node* root, int start, int dest) {
        string toStart;
        string toDest;
        dfs(root, start, toStart);
        dfs(root, dest, toDest);

        int common = 0;
        while (common < static_cast<int>(toStart.size()) &&
               common < static_cast<int>(toDest.size()) &&
               toStart[common] == toDest[common]) {
            common++;
        }
        return string(toStart.size() - common, 'U')
             + toDest.substr(common);
    }
};

int main() {
    //       1
    //      /
    //     2
    //    / \
    //   3   4
    Node a{1}, b{2}, c{3}, d{4};
    a.left = &b;
    b.left = &c;
    b.right = &d;

    TreeNavigator nav;
    assert(nav.getDirections(&a, 3, 4) == "UR"); // Shared prefix removed.
    assert(nav.getDirections(&a, 1, 4) == "LR"); // Only downward moves.
    assert(nav.getDirections(&a, 3, 1) == "UU"); // Only upward moves.
    assert(nav.getDirections(&a, 4, 4).empty()); // Same node.
}
