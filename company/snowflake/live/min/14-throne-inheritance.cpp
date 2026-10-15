// A11 · LC 1600: ThroneInheritance(king), birth(parent, child), death(name), getInheritanceOrder() = preorder from king, children in birth order, dead skipped.
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
public:
    ThroneInheritance(string kingName) : king(kingName) {}

    // T: O(1) average
    void birth(string parentName, string childName) { children[parentName].push_back(childName); }

    // mark only, the person stays in the tree, T: O(1) average
    void death(string name) { dead.insert(name); }

    // recursive preorder DFS, T: O(N), S: O(N) output + O(H) recursion
    vector<string> getInheritanceOrder() {
        vector<string> order;
        preorder(king, order);
        return order;
    }

private:
    string king;                                     // root of the family tree
    unordered_map<string, vector<string>> children;  // person -> children in birth order
    unordered_set<string> dead;                      // people who have died

    void preorder(const string& person, vector<string>& order) {
        if (dead.count(person) == 0) { order.push_back(person); }
        auto it = children.find(person);             // find, not [], so a leaf gets no empty entry
        if (it == children.end()) { return; }
        for (const string& child : it->second) { preorder(child, order); }
    }
};

void check(const vector<string>& got, const vector<string>& expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    ThroneInheritance t("king");
    t.birth("king", "andy");
    t.birth("king", "bob");
    t.birth("king", "catherine");
    t.birth("andy", "matthew");
    t.birth("bob", "alex");
    t.birth("bob", "asha");
    check(t.getInheritanceOrder(), {"king", "andy", "matthew", "bob", "alex", "asha", "catherine"}, "LC order");
    t.death("bob");
    check(t.getInheritanceOrder(), {"king", "andy", "matthew", "alex", "asha", "catherine"}, "LC after bob dies");

    ThroneInheritance s("k");
    s.death("k");
    check(s.getInheritanceOrder(), {}, "king dead, nobody else");                    // smallest tree
    s.birth("k", "a");
    s.birth("k", "b");
    s.birth("a", "c");                                                               // born after b, still ahead of b
    check(s.getInheritanceOrder(), {"a", "c", "b"}, "subtree before next sibling");  // preorder, not birth time
    s.death("a");
    check(s.getInheritanceOrder(), {"c", "b"}, "dead parent keeps child in place");
}
