// B10 · LC 146 + resize: LRU cache with get (-1 on miss), put, and resize(newCap);
// shrinking evicts least recently used items until size <= capacity.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

class LRUCache {
    struct Node {
        int key, val;
        Node* prev;
        Node* next;
    };
    int cap;                          // max items kept
    Node head{};                      // sentinel; head.next = most recently used
    Node tail{};                      // sentinel; tail.prev = least recently used
    unordered_map<int, Node*> nodes;  // key -> its node in the list

    // unlink only; the caller keeps or frees the node
    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void pushFront(Node* node) {
        node->prev = &head;
        node->next = head.next;
        head.next->prev = node;
        head.next = node;
    }

    // drop from the LRU end until the size fits. T: O(e), e = items evicted.
    void evict() {
        while ((int)nodes.size() > cap) {
            Node* lru = tail.prev;
            remove(lru);
            nodes.erase(lru->key);
            delete lru;
        }
    }

public:
    LRUCache(int capacity) : cap(capacity) {
        head.next = &tail;
        tail.prev = &head;
    }

    ~LRUCache() {
        Node* cur = head.next;
        while (cur != &tail) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    LRUCache(const LRUCache&) = delete; // copy construction
    LRUCache& operator=(const LRUCache&) = delete; // copy assignment

    // hash lookup + move to front. T: O(1) average.
    int get(int key) {
        auto it = nodes.find(key);   // find, not [], so a miss inserts nothing
        if (it == nodes.end()) { return -1; }
        remove(it->second);
        pushFront(it->second);
        return it->second->val;
    }

    // update in place, or add at the front then evict. T: O(1) average.
    void put(int key, int value) {
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->val = value;
            remove(it->second);
            pushFront(it->second);
            return;
        }
        Node* node = new Node{key, value, nullptr, nullptr};
        nodes[key] = node;
        pushFront(node);
        evict();   // with cap 0 this drops the node just added
    }

    // T: O(e), e = items evicted.
    void resize(int newCap) {
        cap = newCap;
        evict();
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1) == 1, "hit");
    c.put(3, 3);
    check(c.get(2) == -1, "evict LRU");
    c.put(1, 10);
    check(c.get(1) == 10, "update");
    c.resize(3);
    c.put(4, 4);
    c.resize(1);
    check(c.get(4) == 4 && c.get(1) == -1 && c.get(3) == -1, "shrink");
    LRUCache z(0);
    z.put(1, 1);
    check(z.get(1) == -1, "capacity 0");
}
