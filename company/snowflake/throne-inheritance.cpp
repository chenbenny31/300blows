#include <cassert>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

class ThroneInheritance {
    string king;
    unordered_set<string> people;
    unordered_set<string> dead;
    unordered_map<string, vector<string>> children; // parent -> birth-order children

    // T: O(n), S: O(n)
    void dfs(const string& person, vector<string>& order) const {
        if (!dead.count(person)) {
            order.push_back(person);
        }

        auto it = children.find(person);
        if (it == children.end()) { return; }
        for (const string& child : it->second) {
            dfs(child, order);
        }
    }

public:
    // T: O(1), S: O(n)
    bool birth(const string& parent, const string& child) {
        if (parent == child) { return false; }
        if (king.empty()) {
            king = parent;
            people.insert(parent);
        }
        if (!people.count(parent) || people.count(child)) {
            return false;
        }

        children[parent].push_back(child);
        people.insert(child);
        return true;
    }

    void death(const string& name) {
        dead.insert(name);
    }

    vector<string> getInheritanceOrder() const {
        vector<string> order;
        if (!king.empty()) {
            dfs(king, order);
        }
        return order;
    }
};

int main() {
    ThroneInheritance t;
    assert(t.getInheritanceOrder().empty());

    assert(t.birth("king", "andy"));
    assert(t.birth("king", "bob"));
    assert(t.birth("andy", "matt"));

    vector<string> expected = {"king", "andy", "matt", "bob"};
    assert(t.getInheritanceOrder() == expected); // Descendants before siblings.

    t.death("andy");
    expected = {"king", "matt", "bob"};
    assert(t.getInheritanceOrder() == expected); // Dead parent's child stays.

    assert(!t.birth("ghost", "x")); // Unknown parent.
    assert(!t.birth("king", "bob")); // Duplicate name.
    assert(t.getInheritanceOrder() == expected); // Rejections change nothing.
}
