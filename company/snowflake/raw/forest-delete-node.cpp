#include <cassert>
#include <vector>

using std::vector;

// T: O(n), S: O(n)
vector<int> deleteNode(const vector<int>& parent, int x) {
    int n = static_cast<int>(parent.size());
    bool isRoot = (parent[x] == x);
    vector<int> result;

    for (int i = 0; i < n; i++) {
        if (i == x) { continue; }
        int p = parent[i];
        if (p == x) {
            p = isRoot ? i : parent[x];
        }
        result.push_back(p < x ? p : p - 1);
    }
    return result;
}

int main() {
    // 0 -> 1 -> 2 -> 3 delete 1: 0 -> 2 -> 3
    vector<int> want = {0, 0, 1};
    assert(deleteNode({0, 0, 1, 2}, 1) == want);

    // del root: all becomes roots
    // del node has larger index parent: all re-map
    // del the leaf: not shift
    // sep trees: still shift
    // del the only node
}
