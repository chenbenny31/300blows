#include <cassert>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>

using std::multiset;
using std::pair;
using std::string;
using std::unordered_set;

class EventStore {
private:
    int cap;
    multiset<pair<long long, string>> data; // ordered records by (ts, key)

public:
    // T: O(1), S: O(1)
    explicit EventStore(int m) : cap(m) {}
    
    // T: O(log m), S: O(1)
    void record(long long timestamp, const string& key) {
        data.insert({timestamp, key});

        if (static_cast<int>(data.size()) > cap) { // remove oldest record
            data.erase(data.begin());
        }
    }

    // T: O(m) expect, S: O(m)
    int countDistinctBefore(long long t) const {
        unordered_set<string> keys;

        for (const auto& [timestamp, key] : data) {
            if (timestamp >= t) { break; }
            keys.insert(key);
        }

        return static_cast<int>(keys.size());
    }
};

int main() {
    EventStore es(3);
    assert(es.countDistinctBefore(10) == 0);
    es.record(1, "a");
    es.record(2, "b");
    es.record(2, "b"); // dup rec

    assert(es.countDistinctBefore(2) == 1); // strict boundary
    assert(es.countDistinctBefore(3) == 2); // dup keys

    es.record(3, "c"); // evict 1-"a"
    assert(es.countDistinctBefore(3) == 1); // only b qualifies

    // es with cap==0
}
