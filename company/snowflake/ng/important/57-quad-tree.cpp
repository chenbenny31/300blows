// LC tag · LC 427: n x n grid of 0/1, n a power of 2; build its quad tree. A region of
// equal cells is one leaf with that value; otherwise split into 4 quadrants.
// Follow-up: avoid rescanning each region, O(n^2) total.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct Node {
    bool val;
    bool isLeaf;
    Node* topLeft = nullptr;
    Node* topRight = nullptr;
    Node* bottomLeft = nullptr;
    Node* bottomRight = nullptr;
};

// leaf if the size x size region at (r, c) is uniform, else recurse on 4 quadrants
Node* build(const vector<vector<int>>& grid, int r, int c, int size) {
    bool uniform = true;
    for (int i = r; i < r + size && uniform; i++) {
        for (int j = c; j < c + size; j++) {
            if (grid[i][j] != grid[r][c]) {
                uniform = false;
                break;
            }
        }
    }
    if (uniform) { return new Node{grid[r][c] == 1, true}; }
    int half = size / 2;
    Node* node = new Node{false, false};
    node->topLeft = build(grid, r, c, half);
    node->topRight = build(grid, r, c + half, half);
    node->bottomLeft = build(grid, r + half, c, half);
    node->bottomRight = build(grid, r + half, c + half, half);
    return node;
}

// Scan each region, split if mixed. T: O(n^2 log n), each level rescans every cell.
// S: O(n^2) nodes.
Node* construct(const vector<vector<int>>& grid) {
    return build(grid, 0, 0, grid.size());
}

// pre[i][j] = sum of grid[0..i-1][0..j-1]; region sum 0 or size^2 means uniform
Node* buildFast(const vector<vector<int>>& pre, int r, int c, int size) {
    int sum = pre[r + size][c + size] - pre[r][c + size] - pre[r + size][c] + pre[r][c];
    if (sum == 0 || sum == size * size) { return new Node{sum > 0, true}; }
    int half = size / 2;
    Node* node = new Node{false, false};
    node->topLeft = buildFast(pre, r, c, half);
    node->topRight = buildFast(pre, r, c + half, half);
    node->bottomLeft = buildFast(pre, r + half, c, half);
    node->bottomRight = buildFast(pre, r + half, c + half, half);
    return node;
}

// follow-up: 2D prefix sums make the uniform check O(1). T: O(n^2), S: O(n^2).
Node* constructFast(const vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> pre(n + 1, vector<int>(n + 1, 0));   // pre[i][j] as in buildFast
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pre[i + 1][j + 1] = pre[i][j + 1] + pre[i + 1][j] - pre[i][j] + grid[i][j];
        }
    }
    return buildFast(pre, 0, 0, n);
}

void destroy(Node* node) {
    if (!node) { return; }
    destroy(node->topLeft);
    destroy(node->topRight);
    destroy(node->bottomLeft);
    destroy(node->bottomRight);
    delete node;
}

// preorder: "0"/"1" = leaf value, "*" = internal node followed by its 4 children
string serialize(Node* node) {
    if (node->isLeaf) { return node->val ? "1" : "0"; }
    return "*" + serialize(node->topLeft) + serialize(node->topRight) +
           serialize(node->bottomLeft) + serialize(node->bottomRight);
}

void test(const string& label, const vector<vector<int>>& grid, const string& want) {
    Node* root = construct(grid);
    cout << label << (serialize(root) == want ? " PASS\n" : " FAIL\n");
    destroy(root);
}

int main() {
    test("checker", {{0, 1}, {1, 0}}, "*0110");
    test("uniform", {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}}, "1");
    test("nested", {{1, 1, 0, 1}, {1, 1, 0, 0}, {1, 1, 1, 1}, {1, 1, 1, 1}}, "*1*010011");
}
