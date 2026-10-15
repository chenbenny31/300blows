// E01 (IC1/IC2) · LC 362: hit(timestamp) records a hit; getHits(timestamp) = hits in the
// past 300 seconds, i.e. in (timestamp - 300, timestamp]. Timestamps are non-decreasing.
// Follow-up: huge number of hits per second -> fixed memory via 300 circular buckets.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

class HitCounter {
    queue<int> hits;   // timestamps of hits still inside the window, oldest at front

    // a hit at s counts iff s > timestamp - 300
    void evict(int timestamp) {
        while (!hits.empty() && hits.front() <= timestamp - 300) { hits.pop(); }
    }

public:
    // T: O(1) amortized.
    void hit(int timestamp) {
        hits.push(timestamp);
        evict(timestamp);
    }

    // T: O(1) amortized; each hit is popped once.
    int getHits(int timestamp) {
        evict(timestamp);
        return hits.size();
    }
};

// follow-up: one bucket per second of the window, reused circularly.
// T: O(1) hit, O(300) getHits; S: O(300).
class HitCounterBuckets {
    vector<int> times;          // times[i] = the second bucket i currently counts
    vector<long long> counts;   // counts[i] = hits during second times[i]

public:
    HitCounterBuckets() : times(300, 0), counts(300, 0) {}

    void hit(int timestamp) {
        int i = timestamp % 300;
        // bucket still holds an older second: overwrite it, never add to it
        if (times[i] != timestamp) {
            times[i] = timestamp;
            counts[i] = 0;
        }
        counts[i]++;
    }

    long long getHits(int timestamp) {
        long long total = 0;
        for (int i = 0; i < 300; i++) {
            if (timestamp - times[i] < 300) { total += counts[i]; }
        }
        return total;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    HitCounter c;
    check(c.getHits(1) == 0, "no hits");
    c.hit(1);
    c.hit(2);
    c.hit(3);
    check(c.getHits(4) == 3, "lc example");
    c.hit(300);
    check(c.getHits(300) == 4, "edge 300");
    check(c.getHits(301) == 3, "first expires");
}
