// events (timestamp, key); keep only the m records with the latest timestamps (Q10, J14594)
// countDistinctBefore(t): distinct keys among kept records with timestamp strictly < t
// follow-up topKey(): most frequent key among kept records

// m = number of records, or number of distinct timestamps?
// "before": strictly < t, or <= t?
// records can arrive out of order?
// same timestamp with several keys? which one is evicted on a timestamp tie?
// top key: ties broken how? single key or top K? empty store returns?

#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>
#include <iostream>

using std::string;
using std::pair;

// T: record O(log m), countDistinctBefore O(W), W: kept records before t, topKey O(1)
// S: O(m)

class EventStore {
private:
    int m;
    std::multiset<pair<long long, string>> kept; // (timestamp, key), oldest first, at most m
    std::unordered_map<string, int> freq; // key -> kept records with that key
    std::set<pair<int, string>> byFreq; // (-count, key): most frequent first, ties by key

public:
    EventStore(int capacity) : m(capacity) {}

    void record(long long timestamp, const string& key) {
        kept.insert({timestamp, key});
        bump(key, +1);
        if (static_cast<int>(kept.size()) > m) {
            auto oldest = kept.begin(); // smallest timestamp, may be the record just added
            bump(oldest->second, -1); // use its key before the node is erased
            kept.erase(oldest);
        }
    }

    int countDistinctBefore(long long t) {
        std::unordered_set<string> keys; // distinct keys seen with timestamp < t
        for (auto it = kept.begin(); it != kept.end() && it->first < t; ++it) {
            keys.insert(it->second);
        }
        return static_cast<int>(keys.size());
    }

    string topKey() {
        return byFreq.empty() ? "" : byFreq.begin()->second;
    }

private:
    void bump(const string& key, int delta) {
        auto it = freq.find(key);
        int old = (it == freq.end()) ? 0 : it->second;
        if (old > 0) {
            byFreq.erase({-old, key}); // remove the stale ranking entry first
        }
        int now = old + delta;
        if (now == 0) {
            freq.erase(key);
        } else {
            freq[key] = now;
            byFreq.insert({-now, key});
        }
    }
};

int main() {
    EventStore s(3);
    s.record(1, "a");
    s.record(2, "b");
    s.record(3, "a");
    assert(s.countDistinctBefore(3) == 2); // target: a@1, b@2
    assert(s.countDistinctBefore(2) == 1); // edge: strictly < 2 excludes b@2
    assert(s.topKey() == "a"); // follow-up target

    s.record(4, "b"); // evicts a@1
    assert(s.countDistinctBefore(5) == 2);
    assert(s.topKey() == "b"); // b: 2, a: 1

    s.record(0, "c"); // fault: late record older than all kept, evicted at once
    assert(s.countDistinctBefore(5) == 2);
    assert(s.topKey() == "b");

    EventStore empty(2);
    assert(empty.countDistinctBefore(5) == 0); // null: nothing recorded
    assert(empty.topKey() == "");

    std::cout << "all tests passed\n";
    return 0;
}
