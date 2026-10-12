#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

struct OldValue {
    bool existed;
    string value;
};

class TransactionKV {
private:
    unordered_map<string, string> store;
    vector<unordered_map<string, OldValue>> undo;

    // T: O(1), S: O(1)
    void remember(const string& key) {
        if (undo.empty() || undo.back().count(key)) { return; }

        auto it = store.find(key);
        if (it == store.end()) {
            undo.back()[key] = {false, ""};
        } else {
            undo.back()[key] = {true, it->second};
        }
    }

public:
    // T: O(1), S: O(1)
    string get(const string& key) const {
        auto it = store.find(key);
        return it == store.end() ? "" : it->second;
    }

    // T: O(1), S: O(1)
    void setValue(const string& key, const string& value) {
        remember(key);
        store[key] = value;
    }

    // T: O(1), S: O(1)
    void remove(const string& key) {
        remember(key);
        store.erase(key);
    }

    // T: O(1), S: O(1)
    void begin() { // construct an empty map at end of undo
        undo.emplace_back();
    }

    // T: O(k), S: O(k)
    bool rollback() {
        if (undo.empty()) { return false; }

        for (const auto& [key, old] : undo.back()) {
            if (old.existed) {
                store[key] = old.value;
            } else {
                store.erase(key);
            }
        }

        undo.pop_back();
        return true;
    }

    // T: O(k), S: O(k)
    bool commit() {
        if (undo.empty()) { return false; }

        if (undo.size() > 1) {
            auto& parent = undo[undo.size() - 2]; // undo = [outer log, inner log], [size() - 2, back()]

            for (const auto& [key, old] : undo.back()) { // merge inner log into outer log
                parent.emplace(key, old); // preserve parent's older value
            }
        }

        undo.pop_back();
        return true;
    }
};

int main() {
    TransactionKV kv;
    kv.setValue("x", "1");

    kv.begin();                // Outer: original x=1.
    kv.setValue("x", "2");
    kv.setValue("x", "3");      // Must still remember x=1.

    kv.begin();                // Inner: original x=3.
    kv.remove("x");
    assert(kv.get("x").empty());
    kv.setValue("x", "4");      // Delete/recreate.
    kv.setValue("y", "9");      // Created only in inner transaction.

    assert(kv.commit());
    assert(kv.get("x") == "4");
    assert(kv.get("y") == "9");

    assert(kv.rollback());
    assert(kv.get("x") == "1"); // Preserve parent's original.
    assert(kv.get("y").empty()); // Merge child's undo into parent.

    kv.begin();
    kv.setValue("x", "5");
    assert(kv.commit());
    assert(kv.get("x") == "5"); // Outermost commit.

    assert(!kv.commit());      // No open transaction.
    assert(!kv.rollback());
}
