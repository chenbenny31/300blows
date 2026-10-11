// LRU cache: get(key), put(key, value), resize(newCapacity) (Q33, LC146 + resize)
// a hit or a write makes the key most recent; over capacity evicts the least recent
// shrinking evicts repeatedly until it fits, growing keeps everything

// get miss returns -1? does a miss touch the order?
// put on an existing key: update value and recency, no extra space?
// capacity >= 1? if 0 allowed, what do get/put do?
// resize to the same capacity: no-op?
// thread-safe?

#include <list>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <iostream>

using std::pair;

// T: get/put O(1) average, resize O(e), e: entries evicted
// S: O(capacity)

class LRUCache {
private:
    int cap;
    std::list<pair<int, int>> order; // (key, value), front = most recent, back = least recent
    std::unordered_map<int, std::list<pair<int, int>>::iterator> pos; // key -> its node in order

public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        auto it = pos.find(key); // find, not [], a miss must not insert
        if (it == pos.end()) {
            return -1;
        }
        order.splice(order.begin(), order, it->second); // move node to front, iterator stays valid
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = pos.find(key);
        if (it != pos.end()) {
            it->second->second = value;
            order.splice(order.begin(), order, it->second);
            return;
        }
        order.push_front({key, value});
        pos[key] = order.begin();
        evict();
    }

    void resize(int newCapacity) {
        cap = newCapacity;
        evict();
    }

private:
    void evict() {
        while (static_cast<int>(order.size()) > cap) {
            pos.erase(order.back().first); // erase the map entry before its node dies
            order.pop_back();
        }
    }
};

int main() {
    LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    assert(c.get(1) == 1); // target: hit, 1 becomes most recent
    c.put(3, 3); // evicts 2
    assert(c.get(2) == -1); // fault: miss
    c.put(1, 10); // edge: update refreshes 1, takes no extra slot
    c.put(4, 4); // evicts 3, not 1
    assert(c.get(3) == -1);
    assert(c.get(1) == 10);

    LRUCache r(2);
    r.put(1, 1);
    r.put(2, 2);
    r.get(1);
    r.resize(1); // shrink keeps the most recent
    assert(r.get(2) == -1);
    assert(r.get(1) == 1);
    r.resize(3); // grow keeps content
    r.put(5, 5);
    r.put(6, 6);
    assert(r.get(1) == 1);
    r.resize(1); // edge: shrink by two in one call
    assert(r.get(1) == 1);
    assert(r.get(6) == -1);
    assert(r.get(5) == -1);

    std::cout << "all tests passed\n";
    return 0;
}
