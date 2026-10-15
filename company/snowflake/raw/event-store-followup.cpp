#include <cassert>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using std::multiset;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::unordered_set;

class EventStore {
private:
    int cap;
    multiset<pair<long long, string>> data; // multiset of (ts, key)
    unordered_map<string, int> freq;
    set<pair<int, string>> rank; // (-count, key)

    // T: O(logm), S: O(1)
    void changeCount(const string& key, int delta) {
        auto it = freq.find(key);
        int oldCount = it == freq.end() ? 0 : it->second;

        if (oldCount > 0) { rank.erase({-oldCount, key}); }

        int count = oldCount + delta;
        if (count == 0) {
            freq.erase(key);
        } else {
            freq[key] = count;
            rank.insert({-count, key});
        }
    }

public:
    // T: O(1), S: O(1)
    explicit EventStore(int m) : cap(m) {}

    // T: O(logm), S: O(1)
    void record(long long timestamp, const string& key) {
        data.insert({timestamp, key});
        changeCount(key, 1);

        if (static_cast<int>(data.size()) > cap) {
            auto oldest = data.begin();
            changeCount(oldest->second, -1);
            data.erase(oldest);
        }
    }

    // T: O(m), S: O(m)
    int countDistinctBefore(long long t) const {
        unordered_set<string> keys;
        for (const auto& [timestamp, key] : data) {
            if (timestamp >= t) { break; }
            keys.insert(key);
        }
        return static_cast<int>(keys.size());
    }

    // T: O(1), S: O(1)
    string topKey() const {
        return rank.empty() ? "" : rank.begin()->second;
    }
};

int main() {
    EventStore store(3);
    assert(store.topKey().empty());

    store.record(1, "b");
    store.record(2, "a");
    assert(store.topKey() == "a"); // equal count: alpha-order

    store.record(2, "a");
    assert(store.topKey() == "a");
    assert(store.countDistinctBefore(2) == 1); // strictly boundary

    store.record(3, "b");
    store.record(4, "b");
    assert(store.topKey() == "b");

    // store with cap==0
}
