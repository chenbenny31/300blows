// B06 · LC 635 / 347 prototypes: record(ts, key) keeps the m latest records; countDistinctBefore(t) = distinct keys with ts < t. Follow-up: mostFrequentKey().
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::multimap;
using std::unordered_map;
using std::unordered_set;

class EventStore {
public:
    EventStore(int m) : m(m) {}

    // time-ordered multimap, evict the oldest when over m, T: O(logm), S: O(1) extra
    void record(int timestamp, const string& key) {
        kept.insert({timestamp, key});   // goes after records with an equal timestamp
        freq[key]++;
        if ((int)kept.size() > m) {
            auto oldest = kept.begin();  // smallest ts, earliest recorded among ties; may be the new one
            if (--freq[oldest->second] == 0) { freq.erase(oldest->second); }
            kept.erase(oldest);
        }
    }

    // scan in time order until ts >= t, T: O(W), S: O(W), W = kept records before t
    int countDistinctBefore(int t) {
        unordered_set<string> keys;      // distinct keys with ts < t
        for (auto it = kept.begin(); it != kept.end() && it->first < t; ++it) { keys.insert(it->second); }
        return keys.size();
    }

    // follow-up: scan key counts, ties by smaller key, "" when empty, T: O(U), S: O(1)
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
    int m;                               // max records kept
    multimap<int, string> kept;          // timestamp -> key, one entry per kept record, oldest first
    unordered_map<string, int> freq;     // key -> kept records with that key (only counts > 0)
};

void check(int got, int expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << " " << got << " expect " << expect << "\n";
}

void check(const string& got, const string& expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << " \"" << got << "\" expect \"" << expect << "\"\n";
}

int main() {
    EventStore s(3);
    s.record(1, "a");
    s.record(2, "b");
    s.record(3, "a");
    check(s.countDistinctBefore(3), 2, "before 3: a@1 b@2");
    check(s.countDistinctBefore(2), 1, "strict < t");
    check(s.mostFrequentKey(), "a", "a twice");
    s.record(4, "b");                    // evicts a@1
    check(s.countDistinctBefore(3), 1, "after evict: only b@2");
    check(s.mostFrequentKey(), "b", "evict updates counts");
    s.record(0, "c");                    // older than all kept, dropped at once
    check(s.countDistinctBefore(1), 0, "late record not kept");

    EventStore t(2);                     // timestamp ties evict the earliest recorded
    t.record(5, "z");
    t.record(5, "y");
    t.record(5, "x");
    check(t.mostFrequentKey(), "x", "tie evicted z, not smallest key");

    EventStore e(0);                     // m = 0 keeps nothing
    e.record(1, "a");
    check(e.countDistinctBefore(5), 0, "m = 0 count");
    check(e.mostFrequentKey(), "", "m = 0 most frequent");
}
