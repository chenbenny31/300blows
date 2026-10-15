// B06: record(timestamp, key) keeps only the m records with the latest timestamps
// (ties: the earliest recorded goes first); countDistinctBefore(t) = distinct keys
// among kept records with timestamp < t. Follow-up: mostFrequentKey().
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
    int m;                              // max records kept
    multimap<long long, string> kept;   // timestamp -> key per kept record, oldest first

public:
    EventStore(int cap) : m(cap) {}

    // Insert, then drop the oldest if over m. T: O(log m).
    void record(long long timestamp, const string& key) {
        // inserted after equal timestamps, so begin() is the earliest recorded tie
        kept.insert({timestamp, key});
        // may drop the new record itself if it is older than everything kept
        if ((int)kept.size() > m) { kept.erase(kept.begin()); }
    }

    // Scan in time order until timestamp >= t. T: O(m), S: O(m).
    int countDistinctBefore(long long t) {
        unordered_set<string> keys;   // distinct keys with timestamp < t
        for (auto it = kept.begin(); it != kept.end() && it->first < t; ++it) {
            keys.insert(it->second);
        }
        return keys.size();
    }

    // follow-up: count keys over kept records; ties -> smaller key, "" if empty. T: O(m).
    string mostFrequentKey() {
        unordered_map<string, int> freq;   // key -> kept records with that key
        for (const auto& [ts, key] : kept) { freq[key]++; }
        string best;
        int bestCount = 0;
        for (const auto& [key, c] : freq) {
            if (c > bestCount || (c == bestCount && key < best)) {
                best = key;
                bestCount = c;
            }
        }
        return best;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    EventStore s(3);
    s.record(1, "login");
    s.record(2, "click");
    s.record(3, "login");
    check(s.countDistinctBefore(2) == 1, "strict before");
    s.record(4, "click");
    check(s.countDistinctBefore(3) == 1, "evict oldest");
    s.record(0, "logout");
    check(s.countDistinctBefore(5) == 2, "late record dropped");
}
