// B04 (LC 706 / 1797 parts): KV store with get/set/update/remove, TTL expiry, and listValid(now) sorted by key.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::multimap;
using std::pair;

struct Entry {
    string value;
    long long expireAt;   // first time the entry is no longer valid; LLONG_MAX = never expires
};

class TTLKVStore {
public:
    // ttl <= 0 means no expiry; overwrites value and expiry, T: O(log n), S: O(1)
    void set(const string& key, const string& value, long long now, long long ttl) {
        long long expireAt = ttl > 0 ? now + ttl : LLONG_MAX;
        store[key] = {value, expireAt};
        if (expireAt != LLONG_MAX) { byExpiry.insert({expireAt, key}); }   // old pair for key stays, skipped in purge
    }

    // lazy removal of this key only, T: O(log n), S: O(1)
    string get(const string& key, long long now) {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }
        if (it->second.expireAt <= now) {
            store.erase(it);
            return "";
        }
        return it->second.value;
    }

    // live key only, keeps its expiry, T: O(log n), S: O(1)
    bool update(const string& key, const string& value, long long now) {
        auto it = store.find(key);
        if (it == store.end()) { return false; }
        if (it->second.expireAt <= now) {
            store.erase(it);
            return false;
        }
        it->second.value = value;
        return true;
    }

    // true iff a live key was removed, T: O(log n), S: O(1)
    bool remove(const string& key, long long now) {
        auto it = store.find(key);
        if (it == store.end()) { return false; }
        bool live = now < it->second.expireAt;
        store.erase(it);   // its byExpiry pair goes stale, skipped in purge
        return live;
    }

    // scan and filter (map is already key-sorted), T: O(n), S: O(k)
    vector<pair<string, string>> listValid(long long now) {
        vector<pair<string, string>> out;
        for (const auto& [key, e] : store) {
            if (now < e.expireAt) { out.push_back({key, e.value}); }
        }
        return out;
    }

    // purge first, then every stored entry is live, T: O(k) + purge, S: O(k)
    vector<pair<string, string>> listValid2(long long now) {
        purgeExpired(now);
        vector<pair<string, string>> out;
        for (const auto& [key, e] : store) { out.push_back({key, e.value}); }
        return out;
    }

    // pop earliest expiries from the index, T: O((x + s) log m) amortized O(log m) per set, S: O(1)
    void purgeExpired(long long now) {
        while (!byExpiry.empty() && byExpiry.begin()->first <= now) {
            auto it = store.find(byExpiry.begin()->second);
            // the key may have been re-set later; trust the store's expiry, not the index pair
            if (it != store.end() && it->second.expireAt <= now) { store.erase(it); }
            byExpiry.erase(byExpiry.begin());
        }
    }

    // physical entries, expired-but-unpurged included (test hook), T: O(1)
    int storedCount() { return store.size(); }

private:
    map<string, Entry> store;              // key -> value and expiry, sorted by key
    multimap<long long, string> byExpiry;  // expireAt -> key, may hold stale pairs
};

bool check(const string& got, const string& expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got \"" << got << "\", expected \"" << expected << "\"\n";
    return false;
}

