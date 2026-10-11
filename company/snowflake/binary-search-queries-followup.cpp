#include <cassert>
#include <unordered_map>
#include <vector>

using std::vector;

class SearchIndex {
private:
    std::unordered_map<int, int> first; // val -> its first idx

public:
    // T: O(n) expect, S: O(n)
    explicit SearchIndex(const vector<int>& a) {
        for (int i = 0; i < static_cast<int>(a.size()); i++) {
            first.emplace(a[i], i); // emplace prevent dup overwrite
        }
    }

    // T: O(1) expect, O(n) worst, S: O(1)
    int search(int target) const {
        auto it = first.find(target);
        return it == first.end() ? -1 : it->second;
    }
};

int main() {
    SearchIndex si({1, 3, 5, 5, 5, 9});

    assert(si.search(5) == 2);
    // repeat query
    // missing val
    // last elem
    // empty si
}
