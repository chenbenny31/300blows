
#include <cassert>
#include <vector>

using std::vector;

struct Node {
    bool val;
    bool isLeaf;
    Node* topLeft = nullptr;
    Node* topRight = nullptr;
    Node* bottomLeft = nullptr;
    Node* bottomRight = nullptr;
};

class QuadTreeBuilder {
    // T: O(n² log n), S: O(log n) recursion, excluding output.
    Node* build(const vector<vector<int>>& grid, int r, int c, int size) {
        bool uniform = true;
        for (int i = r; i < r + size && uniform; ++i) {
            for (int j = c; j < c + size; ++j) {
                if (grid[i][j] != grid[r][c]) {
                    uniform = false;
                    break;
                }
            }
        }

        if (uniform) {
            return new Node{grid[r][c] == 1, true};
        }

        int half = size / 2;
        Node* node = new Node{false, false};
        node->topLeft = build(grid, r, c, half);
        node->topRight = build(grid, r, c + half, half);
        node->bottomLeft = build(grid, r + half, c, half);
        node->bottomRight = build(grid, r + half, c + half, half);
        return node;
    }

public:
    // T: O(n² log n), S: O(n²), including output.
    Node* construct(const vector<vector<int>>& grid) {
        return build(grid, 0, 0, static_cast<int>(grid.size()));
    }
};

// Caller owns the returned tree. Test cleanup:
// T: O(number of nodes), S: O(tree height).
void destroy(Node* node) {
    if (!node) {
        return;
    }
    destroy(node->topLeft);
    destroy(node->topRight);
    destroy(node->bottomLeft);
    destroy(node->bottomRight);
    delete node;
}

int main() {
    QuadTreeBuilder builder;

    Node* root = builder.construct({{1, 0}, {1, 1}});
    assert(!root->isLeaf);
    assert(root->topLeft->isLeaf && root->topLeft->val);
    assert(root->topRight->isLeaf && !root->topRight->val);
    assert(root->bottomLeft->isLeaf && root->bottomLeft->val);
    assert(root->bottomRight->isLeaf && root->bottomRight->val);
    destroy(root);

    root = builder.construct({{0, 0}, {0, 0}});
    assert(root->isLeaf && !root->val); // Uniform region stays one leaf.
    destroy(root);

    root = builder.construct({{1}});
    assert(root->isLeaf && root->val); // Single cell.
    destroy(root);
}
