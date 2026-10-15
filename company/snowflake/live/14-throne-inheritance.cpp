// A11 · LC 1600: ThroneInheritance(kingName), birth(parent, child), death(name), getInheritanceOrder() = preorder from the king,
// children in birth order, dead people skipped but their descendants kept.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

class ThroneInheritance {
public:
    ThroneInheritance(string kingName) : king(kingName) {}

    // T: O(1) average
    void birth(string parentName, string childName) {
        children[parentName].push_back(childName);
    }

    // T: O(1) average, the person stays in the tree
    void death(string name) {
        dead.insert(name);
    }

    // recursive preorder DFS, T: O(N), S: O(N) output + O(H) recursion
    vector<string> getInheritanceOrder() {
        vector<string> order;
        preorder(king, order);
        return order;
    }

    // explicit-stack preorder for deep family chains, T: O(N), S: O(N)
    vector<string> getInheritanceOrder2() {
        vector<string> order;
        vector<string> stk = {king};       // people still to visit, top = next in line
        while (!stk.empty()) {
            string cur = stk.back();
            stk.pop_back();
            if (dead.count(cur) == 0) { order.push_back(cur); }
            auto it = children.find(cur);
            if (it == children.end()) { continue; }
            const vector<string>& kids = it->second;
            for (int i = (int)kids.size() - 1; i >= 0; i--) {   // push youngest first so the eldest pops first
                stk.push_back(kids[i]);
            }
        }
        return order;
    }

private:
    string king;                                   // root of the family tree
    unordered_map<string, vector<string>> children; // person -> children in birth order
    unordered_set<string> dead;                    // people who have died

    void preorder(const string& person, vector<string>& order) {
        if (dead.count(person) == 0) { order.push_back(person); }
        auto it = children.find(person);           // find, not [], so a leaf gets no empty entry
        if (it == children.end()) { return; }
        for (const string& child : it->second) {
            preorder(child, order);
        }
    }
};

bool check(ThroneInheritance& t, const vector<string>& expected, const string& label) {
    vector<string> got = t.getInheritanceOrder();
    vector<string> got2 = t.getInheritanceOrder2();
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << ": got size " << got.size() << " / " << got2.size()
         << ", expected size " << expected.size() << "\n";
    return false;
}

bool testLeetCodeExample() {
    bool ok = true;
    ThroneInheritance t("king");
    t.birth("king", "andy");
    t.birth("king", "bob");
    t.birth("king", "catherine");
    t.birth("andy", "matthew");
    t.birth("bob", "alex");
    t.birth("bob", "asha");
    ok &= check(t, {"king", "andy", "matthew", "bob", "alex", "asha", "catherine"}, "lc order");
    t.death("bob");
    ok &= check(t, {"king", "andy", "matthew", "alex", "asha", "catherine"}, "lc after bob dies");
    return ok;
}

bool testOnlyKing() {
    bool ok = true;
    ThroneInheritance t("k");
    ok &= check(t, {"k"}, "only king");
    t.death("k");
    ok &= check(t, {}, "king dead, nobody else");
    t.birth("k", "a");                             // a dead parent can still have children
    ok &= check(t, {"a"}, "child of dead king");
    return ok;
}

bool testGrandchildBeforeUncle() {
    bool ok = true;
    ThroneInheritance t("k");
    t.birth("k", "a");
    t.birth("k", "b");
    t.birth("a", "c");                             // born after b, still ahead of b
    t.birth("c", "d");
    ok &= check(t, {"k", "a", "c", "d", "b"}, "whole subtree before next sibling");
    t.death("a");
    t.death("c");
    ok &= check(t, {"k", "d", "b"}, "two dead generations keep d in place");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testLeetCodeExample();          // LC 1600 example
    ok &= testOnlyKing();                 // smallest tree, king dies, birth under a dead parent
    ok &= testGrandchildBeforeUncle();    // preorder vs birth-time order, consecutive dead ancestors
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
