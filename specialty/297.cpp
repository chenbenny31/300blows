// LC 297 Serialize and Deserialize Binary Tree: turn a binary tree into a string and
// rebuild the identical tree from that string.
// Trick: preorder with "#" for null fixes the shape; rebuild in that order via a cursor
// Recognize: one traversal alone is ambiguous until null markers are written out
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::to_string;
using std::stoi;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

void serializeHelper(TreeNode* root, string& out) {
    if (!out.empty()) { out += ','; }
    if (root == nullptr) {
        out += '#';
        return;
    }
    out += to_string(root->val);
    serializeHelper(root->left, out);
    serializeHelper(root->right, out);
}

// Preorder, comma-separated, "#" = null. T: O(n), S: O(n).
string serialize(TreeNode* root) {
    string out;
    serializeHelper(root, out);
    return out;
}

// i is shared across calls: each call consumes exactly its own subtree's tokens
TreeNode* build(const vector<string>& tokens, int& i) {
    string tok = tokens[i++];
    if (tok == "#") { return nullptr; }
    TreeNode* node = new TreeNode(stoi(tok));
    node->left = build(tokens, i);
    node->right = build(tokens, i);
    return node;
}

// Split on ',' with a char loop, then rebuild in preorder. T: O(n), S: O(n).
TreeNode* deserialize(const string& data) {
    vector<string> tokens;   // preorder values, "#" for null
    string tok;
    for (char c : data) {
        if (c == ',') {
            tokens.push_back(tok);
            tok = "";
        } else {
            tok += c;
        }
    }
    tokens.push_back(tok);
    int i = 0;
    return build(tokens, i);
}

void freeTree(TreeNode* root) {
    if (root == nullptr) { return; }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void test(const string& label, TreeNode* root) {
    string first = serialize(root);
    TreeNode* copy = deserialize(first);
    cout << label << (serialize(copy) == first ? " PASS\n" : " FAIL\n");
    freeTree(copy);
}

int main() {
    TreeNode n4(4), n5(5), n3(3, &n4, &n5), n2(2), n1(1, &n2, &n3);
    test("lc ex1", &n1);
    test("empty", nullptr);
}
