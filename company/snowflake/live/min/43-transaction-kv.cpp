// I02 · LC 706/1146 partial prototypes: single-client KV with get/set/del and nested begin/commit/rollback; commit/rollback with no open transaction return false.
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

// per-level undo log, get/set/del/begin T: O(1) avg, commit/rollback O(K) keys touched in top level, S: O(N + total K)
class TransactionalKV {
public:
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

    bool commit() {
        if (undo.empty()) { return false; }
        if (undo.size() >= 2) {
            auto& parent = undo[undo.size() - 2];
            for (const auto& [key, old] : undo.back()) { parent.emplace(key, old); }   // emplace: parent's older value wins
        }
        undo.pop_back();
        return true;
    }

    bool rollback() {
        if (undo.empty()) { return false; }
        for (const auto& [key, old] : undo.back()) {
            if (old) {
                store[key] = *old;
            } else {
                store.erase(key);   // absent before this level
            }
        }
        undo.pop_back();
        return true;
    }

private:
    unordered_map<string, string> store;                    // key -> current value
    vector<unordered_map<string, optional<string>>> undo;   // per open level: key -> value before its first write there (nullopt = absent)

    void remember(const string& key) {
        if (undo.empty() || undo.back().count(key)) { return; }   // only the first write in this level is logged
        auto it = store.find(key);
        undo.back()[key] = it == store.end() ? optional<string>() : it->second;
    }
};

void check(const string& got, const string& expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

void check(bool got, bool expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    TransactionalKV kv;
    check(kv.get("x"), "", "get missing");
    check(kv.commit(), false, "commit with no transaction");
    kv.set("x", "1");
    kv.begin();
    kv.set("x", "2");
    kv.set("x", "3");          // second write must not overwrite the logged "1"
    kv.set("y", "5");          // created inside the transaction
    check(kv.rollback(), true, "rollback");
    check(kv.get("x"), "1", "x restored to pre-begin value");
    check(kv.get("y"), "", "y created in txn is gone");

    kv.begin();                // L1
    kv.del("x");
    kv.begin();                // L2
    kv.set("x", "9");          // re-create a key L1 deleted
    kv.set("z", "7");          // touched only in L2
    check(kv.commit(), true, "inner commit");
    check(kv.get("x"), "9", "x after inner commit");
    check(kv.rollback(), true, "outer rollback");
    check(kv.get("x"), "1", "x: parent kept its earliest entry");
    check(kv.get("z"), "", "z: merged from L2, undone by L1");

    kv.begin();
    kv.set("a", "1");
    kv.begin();
    kv.set("a", "2");
    check(kv.rollback(), true, "inner rollback");
    check(kv.commit(), true, "outer commit");
    check(kv.get("a"), "1", "a permanent after outer commit");
    check(kv.rollback(), false, "rollback after everything committed");
}
