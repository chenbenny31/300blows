// B10 (LC 146 + resize): LRU cache with get, put, resize(newCap); a miss returns -1,
// shrinking evicts least recently used items until size <= capacity, capacity 0 caches nothing.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <list>

using std::cout;
using std::string;
using std::vector;
using std::list;
using std::pair;
using std::unordered_map;

class LRUCache {
public:
    LRUCache(int capacity) : cap(capacity) {}

    // hash lookup + splice to front, T: O(1) average, S: O(1)
    int get(int key) {
        auto it = pos.find(key);   // find, not [], so a miss inserts nothing
        if (it == pos.end()) { return -1; }
        order.splice(order.begin(), order, it->second);   // iterator stays valid after splice
        return it->second->second;
    }

    // update in place or push front, then evict, T: O(1) average, S: O(1) extra
    void put(int key, int value) {
        auto it = pos.find(key);
        if (it != pos.end()) {
            it->second->second = value;
            order.splice(order.begin(), order, it->second);
            return;
        }
        order.push_front({key, value});
        pos[key] = order.begin();
        evict();   // with cap 0 this removes the item just added
    }

    // set capacity, evict from the back until it fits, T: O(e) average, e: items evicted, S: O(1)
    void resize(int newCap) {
        cap = newCap;
        evict();
    }

    int size() { return order.size(); }

private:
    int cap;                                                // max items kept
    list<pair<int, int>> order;                             // (key, value), front = most recent
    unordered_map<int, list<pair<int, int>>::iterator> pos; // key -> its node in order

    void evict() {
        while ((int)order.size() > cap) {
            pos.erase(order.back().first);   // erase map entry before its node is destroyed
            order.pop_back();
        }
    }
};

bool check(int got, int expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool testLeetCodeExample() {
    bool ok = true;
    LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    ok &= check(c.get(1), 1, "ex get(1) hit");
    c.put(3, 3);   // evicts 2
    ok &= check(c.get(2), -1, "ex get(2) evicted");
    c.put(4, 4);   // evicts 1
    ok &= check(c.get(1), -1, "ex get(1) evicted");
    ok &= check(c.get(3), 3, "ex get(3)");
    ok &= check(c.get(4), 4, "ex get(4)");
    return ok;
}

bool testUpdateRefreshes() {
    bool ok = true;
    LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    c.put(1, 10);   // update: 1 becomes most recent, no extra slot
    ok &= check(c.size(), 2, "upd size stays 2");
    c.put(3, 3);    // evicts 2, not 1
    ok &= check(c.get(2), -1, "upd get(2) evicted");
    ok &= check(c.get(1), 10, "upd get(1) new value");
    ok &= check(c.get(42), -1, "upd miss on unknown key");
    ok &= check(c.size(), 2, "upd miss inserts nothing");
    return ok;
}

bool testResize() {
    bool ok = true;
    LRUCache c(3);
    c.put(1, 1);
    c.put(2, 2);
    c.put(3, 3);
    c.get(1);        // order now 1, 3, 2 (most -> least recent)
    c.resize(1);     // shrink by two in one call: evicts 2 then 3
    ok &= check(c.size(), 1, "shrink size 1");
    ok &= check(c.get(2), -1, "shrink 2 evicted");
    ok &= check(c.get(3), -1, "shrink 3 evicted");
    ok &= check(c.get(1), 1, "shrink keeps most recent");
    c.resize(3);     // grow keeps content
    c.put(5, 5);
    c.put(6, 6);
    ok &= check(c.size(), 3, "grow size 3");
    ok &= check(c.get(1), 1, "grow keeps 1");
    c.resize(3);     // same capacity: no-op
    ok &= check(c.size(), 3, "same-cap resize no-op");
    c.put(7, 7);     // evicts 5 (least recent after get(1))
    ok &= check(c.get(5), -1, "after grow evicts LRU 5");
    ok &= check(c.get(6), 6, "after grow keeps 6");
    return ok;
}

bool testZeroCapacity() {
    bool ok = true;
    LRUCache c(0);
    c.put(1, 1);     // stored and immediately evicted
    ok &= check(c.get(1), -1, "cap0 get miss");
    ok &= check(c.size(), 0, "cap0 size 0");
    LRUCache d(2);
    d.put(1, 1);
    d.put(2, 2);
    d.resize(0);     // clears everything
    ok &= check(d.size(), 0, "resize0 clears");
    ok &= check(d.get(1), -1, "resize0 get miss");
    d.resize(1);
    d.put(3, 3);
    ok &= check(d.get(3), 3, "regrow after 0 works");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testLeetCodeExample();   // LC 146 example
    ok &= testUpdateRefreshes();   // put on existing key refreshes recency, miss does not insert
    ok &= testResize();            // shrink evicts several LRU items at once, grow keeps data
    ok &= testZeroCapacity();      // capacity 0 at construction and via resize
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
