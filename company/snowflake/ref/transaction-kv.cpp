// KV store: get, set, remove; then begin, commit, rollback with nested transactions (Q28, C18)
// rollback undoes every change since the matching begin; commit keeps the changes and closes
// the innermost transaction (its changes become part of the parent, so a parent rollback still undoes them)

// nested transactions allowed? commit closes the innermost or all?
// commit / rollback with no open transaction: error or no-op?
// "constant time": for which operations? worst case or amortized?
// get on a missing key returns? (here "", values are non-empty)
// concurrency / isolation between clients? (single client here)

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;
using std::pair;

// 1: begin copies the whole store, T: get/set/remove O(1), begin O(N), rollback O(N), commit O(N) to free, S: O(N * depth)
class TransactionKV_1 {
private:
    std::unordered_map<string, string> store; // key -> value
    vector<std::unordered_map<string, string>> snapshots; // store as it was at each open begin

public:
    string get(const string& key) {
        auto it = store.find(key);
        return it == store.end() ? "" : it->second;
    }

    void set(const string& key, const string& value) {
        store[key] = value;
    }

    void remove(const string& key) {
        store.erase(key);
    }

    void begin() {
        snapshots.push_back(store);
    }

    bool rollback() {
        if (snapshots.empty()) {
            return false;
        }
        store = std::move(snapshots.back());
        snapshots.pop_back();
        return true;
    }

    bool commit() {
        if (snapshots.empty()) {
            return false;
        }
        snapshots.pop_back(); // current store already holds the changes
        return true;
    }
};

// 2: undo log per transaction, value before its first write there
// T: get/set/remove O(1), begin O(1), rollback / commit O(K), K: keys touched in that transaction, S: O(N + total K)
class TransactionKV_2 {
private:
    std::unordered_map<string, string> store; // key -> value
    vector<std::unordered_map<string, pair<bool, string>>> undo; // per open transaction: key -> (existed, old value)

public:
    string get(const string& key) {
        auto it = store.find(key);
        return it == store.end() ? "" : it->second;
    }

    void set(const string& key, const string& value) {
        remember(key);
        store[key] = value;
    }

    void remove(const string& key) {
        remember(key);
        store.erase(key);
    }

    void begin() {
        undo.push_back({});
    }

    bool rollback() {
        if (undo.empty()) {
            return false;
        }
        for (const auto& kv : undo.back()) {
            if (kv.second.first) {
                store[kv.first] = kv.second.second; // restore old value
            } else {
                store.erase(kv.first); // key did not exist before
            }
        }
        undo.pop_back();
        return true;
    }

    bool commit() {
        if (undo.empty()) {
            return false;
        }
        if (undo.size() > 1) {
            auto& parent = undo[undo.size() - 2];
            for (const auto& kv : undo.back()) {
                parent.emplace(kv.first, kv.second); // emplace keeps the parent's older original
            }
        }
        undo.pop_back();
        return true;
    }

private:
    void remember(const string& key) {
        if (undo.empty() || undo.back().count(key) != 0) {
            return; // no transaction, or this transaction already saved the original
        }
        auto it = store.find(key);
        undo.back()[key] = (it == store.end()) ? pair<bool, string>(false, "") : pair<bool, string>(true, it->second);
    }
};

int main() {
    TransactionKV_1 a;
    assert(a.get("x") == ""); // null: missing key
    assert(!a.commit()); // fault: no open transaction
    assert(!a.rollback());
    a.set("x", "1");
    a.begin();
    a.set("x", "2");
    a.set("x", "3"); // same key twice in one transaction
    assert(a.rollback());
    assert(a.get("x") == "1"); // target: rollback restores the value before begin
    a.begin();
    a.remove("x");
    a.set("y", "5");
    a.begin();
    a.set("x", "9"); // re-create a key deleted by the outer transaction
    assert(a.commit()); // inner commit
    assert(a.get("x") == "9");
    assert(a.rollback()); // outer rollback undoes the committed inner changes too
    assert(a.get("x") == "1"); // edge: nested + delete then re-create
    assert(a.get("y") == ""); // key created inside the transaction is gone

    TransactionKV_2 b;
    assert(b.get("x") == ""); // null
    assert(!b.commit()); // fault
    assert(!b.rollback());
    b.set("x", "1");
    b.begin();
    b.set("x", "2");
    b.set("x", "3");
    assert(b.rollback());
    assert(b.get("x") == "1"); // target: only the first write in a transaction is remembered
    b.begin();
    b.remove("x");
    b.set("y", "5");
    b.begin();
    b.set("x", "9");
    assert(b.commit());
    assert(b.get("x") == "9");
    assert(b.rollback());
    assert(b.get("x") == "1"); // edge: parent kept its own original of x, not the inner one
    assert(b.get("y") == "");

    std::cout << "all tests passed\n";
    return 0;
}
