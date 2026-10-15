// B24 · LC 981 follow-up: set(key, value, ts) with ts in any order; get(key, t) = value with the largest ts <= t, "" if none; same ts overwrites.
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <iterator>
#include <climits>
using std::cout;
using std::string;
using std::map;
using std::unordered_map;

class TimeMap {
public:
    // per-key ordered map, T: O(logv), S: O(1) extra
    void set(const string& key, const string& value, int timestamp) {
        store[key][timestamp] = value;   // same timestamp overwrites
    }

    // upper_bound then step back, T: O(logv), S: O(1)
    string get(const string& key, int t) {
        auto it = store.find(key);       // find, not [], so an unknown key inserts nothing
        if (it == store.end()) { return ""; }
        auto pos = it->second.upper_bound(t);            // first timestamp > t
        if (pos == it->second.begin()) { return ""; }    // every version is newer than t
        return std::prev(pos)->second;
    }

private:
    unordered_map<string, map<int, string>> store;   // key -> timestamp -> value
};

void check(const string& got, const string& expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << " \"" << got << "\" expect \"" << expect << "\"\n";
}

int main() {
    TimeMap tm;                          // LC 981 example
    tm.set("foo", "bar", 1);
    check(tm.get("foo", 3), "bar", "ex get@3");
    tm.set("foo", "bar2", 4);
    check(tm.get("foo", 4), "bar2", "ex get@4 exact");

    TimeMap o;                           // out-of-order inserts
    o.set("k", "late", 10);
    o.set("k", "early", 5);              // arrives after a newer timestamp
    check(o.get("k", 7), "early", "ooo get@7");
    check(o.get("k", 12), "late", "ooo get@12");
    check(o.get("k", 4), "", "before all versions");
    o.set("k", "fixed", 5);              // same timestamp overwrites
    check(o.get("k", 6), "fixed", "overwrite");
    check(o.get("none", INT_MAX), "", "unknown key");
}
