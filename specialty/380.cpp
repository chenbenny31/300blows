// LC 380 Insert Delete GetRandom O(1): set with insert/remove (return whether changed)
// and getRandom uniform over current elements, all average O(1).
// Trick: vector + val->index map; remove = move the last element into the hole, pop back
// Recognize: O(1) delete AND O(1) uniform random -> dense array plus index hash map
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::rand;

class RandomizedSet {
    vector<int> vals;                // the elements, in arbitrary order
    unordered_map<int, int> index;   // val -> its position in vals

public:
    // Append and record the position. T: O(1) average.
    bool insert(int val) {
        if (index.count(val)) { return false; }
        index[val] = vals.size();
        vals.push_back(val);
        return true;
    }

    // Move the last element into val's slot, then pop the back. T: O(1) average.
    bool remove(int val) {
        auto it = index.find(val);
        if (it == index.end()) { return false; }
        int last = vals.back();
        vals[it->second] = last;
        index[last] = it->second;   // before erase: val may be last itself
        vals.pop_back();
        index.erase(val);
        return true;
    }

    // Random position in the dense vector. T: O(1).
    int getRandom() { return vals[rand() % vals.size()]; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RandomizedSet s;
    check(s.insert(1) && !s.remove(2) && s.insert(2), "lc ex1 ops");
    int r = s.getRandom();
    check(r == 1 || r == 2, "random member");
    check(s.remove(1) && !s.insert(2), "remove, dup insert");
    check(s.getRandom() == 2, "single left");
    check(s.remove(2) && s.insert(2), "remove last");
}
