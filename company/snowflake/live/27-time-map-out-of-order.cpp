// B24 (LC 981 follow-up): set(key, value, timestamp) with timestamps in any order; get(key, t) returns
// the value with the largest timestamp <= t, or "" if none; the same (key, timestamp) overwrites.

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
using std::unordered_map;
using std::map;
using std::pair;

class TimeMap {
public:
    // per-key ordered map, T: O(logv), S: O(1) extra
    void set(const string& key, const string& value, int timestamp) {
        store[key][timestamp] = value;   // same timestamp overwrites
    }

    // upper_bound then step back, T: O(logv), S: O(1)
    string get(const string& key, int t) {
        auto it = store.find(key);
        if (it == store.end()) { return ""; }
        auto pos = it->second.upper_bound(t);   // first timestamp > t
        if (pos == it->second.begin()) { return ""; }   // every version is newer than t
        return std::prev(pos)->second;
    }

    // per-key sorted vector, insert at lower_bound, T: O(v) for the shift, S: O(1) extra
    void set2(const string& key, const string& value, int timestamp) {
        vector<pair<int, string>>& versions = sorted[key];
        auto pos = std::lower_bound(versions.begin(), versions.end(), timestamp,
                                    [](const pair<int, string>& p, int ts) { return p.first < ts; });
        if (pos != versions.end() && pos->first == timestamp) {
            pos->second = value;
            return;
        }
        versions.insert(pos, {timestamp, value});
    }

    // binary search on the sorted vector, T: O(logv), S: O(1)
    string get2(const string& key, int t) {
        auto it = sorted.find(key);
        if (it == sorted.end()) { return ""; }
        const vector<pair<int, string>>& versions = it->second;
        auto pos = std::upper_bound(versions.begin(), versions.end(), t,
                                    [](int ts, const pair<int, string>& p) { return ts < p.first; });
        if (pos == versions.begin()) { return ""; }
        return std::prev(pos)->second;
    }

private:
    unordered_map<string, map<int, string>> store;              // key -> timestamp -> value
    unordered_map<string, vector<pair<int, string>>> sorted;    // key -> (timestamp, value) sorted by timestamp
};

bool check(const string& got, const string& expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got \"" << got << "\", expected \"" << expected << "\"\n";
    return false;
}

// applies one set to both versions
void put(TimeMap& tm, const string& key, const string& value, int timestamp) {
    tm.set(key, value, timestamp);
    tm.set2(key, value, timestamp);
}

// checks get and get2 against the same expected value
bool expect(TimeMap& tm, const string& key, int t, const string& expected, const string& label) {
    bool ok = check(tm.get(key, t), expected, label);
    ok &= check(tm.get2(key, t), expected, label + " (2)");
    return ok;
}

bool testLeetCodeExample() {
    bool ok = true;
    TimeMap tm;
    put(tm, "foo", "bar", 1);
    ok &= expect(tm, "foo", 1, "bar", "ex get@1");
    ok &= expect(tm, "foo", 3, "bar", "ex get@3");
    put(tm, "foo", "bar2", 4);
    ok &= expect(tm, "foo", 4, "bar2", "ex get@4 exact");
    ok &= expect(tm, "foo", 5, "bar2", "ex get@5");
    return ok;
}

bool testOutOfOrder() {
    bool ok = true;
    TimeMap tm;
    put(tm, "k", "late", 10);
    put(tm, "k", "early", 5);   // arrives after a newer timestamp
    ok &= expect(tm, "k", 7, "early", "ooo get@7");
    ok &= expect(tm, "k", 12, "late", "ooo get@12");
    ok &= expect(tm, "k", 4, "", "ooo get@4 before all");
    put(tm, "k", "mid", 7);     // lands between two existing versions
    ok &= expect(tm, "k", 8, "mid", "ooo get@8");
    ok &= expect(tm, "k", 6, "early", "ooo get@6");
    put(tm, "k", "fixed", 5);   // same timestamp overwrites
    ok &= expect(tm, "k", 5, "fixed", "ooo get@5 overwritten");
    ok &= expect(tm, "k", 6, "fixed", "ooo get@6 after overwrite");
    return ok;
}

bool testEdges() {
    bool ok = true;
    TimeMap tm;
    ok &= expect(tm, "none", 1, "", "unknown key");
    put(tm, "a", "zero", 0);
    put(tm, "b", "other", 3);
    ok &= expect(tm, "a", 0, "zero", "timestamp 0 exact");
    ok &= expect(tm, "a", INT_MAX, "zero", "t = INT_MAX");
    ok &= expect(tm, "b", 2, "", "key isolation, b before its first");
    ok &= expect(tm, "a", 3, "zero", "key isolation, a ignores b");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testLeetCodeExample();   // LC 981 example, in-order sets
    ok &= testOutOfOrder();        // adversarial: out-of-order inserts, exact hits, before-first, overwrite
    ok &= testEdges();             // unknown key, timestamp 0 / INT_MAX, keys isolated
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
