// forest as a parent array, parent[i] == i means i is a root; delete node x
// x's direct children move up to parent[x]; if x is a root, each child becomes its own root
// survivors are re-indexed in original order and every parent value is renumbered (Q41, J11189 NG)

// root encoded as parent[i] == i, or -1?
// x is a root: children become separate roots?
// keep survivors in original order?
// return only the new parent array, or payload / old->new map too?
// input always a valid forest (no cycles)? x in range?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

class ForestEditor {
public:
    // pick each survivor's parent in OLD indices, then shift every index past x down by one
    // T: O(n), S: O(n) for the output, input not modified
    vector<int> deleteNode(const vector<int>& parent, int x) {
        int n = static_cast<int>(parent.size());
        bool xIsRoot = parent[x] == x;
        vector<int> result; // new parent array, n - 1 entries
        for (int i = 0; i < n; i++) {
            if (i == x) {
                continue;
            }
            int p = parent[i]; // old index
            if (p == x) {
                p = xIsRoot ? i : parent[x]; // child of x: becomes a root, or moves up to x's parent
            }
            result.push_back(p < x ? p : p - 1); // old -> new index, p is never x here
        }
        return result;
    }
};

int main() {
    ForestEditor fe;
    vector<int> want;

    assert(fe.deleteNode({0}, 0).empty()); // only node

    want = {0, 1};
    assert(fe.deleteNode({0, 0, 0}, 0) == want); // root with two children, each becomes a root

    want = {0, 0, 1};
    assert(fe.deleteNode({0, 0, 1, 2}, 1) == want); // chain 0 <- 1 <- 2 <- 3, 2 moves up to 0, 3 keeps 2 renumbered

    want = {0, 0};
    assert(fe.deleteNode({0, 0, 1}, 2) == want); // x is the last index, nothing shifts

    want = {0, 1, 1, 2};
    assert(fe.deleteNode({0, 0, 2, 2, 3}, 1) == want); // two trees, every index after x shifts down

    want = {0, 0};
    assert(fe.deleteNode({1, 1, 0}, 0) == want); // x's parent has a larger index, must be renumbered too

    std::cout << "all tests passed\n";
    return 0;
}
