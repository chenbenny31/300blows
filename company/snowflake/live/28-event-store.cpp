// B06 (LC 635 / 347 partial prototypes): record(timestamp, key) keeps only the m records with the latest timestamps;
// countDistinctBefore(t) = distinct keys among kept records with timestamp < t; follow-up mostFrequentKey().

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
using std::unordered_set;
using std::multimap;

class EventStore {
public:
    EventStore(int m) : m(m) {}

    // insert into a time-ordered multimap, evict the oldest when over m, T: O(logm), S: O(1) extra
    void record(int timestamp, const string& key) {
        kept.insert({timestamp, key});   // goes after records with an equal timestamp
        freq[key]++;
        if ((int)kept.size() > m) {
            auto oldest = kept.begin();   // smallest timestamp, earliest recorded among ties; may be the new one
            auto f = freq.find(oldest->second);
            f->second--;
            if (f->second == 0) { freq.erase(f); }
            kept.erase(oldest);
        }
    }

    // scan kept records in time order until timestamp >= t, T: O(W), S: O(W), W: kept records before t
    int countDistinctBefore(int t) {
        unordered_set<string> keys;   // distinct keys seen with timestamp < t
        for (auto it = kept.begin(); it != kept.end() && it->first < t; ++it) { keys.insert(it->second); }
        return keys.size();
    }

    // scan the key counts, ties by smaller key, "" when empty, T: O(U), S: O(1), U: distinct kept keys
    string mostFrequentKey() {
        string best = "";
        int bestCount = 0;
        for (const auto& [key, c] : freq) {
            if (c > bestCount || (c == bestCount && key < best)) {
                best = key;
                bestCount = c;
            }
        }
        return best;
    }

private:
    int m;                             // max records kept
    multimap<int, string> kept;        // timestamp -> key, one entry per kept record, oldest first
    unordered_map<string, int> freq;   // key -> kept records with that key (only counts > 0)
};

bool check(int got, int expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool check(const string& got, const string& expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got \"" << got << "\", expected \"" << expected << "\"\n";
    return false;
}

bool testBasicAndLate() {
    bool ok = true;
    EventStore s(3);
    s.record(1, "a");
    s.record(2, "b");
    s.record(3, "a");
    ok &= check(s.countDistinctBefore(3), 2, "before 3: a@1 b@2");
    ok &= check(s.countDistinctBefore(2), 1, "before 2 is strict");
    ok &= check(s.countDistinctBefore(1), 0, "before 1: nothing");
    ok &= check(s.countDistinctBefore(100), 2, "before 100: a, b");
    s.record(4, "b");   // evicts a@1
    ok &= check(s.countDistinctBefore(5), 2, "after evict before 5");
    ok &= check(s.countDistinctBefore(3), 1, "after evict before 3: only b@2");
    s.record(0, "c");   // older than every kept record, evicted at once
    ok &= check(s.countDistinctBefore(5), 2, "late c dropped");
    ok &= check(s.countDistinctBefore(1), 0, "late c not kept");
    return ok;
}

bool testMostFrequent() {
    bool ok = true;
    EventStore s(3);
    s.record(1, "a");
    s.record(2, "b");
    s.record(3, "a");
    ok &= check(s.mostFrequentKey(), "a", "a twice");
    s.record(4, "b");   // evicts a@1: a 1, b 2
    ok &= check(s.mostFrequentKey(), "b", "after evict b leads");
    s.record(0, "c");   // dropped immediately, must not stay counted
    ok &= check(s.mostFrequentKey(), "b", "late c not counted");
    return ok;
}

bool testTimestampTies() {
    bool ok = true;
    EventStore s(2);
    s.record(5, "z");
    s.record(5, "y");
    s.record(5, "x");   // three at time 5: evict the earliest recorded, z (not the smallest key x)
    ok &= check(s.countDistinctBefore(6), 2, "tie keeps two");
    ok &= check(s.mostFrequentKey(), "x", "tie evicted z, x and y left");
    EventStore t(2);
    t.record(5, "p");
    t.record(6, "q");
    t.record(5, "r");   // equals the oldest timestamp: p (recorded earlier) goes
    ok &= check(t.countDistinctBefore(6), 1, "r@5 kept");
    ok &= check(t.mostFrequentKey(), "q", "p evicted, q and r tie");
    return ok;
}

bool testDuplicatesAndEmpty() {
    bool ok = true;
    EventStore s(3);
    s.record(1, "a");
    s.record(1, "a");   // identical record counts separately
    s.record(2, "b");
    ok &= check(s.mostFrequentKey(), "a", "dup a counted twice");
    ok &= check(s.countDistinctBefore(3), 2, "dup distinct still 2");
    s.record(3, "b");
    s.record(4, "b");   // both a@1 copies evicted
    ok &= check(s.countDistinctBefore(5), 1, "only b left");
    ok &= check(s.countDistinctBefore(2), 0, "a@1 gone");
    ok &= check(s.mostFrequentKey(), "b", "b leads");
    EventStore e(2);
    ok &= check(e.countDistinctBefore(5), 0, "empty count");
    ok &= check(e.mostFrequentKey(), "", "empty most frequent");
    EventStore z(0);
    z.record(1, "a");   // m = 0 keeps nothing
    ok &= check(z.countDistinctBefore(5), 0, "m=0 count");
    ok &= check(z.mostFrequentKey(), "", "m=0 most frequent");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testBasicAndLate();        // given-style: strict < t, eviction, late record older than all
    ok &= testMostFrequent();        // follow-up: counts track eviction
    ok &= testTimestampTies();       // adversarial: tie on timestamp evicts the earliest recorded
    ok &= testDuplicatesAndEmpty();  // identical records, empty store, m = 0
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
