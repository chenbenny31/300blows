// birth(parent, child), death(name), getInheritanceOrder() (Q08, LC1600 variant, H01 IC1)
// no king given at construction: the parent in the first birth becomes the king
// order = preorder from the king, children in birth order, dead people skipped but their children kept

// how is the king created: first birth's parent, or a separate call?
// birth with an unknown parent: reject, or start a new tree?
// names unique and non-empty? a child born twice?
// dead person's children still inherit?
// how deep can the family tree get?

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

// T: birth / death O(1) average, getInheritanceOrder O(n), S: O(n)

class ThroneInheritance {
private:
    string king; // empty until the first birth names one
    std::unordered_set<string> people; // everyone known, king included
    std::unordered_map<string, vector<string>> children; // person -> children in birth order
    std::unordered_set<string> dead;

public:
    bool birth(const string& parent, const string& child) {
        if (king.empty()) {
            king = parent; // first birth creates the root
            people.insert(parent);
        }
        if (people.count(parent) == 0 || people.count(child) != 0) {
            return false; // unknown parent, or name already used
        }
        children[parent].push_back(child);
        people.insert(child);
        return true;
    }

    void death(const string& name) {
        dead.insert(name); // stays in the tree, only skipped in the order
    }

    vector<string> getInheritanceOrder() {
        vector<string> order;
        if (!king.empty()) {
            preorder(king, order);
        }
        return order;
    }

private:
    void preorder(const string& person, vector<string>& order) {
        if (dead.count(person) == 0) {
            order.push_back(person);
        }
        auto it = children.find(person); // find, not [], a leaf must not get an empty entry
        if (it == children.end()) {
            return;
        }
        for (const string& c : it->second) {
            preorder(c, order);
        }
    }
};

int main() {
    ThroneInheritance t;
    vector<string> want;

    assert(t.getInheritanceOrder().empty()); // null: no king yet

    t.birth("king", "andy");
    t.birth("king", "bob");
    t.birth("andy", "matt");
    want = {"king", "andy", "matt", "bob"};
    assert(t.getInheritanceOrder() == want); // target: andy's line before bob

    t.death("andy");
    want = {"king", "matt", "bob"};
    assert(t.getInheritanceOrder() == want); // edge: dead skipped, his child keeps the spot

    assert(!t.birth("ghost", "x")); // fault: unknown parent rejected
    assert(t.getInheritanceOrder() == want);

    std::cout << "all tests passed\n";
    return 0;
}
