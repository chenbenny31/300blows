// I02 (no direct LC; 706 / 1146 partial prototypes): single-client KV store with get /
// set / del and nested begin / commit / rollback; commit / rollback with no open
// transaction return false.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::optional;
using std::nullopt;

class TransactionalKV {
    unordered_map<string, string> store;   // key -> current value
    // undo[i] = key -> its value before level i's first write to it (nullopt = absent)
    vector<unordered_map<string, optional<string>>> undo;

    // Log key's current value on its first write in the top level only. T: O(1).
    void remember(const string& key) {
        if (undo.empty() || undo.back().count(key)) { return; }
        auto it = store.find(key);
        if (it == store.end()) {
            undo.back()[key] = nullopt;
        } else {
            undo.back()[key] = it->second;
        }
    }

public:
    // Current data always lives in store. T: O(1) average.
    string get(const string& key) {
        auto it = store.find(key);
        return it == store.end() ? "" : it->second;
    }

    void set(const string& key, const string& value) {
        remember(key);
        store[key] = value;
    }

    void del(const string& key) {
        remember(key);
        store.erase(key);
    }

    void begin() { undo.push_back({}); }

    // Merge the top log into its parent; the parent's older entry wins.
    // T: O(K), K = keys written in the top level.
    bool commit() {
        if (undo.empty()) { return false; }
        if (undo.size() >= 2) {
            auto& parent = undo[undo.size() - 2];
            // emplace does nothing if the parent already logged this key
            for (const auto& [key, old] : undo.back()) { parent.emplace(key, old); }
        }
        undo.pop_back();   // outermost commit: changes are already in store
        return true;
    }

    // Restore every key in the top log. T: O(K).
    bool rollback() {
        if (undo.empty()) { return false; }
        for (const auto& [key, old] : undo.back()) {
            if (old) {
                store[key] = *old;
            } else {
                store.erase(key);   // key did not exist before this level
            }
        }
        undo.pop_back();
        return true;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TransactionalKV kv;
    check(!kv.commit() && !kv.rollback(), "no transaction");
    kv.set("user", "alice");
    kv.begin();
    kv.set("user", "bob");
    kv.set("user", "carol");
    kv.set("role", "admin");
    check(kv.rollback() && kv.get("user") == "alice" && kv.get("role") == "", "rollback");
    kv.begin();
    kv.del("user");
    kv.begin();
    kv.set("user", "dave");
    check(kv.commit() && kv.get("user") == "dave", "inner commit");
    check(kv.rollback() && kv.get("user") == "alice", "outer rollback");
    kv.begin();
    kv.set("role", "admin");
    check(kv.commit() && !kv.rollback() && kv.get("role") == "admin", "outer commit");
}
