#include <cassert>
#include <queue>

using std::queue;

class HitCounter {
private:
    queue<long long> hits;

    // T: O(k), S: O(1)
    void purge(long long timestamp) {
        while (!hits.empty() && hits.front() <= timestamp - 300) {
            hits.pop();
        }
    }

public:
    // T: O(1) + purge, S: O(1)
    void hit(long long timestamp) {
        purge(timestamp);
        hits.push(timestamp);
    }

    // T: O(1) + purge, S: O(1)
    long long getHits(long long timestamp) {
        purge(timestamp);
        return static_cast<long long>(hits.size());
    }
};

int main() {
    HitCounter counter;
    assert(counter.getHits(1) == 0);
    counter.hit(1);
    counter.hit(1);
    counter.hit(300);
    assert(counter.getHits(300) == 3);
    assert(counter.getHits(301) == 1);
    assert(counter.getHits(600) == 0);
}
