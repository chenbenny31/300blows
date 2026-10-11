// n x n grid of 0/1, n a power of 2: build its quad tree (LC427)
// a region of all-equal cells is one leaf with that value; otherwise split into 4 quadrants

// n always a power of 2? n >= 1?
// node format given? (here a pool of nodes linked by index, nothing to free)
// value of a non-leaf node: anything?
// non-square / non-power-of-2 grids later?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

struct QuadNode {
    bool val = false;
    bool isLeaf = false;
    int topLeft = -1; // child indices into the pool, -1 for a leaf
    int topRight = -1;
    int bottomLeft = -1;
    int bottomRight = -1;
};

class QuadTreeBuilder {
private:
    vector<QuadNode> pool; // every node built, root returned by index

public:
    // 1: check whether the region is uniform by scanning it, split if not
    // T: O(n^2 logn), each level rescans its cells, S: O(n^2) nodes + O(logn) recursion
    int construct_1(const vector<vector<int>>& grid) {
        pool.clear();
        return build_1(grid, 0, 0, static_cast<int>(grid.size()));
    }

    // 2: build the 4 children first, merge them into one leaf if all are leaves with the same value
    // T: O(n^2), every cell is a leaf once and every merge is O(1), S: O(n^2) nodes + O(logn) recursion
    int construct_2(const vector<vector<int>>& grid) {
        pool.clear();
        return build_2(grid, 0, 0, static_cast<int>(grid.size()));
    }

    const QuadNode& node(int i) const {
        return pool[i];
    }

private:
    int makeLeaf(bool val) {
        QuadNode leaf;
        leaf.val = val;
        leaf.isLeaf = true;
        pool.push_back(leaf);
        return static_cast<int>(pool.size()) - 1;
    }

    int makeInner(int tl, int tr, int bl, int br) {
        QuadNode inner;
        inner.topLeft = tl;
        inner.topRight = tr;
        inner.bottomLeft = bl;
        inner.bottomRight = br;
        pool.push_back(inner); // indices, not references: safe across reallocation
        return static_cast<int>(pool.size()) - 1;
    }

    int build_1(const vector<vector<int>>& grid, int r, int c, int size) {
        bool uniform = true;
        for (int i = r; i < r + size && uniform; i++) {
            for (int j = c; j < c + size; j++) {
                if (grid[i][j] != grid[r][c]) {
                    uniform = false;
                    break;
                }
            }
        }
        if (uniform) {
            return makeLeaf(grid[r][c] == 1);
        }
        int half = size / 2;
        int tl = build_1(grid, r, c, half);
        int tr = build_1(grid, r, c + half, half);
        int bl = build_1(grid, r + half, c, half);
        int br = build_1(grid, r + half, c + half, half);
        return makeInner(tl, tr, bl, br);
    }

    int build_2(const vector<vector<int>>& grid, int r, int c, int size) {
        if (size == 1) {
            return makeLeaf(grid[r][c] == 1);
        }
        int half = size / 2;
        int tl = build_2(grid, r, c, half);
        int tr = build_2(grid, r, c + half, half);
        int bl = build_2(grid, r + half, c, half);
        int br = build_2(grid, r + half, c + half, half);
        bool same = pool[tl].isLeaf && pool[tr].isLeaf && pool[bl].isLeaf && pool[br].isLeaf &&
                    pool[tl].val == pool[tr].val && pool[tl].val == pool[bl].val && pool[tl].val == pool[br].val;
        if (same) {
            return makeLeaf(pool[tl].val); // the 4 children stay in the pool, just unreferenced
        }
        return makeInner(tl, tr, bl, br);
    }
};

// test helper: leaves reachable from node i
int countLeaves(const QuadTreeBuilder& qb, int i) {
    const QuadNode& n = qb.node(i);
    if (n.isLeaf) {
        return 1;
    }
    return countLeaves(qb, n.topLeft) + countLeaves(qb, n.topRight) +
           countLeaves(qb, n.bottomLeft) + countLeaves(qb, n.bottomRight);
}

int main() {
    QuadTreeBuilder qb;
    vector<vector<int>> mixed = {{1, 1, 0, 1},
                                 {1, 1, 0, 0},
                                 {1, 1, 1, 1},
                                 {1, 1, 1, 1}};
    int root;

    root = qb.construct_1(mixed);
    assert(countLeaves(qb, root) == 7); // target: only the top-right quadrant splits again
    root = qb.construct_2(mixed);
    assert(countLeaves(qb, root) == 7);

    root = qb.construct_1({{0, 0}, {0, 0}});
    assert(qb.node(root).isLeaf && !qb.node(root).val); // edge: uniform grid is one leaf
    root = qb.construct_2({{0, 0}, {0, 0}});
    assert(qb.node(root).isLeaf && !qb.node(root).val);

    root = qb.construct_1({{1, 0}, {1, 1}});
    assert(countLeaves(qb, root) == 4); // edge: one different cell forces a split
    root = qb.construct_2({{1, 0}, {1, 1}});
    assert(countLeaves(qb, root) == 4);

    root = qb.construct_1({{1}});
    assert(qb.node(root).isLeaf && qb.node(root).val); // null: 1 x 1 grid
    root = qb.construct_2({{1}});
    assert(qb.node(root).isLeaf && qb.node(root).val);

    std::cout << "all tests passed\n";
    return 0;
}
