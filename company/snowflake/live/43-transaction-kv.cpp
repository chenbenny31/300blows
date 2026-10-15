// I02 (no direct LC; 706 / 1146 partial prototypes): single-client KV store with get / set / del and
// nested begin / commit / rollback; commit / rollback with no open transaction return false.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <optional>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::optional;

class TransactionalKV {
public:
    // get / set / del T: O(1) average, begin O(1), commit / rollback O(K), K: keys touched in the top transaction
    // S: O(N + total K), N: live keys
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

    void begin() {
        undo.push_back({});
    }

    bool commit() {
        if (undo.empty()) { return false; }
        if (undo.size() >= 2) {
            auto& parent = undo[undo.size() - 2];
            // emplace skips keys the parent already logged: its value is older and must win
            for (const auto& [key, old] : undo.back()) { parent.emplace(key, old); }
        }
        undo.pop_back();   // outermost commit: changes are already in store, nothing to merge
        return true;
    }

    bool rollback() {
        if (undo.empty()) { return false; }
        for (const auto& [key, old] : undo.back()) {
            if (old) {
                store[key] = *old;
            } else {
                store.erase(key);   // key did not exist before this transaction
            }
        }
        undo.pop_back();
        return true;
    }

private:
    unordered_map<string, string> store;                      // key -> current value
    vector<unordered_map<string, optional<string>>> undo;     // per open transaction: key -> value before its first write there (nullopt = absent)

    // save the pre-transaction value only on the first write to key in the top transaction
    void remember(const string& key) {
        if (undo.empty() || undo.back().count(key)) { return; }
        auto it = store.find(key);
        optional<string> old;   // nullopt = key absent
        if (it != store.end()) { old = it->second; }
        undo.back()[key] = old;
    }
};

bool check(const string& got, const string& expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got \"" << got << "\", expected \"" << expected << "\"\n";
    return false;
}

bool check(bool got, bool expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool testBasic() {
    bool ok = true;
    TransactionalKV kv;
    ok &= check(kv.get("a"), "", "get missing");
    kv.set("a", "1");
    ok &= check(kv.get("a"), "1", "get after set");
    kv.set("a", "2");
    ok &= check(kv.get("a"), "2", "overwrite");
    kv.del("a");
    ok &= check(kv.get("a"), "", "get after del");
    kv.del("zz");   // deleting a missing key is a no-op
    ok &= check(kv.commit(), false, "commit with no transaction");
    ok &= check(kv.rollback(), false, "rollback with no transaction");
    return ok;
}

bool testRollback() {
    bool ok = true;
    TransactionalKV kv;
    kv.set("x", "1");
    kv.begin();
    kv.set("x", "2");
    kv.set("x", "3");   // second write in the same transaction must not overwrite the saved "1"
    kv.del("x");
    kv.set("y", "5");   // created inside the transaction
    ok &= check(kv.rollback(), true, "rollback");
    ok &= check(kv.get("x"), "1", "x restored to pre-begin value");
    ok &= check(kv.get("y"), "", "y created in txn is gone");
    return ok;
}

bool testNested() {
    bool ok = true;
    TransactionalKV kv;
    kv.set("x", "1");
    kv.begin();          // L1
    kv.del("x");
    kv.set("y", "5");
    kv.begin();          // L2
    kv.set("x", "9");    // re-create a key L1 deleted
    kv.set("y", "6");
    kv.set("z", "7");    // touched only in L2
    ok &= check(kv.commit(), true, "inner commit");
    ok &= check(kv.get("x"), "9", "x after inner commit");
    ok &= check(kv.get("y"), "6", "y after inner commit");
    ok &= check(kv.rollback(), true, "outer rollback");
    ok &= check(kv.get("x"), "1", "x: parent kept its earliest entry");
    ok &= check(kv.get("y"), "", "y: absent before L1");
    ok &= check(kv.get("z"), "", "z: merged from L2, undone by L1");
    ok &= check(kv.commit(), false, "no transaction left");
    return ok;
}

bool testInnerRollback() {
    bool ok = true;
    TransactionalKV kv;
    kv.begin();
    kv.set("a", "1");
    kv.begin();
    kv.set("a", "2");
    kv.del("b");         // delete of a missing key inside a txn, rollback must not create it
    ok &= check(kv.rollback(), true, "inner rollback");
    ok &= check(kv.get("a"), "1", "a back to outer value");
    ok &= check(kv.get("b"), "", "b still absent");
    ok &= check(kv.commit(), true, "outer commit");
    ok &= check(kv.get("a"), "1", "a permanent after outer commit");
    ok &= check(kv.rollback(), false, "rollback after everything committed");
    ok &= check(kv.get("a"), "1", "failed rollback changed nothing");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testBasic();           // CRUD + commit / rollback with no open transaction
    ok &= testRollback();        // single level: repeated writes, delete, new key
    ok &= testNested();          // inner commit merges, parent's earliest value wins on outer rollback
    ok &= testInnerRollback();   // inner rollback then outer commit makes changes permanent
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