bool check(int got, int expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

// listValid first (read-only), then listValid2 (purges)
bool checkList(TTLKVStore& kv, long long now, const vector<pair<string, string>>& expected, const string& label) {
    vector<pair<string, string>> got = kv.listValid(now);
    vector<pair<string, string>> got2 = kv.listValid2(now);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << ": sizes " << got.size() << " / " << got2.size()
         << ", expected " << expected.size() << "\n";
    return false;
}

bool testCrudNoExpiry() {
    bool ok = true;
    TTLKVStore kv;
    long long far = 1000000000000LL;   // far future, still fits now + ttl
    ok &= check(kv.get("a", 0), "", "get on empty store");
    ok &= checkList(kv, 0, {}, "list on empty store");
    kv.set("a", "1", 0, 0);   // ttl 0 = never expires
    ok &= check(kv.update("a", "2", 5), true, "update live key");
    ok &= check(kv.get("a", far), "2", "ttl 0 never expires, update applied");
    ok &= check(kv.update("zz", "x", far), false, "update missing key does not create");
    ok &= check(kv.get("zz", far), "", "zz still missing");
    ok &= check(kv.remove("a", far), true, "remove live key");
    ok &= check(kv.remove("a", far), false, "remove twice");
    ok &= check(kv.get("a", far), "", "get after remove");
    return ok;
}

bool testExpiryBoundary() {
    bool ok = true;
    TTLKVStore kv;
    kv.set("a", "1", 0, 5);   // expireAt 5
    ok &= check(kv.get("a", 4), "1", "now 4 < expireAt 5 valid");
    ok &= check(kv.get("a", 5), "", "now == expireAt expired");
    ok &= check(kv.storedCount(), 0, "get lazily erased a");
    ok &= check(kv.update("a", "2", 5), false, "update on expired key fails");
    kv.set("b", "1", 10, 3);  // expireAt 13
    ok &= check(kv.remove("b", 13), false, "remove expired key reports false");
    ok &= check(kv.get("b", 13), "", "b gone");
    return ok;
}

bool testUpdateKeepsExpiry() {
    bool ok = true;
    TTLKVStore kv;
    kv.set("a", "1", 0, 10);  // expireAt 10
    ok &= check(kv.update("a", "2", 5), true, "update at 5");
    ok &= check(kv.get("a", 9), "2", "new value before old expiry");
    ok &= check(kv.get("a", 10), "", "update did not renew ttl");
    return ok;
}

// adversarial for the lazy index: stale (expireAt, key) pairs must not delete a newer write
bool testResetWithNewExpiry() {
    bool ok = true;
    TTLKVStore kv;
    kv.set("k", "v1", 0, 5);    // index (5,k)
    kv.set("k", "v2", 3, 20);   // expireAt 23; (5,k) now stale
    kv.purgeExpired(6);
    ok &= check(kv.get("k", 6), "v2", "stale pair (5,k) skipped");

    kv.set("j", "a", 6, 100);   // index (106,j)
    kv.set("j", "b", 7, 2);     // shorter: expireAt 9
    kv.purgeExpired(9);
    ok &= check(kv.storedCount(), 1, "shorter re-set purged at 9, only k left");

    kv.set("m", "x", 9, 5);     // index (14,m)
    ok &= check(kv.remove("m", 10), true, "remove m");
    kv.set("m", "y", 10, 10);   // expireAt 20
    kv.set("p", "z", 10, 0);    // no expiry, never indexed
    kv.purgeExpired(14);        // pops stale (14,m)
    ok &= check(kv.get("m", 14), "y", "delete then re-set survives old pair");
    ok &= check(kv.storedCount(), 3, "k, m, p stored");
    ok &= checkList(kv, 14, {{"k", "v2"}, {"m", "y"}, {"p", "z"}}, "list at 14");

    kv.set("k", "v3", 15, -1);  // negative ttl = no expiry; (23,k) now stale
    kv.purgeExpired(200);       // m (20) purged; stale (23,k) and (106,j) skipped
    ok &= check(kv.storedCount(), 2, "only k and p remain");
    ok &= checkList(kv, 200, {{"k", "v3"}, {"p", "z"}}, "list at 200");
    return ok;
}

bool testListValidSorted() {
    bool ok = true;
    TTLKVStore kv;
    kv.set("pear", "3", 0, 10);    // expireAt 10
    kv.set("apple", "1", 0, 0);    // never
    kv.set("fig", "2", 2, 5);      // expireAt 7
    kv.set("banana", "4", 3, 20);  // expireAt 23
    ok &= checkList(kv, 6, {{"apple", "1"}, {"banana", "4"}, {"fig", "2"}, {"pear", "3"}}, "all valid at 6");
    ok &= checkList(kv, 7, {{"apple", "1"}, {"banana", "4"}, {"pear", "3"}}, "fig expires at 7");
    ok &= check(kv.storedCount(), 3, "purge freed fig");
    ok &= checkList(kv, 23, {{"apple", "1"}}, "only no-expiry key at 23");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testCrudNoExpiry();         // CRUD, ttl 0 sentinel, empty store, update never creates
    ok &= testExpiryBoundary();       // now == expireAt is expired, lazy get erase, remove on expired
    ok &= testUpdateKeepsExpiry();    // update keeps expiry contract
    ok &= testResetWithNewExpiry();   // stale index pairs: longer, shorter, delete+re-set, no-expiry re-set
    ok &= testListValidSorted();      // key order, both list versions agree, purge frees memory
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
