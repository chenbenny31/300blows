#include <cassert>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

class TransactionKV {
private:
    std::unordered_map<string, string> store;
    vector<std::unordered_map<string, string>> snapshots;

public:
    // T: O(1), S: O(1)
    string get(const string& key) const {
        auto it = store.find(key);
        return it == store.end() ? "" : it->second;
    }

    // T: O(1), S: O(1)
    void setValue(const string& key, const string& value) {
        store[key] = value;
    }

    // T: O(1), S: O(1)
    void remove(const string& key) {
        store.erase(key);
    }

    // T: O(n) amortized, S: O(n)
    void begin() {
        snapshots.push_back(store);
    }

    // T: O(n), S: O(1)
    bool rollback() {
        if (snapshots.empty()) { return false; }
        store = std::move(snapshots.back());
        snapshots.pop_back();
        return true;
    }

    // T: O(n), S: O(1)
    bool commit() {
        if (snapshots.empty()) { return false; }
        snapshots.pop_back();
        return true;
    }
};

int main() {
    TransactionKV kv;
    kv.setValue("x", "1");

    kv.begin();
    kv.setValue("x", "2");
    kv.setValue("x", "3");
    assert(kv.get("x") == "3");
    assert(kv.rollback());
    assert(kv.get("x") == "1");

    kv.begin(); // x=1
    kv.setValue("x", "2");
    kv.begin(); // x=2
    kv.setValue("x", "3");

    assert(kv.commit());
    assert(kv.get("x") == "3");
    assert(kv.rollback());
    assert(kv.get("x") == "1");
}
