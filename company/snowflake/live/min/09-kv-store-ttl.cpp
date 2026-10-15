// B04: in-memory KV store: set / get / update / remove, then TTL expiry,
// then listValid(now) = all live (key, value) pairs sorted by key.
// Follow-up: purgeExpired(now) removes expired keys without scanning everything.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <climits>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::multimap;

class TTLStore {
public:
    // ttl <= 0 means never expires. T: O(log n).
    void set(const string& key, const string& value, long long now, long long ttl) {
        long long expireAt = ttl > 0 ? now + ttl : LLONG_MAX;
        store[key] = {value, expireAt};
        // follow-up index; an older pair for this key goes stale, purge skips it
        if (ttl > 0) { byExpiry.insert({expireAt, key}); }
    }

    // Lazy expiry: an expired key is erased when touched. T: O(log n).
    string get(const string& key, long long now) {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }
        if (it->second.expireAt <= now) {
            store.erase(it);
            return "";
        }
        return it->second.value;
    }

    // Only a live key; keeps its expiry. T: O(log n).
    bool update(const string& key, const string& value, long long now) {
        if (get(key, now).empty()) { return false; }
        store[key].value = value;
        return true;
    }

    // True if a live key was removed. T: O(log n).
    bool remove(const string& key, long long now) {
        bool live = !get(key, now).empty();
        store.erase(key);
        return live;
    }

    // map is already sorted by key. T: O(n).
    vector<pair<string, string>> listValid(long long now) {
        vector<pair<string, string>> out;
        for (const auto& [key, e] : store) {
            if (now < e.expireAt) { out.push_back({key, e.value}); }
        }
        return out;
    }

    // follow-up: pop the earliest expiries only. T: O(k log n) for k popped entries.
    void purgeExpired(long long now) {
        while (!byExpiry.empty() && byExpiry.begin()->first <= now) {
            auto it = store.find(byExpiry.begin()->second);
            // the key may have been re-set since: trust the store's expiry, not the index
            if (it != store.end() && it->second.expireAt <= now) { store.erase(it); }
            byExpiry.erase(byExpiry.begin());
        }
    }

private:
    struct Entry {
        string value;
        long long expireAt;   // first time the entry is invalid; LLONG_MAX = never
    };
    map<string, Entry> store;              // key -> value and expiry, sorted by key
    multimap<long long, string> byExpiry;  // expireAt -> key; may hold stale pairs
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TTLStore kv;
    kv.set("session", "abc", 0, 10);
    kv.set("theme", "dark", 0, 0);
    check(kv.get("session", 5) == "abc", "get live");
    check(kv.update("session", "xyz", 6) && kv.get("session", 9) == "xyz", "update");
    check(kv.get("session", 10) == "", "expired at boundary");
    check(kv.listValid(10) == vector<pair<string, string>>{{"theme", "dark"}}, "list");
    check(kv.remove("theme", 10) && kv.listValid(10).empty(), "remove");
}
