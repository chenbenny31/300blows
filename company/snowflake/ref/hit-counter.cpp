// hit(timestamp) records a hit; getHits(timestamp) returns hits in the past 300 seconds, window (t - 300, t]
// timestamps in seconds, calls arrive in non-decreasing time order (LC362)
// follow-up: huge number of hits per second -> fixed-size array indexed by timestamp % 300

// window (t - 300, t] or [t - 300, t]?
// timestamps non-decreasing across hit and getHits?
// many hits in the same second?
// counts beyond int?
// concurrent callers?

#include <queue>
#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

// 1: queue of every hit, T: hit O(1), getHits O(1) amortized, S: O(hits in window)
class HitCounter_1 {
private:
    std::queue<long long> hits; // timestamp of each hit, oldest at front

public:
    void hit(long long timestamp) {
        hits.push(timestamp);
    }

    long long getHits(long long timestamp) {
        while (!hits.empty() && hits.front() <= timestamp - 300) { // a hit at s counts iff s > t - 300
            hits.pop();
        }
        return static_cast<long long>(hits.size());
    }
};

// 2: fixed 300 slots, slot = timestamp % 300, T: hit O(1), getHits O(300), S: O(300)
class HitCounter_2 {
private:
    vector<long long> times = vector<long long>(300, -1); // slot -> second it holds, -1 = empty
    vector<long long> counts = vector<long long>(300, 0); // slot -> hits in that second

public:
    void hit(long long timestamp) {
        int i = static_cast<int>(timestamp % 300);
        if (times[i] != timestamp) { // slot still holds an older second: overwrite
            times[i] = timestamp;
            counts[i] = 0;
        }
        counts[i]++;
    }

    long long getHits(long long timestamp) {
        long long total = 0;
        for (int i = 0; i < 300; i++) {
            if (times[i] != -1 && timestamp - times[i] < 300) { // inside (t - 300, t]
                total += counts[i];
            }
        }
        return total;
    }
};

int main() {
    HitCounter_1 a;
    assert(a.getHits(1) == 0); // null: no hits yet
    a.hit(1);
    a.hit(2);
    a.hit(3);
    assert(a.getHits(4) == 3); // target: LC example
    a.hit(300);
    assert(a.getHits(300) == 4); // edge: hit at 1 still inside (0, 300]
    assert(a.getHits(301) == 3); // edge: hit at 1 drops out of (1, 301]

    HitCounter_2 b;
    assert(b.getHits(1) == 0); // null
    b.hit(1);
    b.hit(2);
    b.hit(3);
    assert(b.getHits(4) == 3); // target
    b.hit(300);
    assert(b.getHits(300) == 4); // edge: boundary
    assert(b.getHits(301) == 3);
    b.hit(301); // same slot as second 1: overwritten, not added
    assert(b.getHits(301) == 4); // fault: stale slot reuse

    std::cout << "all tests passed\n";
    return 0;
}
