// B24 · LC 981 variant: set(key, value, timestamp) with timestamps in any order;
// get(key, t) = value with the largest timestamp <= t, or "" if none.
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
using std::cout;
using std::string;
using std::map;
using std::unordered_map;

class TimeMap {
    unordered_map<string, map<int, string>> store;   // key -> timestamp -> value, by time

public:
    // map keeps versions sorted however they arrive. T: O(log v), v = versions of key.
    void set(const string& key, const string& value, int timestamp) {
        store[key][timestamp] = value;   // same timestamp overwrites
    }

    // First version > t, then step back one. T: O(log v).
    string get(const string& key, int t) {
        auto it = store.find(key);   // find, not [], so an unknown key inserts nothing
        if (it == store.end()) { return ""; }
        auto pos = it->second.upper_bound(t);
        if (pos == it->second.begin()) { return ""; }   // every version is newer than t
        --pos;                                          // last timestamp <= t
        return pos->second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TimeMap tm;
    tm.set("price", "120", 10);
    tm.set("price", "100", 5);
    check(tm.get("price", 7) == "100", "late insert");
    check(tm.get("price", 10) == "120", "exact hit");
    check(tm.get("price", 4) == "", "before first");
    tm.set("price", "110", 5);
    check(tm.get("price", 9) == "110", "overwrite");
}
