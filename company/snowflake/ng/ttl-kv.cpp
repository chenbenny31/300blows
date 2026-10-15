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
private:
    struct Entry {
        string value;
        long long expireAt; // ttl + now
    };
    map<string, Entry> store; // key -> Entry
    multimap<long long, string> byExpiry; // sorted non-decre expireAt -> key

public:
    // T: O(logn), ttl == 0 means never expires
    bool set(const string& key, const string& value, long long now, long long ttl) {
        if (ttl < 0) { return false; }
        long long expireAt = ttl > 0 ? now + ttl : LLONG_MAX;
        store[key] = {value, expireAt};

        // follow-up index
        if (ttl > 0) { byExpiry.insert({expireAt, key}); }
        return true;
    }

    // lazy expiry, T: O(logn)
    string get(const string& key, long long now) {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }
        if (it->second.expireAt <= now) { // expireAt==now also expires
            store.erase(it);
            return "";
        }
        return it->second.value;
    }

    // only update live key, T: O(logn)
    bool update(const string& key, const string& value, long long now) {
        if (get(key, now).empty()) { return false; }
        store[key].value = value;
        return true;
    }

    // return true if live key is removed, T: O(logn)
    bool remove(const string& key, long long now) {
        bool live = !get(key, now).empty();
        store.erase(key);
        return live;
    }

    // map is sorted by key, T: O(n)
    vector<pair<string, string>> listValid(long long now) {
        vector<pair<string, string>> result;
        for (const auto& [key, entry] : store) {
            if (now < entry.expireAt) { result.push_back({key, entry.value}); }
        }
        return result;
    }

    // follow-up: pop the earliest expires only, T: O(klogn) for k popped entries
    void purge(long long now) {
        while (!byExpiry.empty() && byExpiry.begin()->first <= now) {
            auto it = store.find(byExpiry.begin()->second);

            if (it != store.end() && it->second.expireAt <= now) {
                store.erase(it);
            }
            byExpiry.erase(byExpiry.begin());
        }
    }
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
