#include <algorithm>
#include <cassert>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::pair;
using std::vector;
using std::string;

struct Entry {
    string value;
    long long expiry;
};

class TTLKVStore {
private:
    std::unordered_map<string, Entry> store;
    std::set<pair<long long, string>> byExpiry; // std::set sorting by smaller expiry

    // T: O(k) amortized, k = exipred entries, S: O(1)
    void purge(long long now) {
        while (!byExpiry.empty() && byExpiry.begin()->first <= now) {
            store.erase(byExpiry.begin()->second);
            byExpiry.erase(byExpiry.begin());
        }
    }

public:
    // T: O(logn) + purge, S: O(1)
    void setValue(const string& key, const string& value,
                  long long ttl, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it != store.end()) {
            byExpiry.erase({it->second.expiry, key});
        }
        long long expiry = now + ttl;
        store[key] = {value, expiry};
        byExpiry.insert({expiry, key});
    }

    // T: O(1) + purge, S: O(1)
    string get(const string& key, long long now) {
        purge(now);
        auto it = store.find(key);
        return it == store.end() ? "" : it->second.value;
    }

    // T: O(logn) + purge, S: O(1)
    bool update(const string& key, const string& value,
                long long ttl, long long now) {
        purge(now);
        if (store.count(key) == 0) { return false; }
        setValue(key, value, ttl, now);
        return true;
    }

    // T: O(logn) + purge, S: O(1)
    bool remove(const string& key, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it == store.end()) { return false; }
        byExpiry.erase({it->second.expiry, key});
        store.erase(it);
        return true;
    }

    // T: O(nlogn) + purge, S: O(n)
    vector<pair<string, string>> listValid(long long now) {
        purge(now);
        vector<pair<string, string>> result;
        for (const auto& [key, entry] : store) {
            result.push_back({key, entry.value});
        }
        std::sort(result.begin(), result.end());
        return result;
    }
};

int main() {
    TTLKVStore kv;

    kv.setValue("a", "old", 5, 0); // expiry=5
    kv.setValue("a", "new", 5, 3); // overwrites, expiry=8
    assert(kv.get("a", 5) == "new");
    assert(kv.get("a", 8).empty()); // exact expiry
    assert(!kv.update("missing", "x", 5, 8)); // cannot create via update

    kv.setValue("b", "2", 5, 10);
    assert(kv.update("b", "3", 5, 12)); // expiry=17
    assert(kv.get("b", 15) == "3");

    kv.setValue("a", "1", 10, 15);
    vector<pair<string, string>> want = {{"a", "1"}, {"b", "3"}};
    assert(kv.listValid(15) == want); // sort by keys

    assert(kv.remove("a", 16));
    assert(!kv.remove("a", 16));
    assert(!kv.update("b", "4", 5, 17)); // cannot renew expired keys
    assert(kv.listValid(17).empty());
}
