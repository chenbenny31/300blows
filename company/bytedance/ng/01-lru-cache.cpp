// ByteDance (hack2hire) / LC 146: LRU cache with get(key) (-1 on miss) and
// put(key, value); when full, put evicts the least recently used key. Both O(1) average.
// Follow-up: per-entry TTL, put(key, value, ttl, now) / get(key, now); ttl 0 = never.
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

    // update in place, or add at the front and evict the LRU if over cap.
    // T: O(1) average.
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
        // with cap 0 this drops the node just added
        if ((int)nodes.size() > cap) {
            Node* lru = tail.prev;
            remove(lru);
            nodes.erase(lru->key);
            delete lru;
        }
    }
};

// follow-up: same DLL + map; each node also stores its expiry time.
class LRUCacheTTL {
    struct Node {
        int key, val;
        long long expireAt;   // absolute expiry time; 0 = never expires
        Node* prev;
        Node* next;
    };
    int cap;                          // max items kept
    Node head{};                      // sentinel; head.next = most recently used
    Node tail{};                      // sentinel; tail.prev = least recently used
    unordered_map<int, Node*> nodes;  // key -> its node in the list

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

    // unlink, forget and free
    void drop(Node* node) {
        remove(node);
        nodes.erase(node->key);
        delete node;
    }

    bool expired(Node* node, long long now) {
        return node->expireAt != 0 && node->expireAt <= now;
    }

public:
    LRUCacheTTL(int capacity) : cap(capacity) {
        head.next = &tail;
        tail.prev = &head;
    }

    ~LRUCacheTTL() {
        Node* cur = head.next;
        while (cur != &tail) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    LRUCacheTTL(const LRUCacheTTL&) = delete; // copy construction
    LRUCacheTTL& operator=(const LRUCacheTTL&) = delete; // copy assignment

    // lazy expiry: an expired hit is dropped and reported as a miss. T: O(1) average.
    int get(int key, long long now) {
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        Node* node = it->second;
        if (expired(node, now)) {
            drop(node);
            return -1;
        }
        remove(node);
        pushFront(node);
        return node->val;
    }

    // when full, purge expired entries first, then evict the LRU only if still over cap.
    // T: O(1) average, O(n) when the purge scan runs.
    void put(int key, int value, int ttl, long long now) {
        long long expireAt = ttl == 0 ? 0 : now + ttl;
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->val = value;
            it->second->expireAt = expireAt;
            remove(it->second);
            pushFront(it->second);
            return;
        }
        if ((int)nodes.size() >= cap) {
            Node* cur = tail.prev;
            while (cur != &head) {
                Node* prv = cur->prev;   // saved: drop frees cur
                if (expired(cur, now)) { drop(cur); }
                cur = prv;
            }
        }
        Node* node = new Node{key, value, expireAt, nullptr, nullptr};
        nodes[key] = node;
        pushFront(node);
        if ((int)nodes.size() > cap) { drop(tail.prev); }
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
    check(c.get(2) == -1, "evict 2");
    c.put(4, 4);
    check(c.get(1) == -1, "evict 1");
    check(c.get(3) == 3 && c.get(4) == 4, "keep 3 4");
}
