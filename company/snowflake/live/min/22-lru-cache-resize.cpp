// B10 · LC 146 + resize: LRU cache get / put / resize(newCap); miss -> -1, shrinking evicts LRU items until size <= cap, cap 0 caches nothing.
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
using std::cout;
using std::string;
using std::list;
using std::pair;
using std::unordered_map;

class LRUCache {
public:
    LRUCache(int capacity) : cap(capacity) {}

    // hash lookup + splice to front, T: O(1) avg, S: O(1)
    int get(int key) {
        auto it = pos.find(key);   // find, not [], so a miss inserts nothing
        if (it == pos.end()) { return -1; }
        order.splice(order.begin(), order, it->second);
        return it->second->second;
    }

    // update in place or push front, then evict, T: O(1) avg, S: O(1) extra
    void put(int key, int value) {
        auto it = pos.find(key);
        if (it != pos.end()) {
            it->second->second = value;
            order.splice(order.begin(), order, it->second);
            return;
        }
        order.push_front({key, value});
        pos[key] = order.begin();
        evict();                   // with cap 0 this removes the item just added
    }

    // evict from the back until it fits, T: O(e), e = items evicted, S: O(1)
    void resize(int newCap) {
        cap = newCap;
        evict();
    }

    int size() { return order.size(); }

private:
    int cap;                                                  // max items kept
    list<pair<int, int>> order;                               // (key, value), front = most recent
    unordered_map<int, list<pair<int, int>>::iterator> pos;   // key -> its node in order

    void evict() {
        while ((int)order.size() > cap) {
            pos.erase(order.back().first);   // erase map entry before its node is destroyed
            order.pop_back();
        }
    }
};

void check(int got, int expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << " " << got << " expect " << expect << "\n";
}

int main() {
    LRUCache c(2);                       // LC 146 example
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1), 1, "hit");
    c.put(3, 3);                         // evicts 2
    check(c.get(2), -1, "2 evicted");
    c.put(1, 10);                        // update refreshes 1, no extra slot
    c.put(4, 4);                         // evicts 3, not 1
    check(c.get(3), -1, "3 evicted");
    check(c.get(1), 10, "updated value");

    LRUCache r(3);                       // shrink by two in one call
    r.put(1, 1);
    r.put(2, 2);
    r.put(3, 3);
    r.get(1);                            // order 1, 3, 2
    r.resize(1);                         // evicts 2 then 3
    check(r.size(), 1, "shrink size");
    check(r.get(3), -1, "shrink evicts 3");
    check(r.get(1), 1, "shrink keeps most recent");
    r.resize(0);                         // clears everything
    check(r.size(), 0, "resize 0 clears");

    LRUCache z(0);                       // capacity 0 at construction
    z.put(1, 1);
    check(z.get(1), -1, "cap 0 miss");
}
