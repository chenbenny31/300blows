// A11 · LC 1600: ThroneInheritance(kingName), birth(parent, child), death(name),
// getInheritanceOrder() = preorder from the king, children in birth order, dead skipped.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

class ThroneInheritance {
    unordered_map<string, vector<string>> children;   // person -> children in birth order
    unordered_set<string> dead;                       // people who have died
    string king;                                      // root of the family tree

    // preorder: a person, then each child's whole subtree in birth order
    void preorder(const string& person, vector<string>& order) {
        if (!dead.count(person)) { order.push_back(person); }
        // find, not [], so a leaf gets no empty entry
        auto it = children.find(person);
        if (it == children.end()) { return; }
        for (const string& child : it->second) { preorder(child, order); }
    }

public:
    ThroneInheritance(const string& kingName) : king(kingName) {}

    // T: O(1) average.
    void birth(const string& parentName, const string& childName) {
        children[parentName].push_back(childName);
    }

    // Only mark: the dead stay in the tree, so descendants keep their place. T: O(1).
    void death(const string& name) { dead.insert(name); }

    // Recursive preorder DFS. T: O(N), S: O(N) output + O(H) recursion.
    vector<string> getInheritanceOrder() {
        vector<string> order;
        preorder(king, order);
        return order;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ThroneInheritance t("king");
    t.birth("king", "andy");
    t.birth("king", "bob");
    t.birth("andy", "matthew");
    check(t.getInheritanceOrder() == vector<string>{"king", "andy", "matthew", "bob"},
          "grandchild before uncle");
    t.death("andy");
    check(t.getInheritanceOrder() == vector<string>{"king", "matthew", "bob"},
          "skip dead");
    t.birth("andy", "cathy");
    check(t.getInheritanceOrder() == vector<string>{"king", "matthew", "cathy", "bob"},
          "birth under dead parent");
}
