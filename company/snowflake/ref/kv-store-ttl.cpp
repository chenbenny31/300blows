// Build a KV store with get, set, update and delete. Add a TTL so entry expires ttl after last write then drop
// re-new only works on a live entry

// TTL relative to the write(now + ttl) or absolute expiry?
// update renews TTL?
// now==expiry, is entry still valid
// now non-decreasing?
// get return on a missing or expried key? delete return?

// T: O(logN) amortized per op, list adds O(KlogK), N: live entries, K: entires returned
// S: O(N)

#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <utility>
// #include <optional>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::string;
using std::vector;
using std::pair;

struct Entry {
    string value;
    long long expiredAt; // first time no longer valid
};

class TTLKVStore {
private:
    std::unordered_map<string, Entry> store; // key -> live-value, expiry
    std::set<pair<long long, string>> byExpiry; // (expiredAt, key), earliest expiry first

    // drop every entry with expireAt <= now, each entry leaves once -> amortized
    void purge(long long now) {
        while (!byExpiry.empty() && byExpiry.begin()->first <= now) {
            store.erase(byExpiry.begin()->second); // erasing the Entry first before set
            byExpiry.erase(byExpiry.begin());
        }
    }

public:
    void set(const string& key, const string& value, long long ttl, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it != store.end()) {
            byExpiry.erase({it->second.expiredAt, key});
        }
        store[key] = {value, now + ttl};
        byExpiry.insert({now + ttl, key});
    }

    // std::optional<string> get(const string& key, long long now) {
    string get(const string& key, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it == store.end()) {
            // return std::nullopt;
            return {}; // values are non-empty by contract
        }
        return it->second.value;
    }

    // live-key onlye, renews its ttl, exclude expired keys
    bool update(const string& key, const string& value, long long ttl, long long now) {
        purge(now);
        if (store.count(key) == 0) {
            return false;
        }
        set(key, value, ttl, now);
        return true;
    }

    bool remove(const string& key, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it == store.end()) {
            return false;
        }
        byExpiry.erase({it->second.expiredAt, key});
        store.erase(it);
        return true;
    }

    int countValid(long long now) {
        purge(now);
        return static_cast<int>(store.size());
    }

    vector<pair<string, string>> listValid(long long now) {
        purge(now);
        vector<pair<string, string>> result;
        for (const auto& kv : store) {
            result.push_back({kv.first, kv.second.value});
        }
        std::sort(result.begin(), result.end());
        return result;
    }
};

int main() {
    TTLKVStore kv;

    kv.set("a", "1", 5, 0); // expires at 5
    assert(kv.get("a", 4) == "1");
    // assert(!kv.get("a", 5)); // now == expireAt is expired
    assert(kv.get("a", 5).empty());

    kv.set("b", "2", 5, 10); // expired at 15
    kv.set("b", "3", 5, 13); // overwrite, expires at 18
    assert(kv.get("b", 16) == "3");

    assert(!kv.update("zz", "x", 5, 16)); // missing key
    assert(kv.update("b", "4", 10, 17)); // renew, expired at 27
    kv.set("c", "5", 100, 17);
    assert(kv.countValid(20) == 2);
    vector<pair<string, string>> want = {{"b", "4"}, {"c", "5"}};
    assert(kv.listValid(20) == want);

    assert(kv.remove("c", 21));
    assert(!kv.remove("c", 21)); // already removed
    assert(!kv.update("b", "5", 5, 27)); // expired at 27, can't renew
    assert(kv.countValid(27) == 0);
    assert(kv.listValid(27).empty());

    std::cout << "all tests passed\n";
    return 0;
}

