#include <array>
#include <cassert>

using std::array;

class HitCounter {
private:
    array<long long, 300> times{};
    array<long long, 300> counts{};

public:
    // T: O(1), S: O(1)
    void hit(long long timestamp) {
        int slot = timestamp % 300;
        if (times[slot] != timestamp) {
            times[slot] = timestamp;
            counts[slot] = 0;
        }
        counts[slot]++;
    }

    // T: O(1), S: O(1)
    long long getHits(long long timestamp) const {
        long long total = 0;
        for (int i = 0; i < 300; i++) {
            if (timestamp - times[i] < 300) {
                total += counts[i];
            }
        }
        return total;
    }
};

int main() {
    HitCounter counter;
    assert(counter.getHits(1) == 0); // Unused buckets have zero counts.

    counter.hit(1);
    counter.hit(1);
    counter.hit(300);
    assert(counter.getHits(300) == 3);
    assert(counter.getHits(301) == 1); // Exact expiration.

    counter.hit(301); // Reuses second 1's bucket.
    assert(counter.getHits(301) == 2); // Reset stale count before incrementing.
    assert(counter.getHits(601) == 0); // Everything expired.
}
