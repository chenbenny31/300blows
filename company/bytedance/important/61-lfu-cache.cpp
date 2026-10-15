// ByteDance (hack2hire) / LC 460: LFU cache with get (-1 on miss) and put. When full,
// evict the least frequently used key; ties -> the least recently used among them.
// get and put both count as a use.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

class LFUCache {
    struct Node {
        int key, val, freq;
        Node* prev;
        Node* next;
    };
    struct Bucket {
        Node head{};   // sentinel; head.next = most recently used
        Node tail{};   // sentinel; tail.prev = least recently used
    };
    int cap;                              // max items kept
    int minFreq = 0;                      // smallest freq with a non-empty bucket
    unordered_map<int, Node*> nodes;      // key -> its node
    // freq -> recency list of nodes with that freq; map elements never move in memory
    unordered_map<int, Bucket> buckets;

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // front of the bucket for node->freq; link the sentinels on first use
    void pushFront(Node* node) {
        Bucket& b = buckets[node->freq];
        if (!b.head.next) {
            b.head.next = &b.tail;
            b.tail.prev = &b.head;
        }
        node->prev = &b.head;
        node->next = b.head.next;
        b.head.next->prev = node;
        b.head.next = node;
    }

    // move node up one frequency. T: O(1) average.
    void touch(Node* node) {
        remove(node);
        Bucket& b = buckets[node->freq];
        if (node->freq == minFreq && b.head.next == &b.tail) { minFreq++; }
        node->freq++;
        pushFront(node);
    }

public:
    LFUCache(int capacity) : cap(capacity) {}

    ~LFUCache() {
        for (auto& [key, node] : nodes) { delete node; }
    }

    LFUCache(const LFUCache&) = delete; // copy construction
    LFUCache& operator=(const LFUCache&) = delete; // copy assignment

    // T: O(1) average.
    int get(int key) {
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        touch(it->second);
        return it->second->val;
    }

    // T: O(1) average.
    void put(int key, int value) {
        if (cap == 0) { return; }
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->val = value;
            touch(it->second);
            return;
        }
        if ((int)nodes.size() == cap) {
            // least recent node in the least frequent bucket
            Node* victim = buckets[minFreq].tail.prev;
            remove(victim);
            nodes.erase(victim->key);
            delete victim;
        }
        Node* node = new Node{key, value, 1, nullptr, nullptr};
        nodes[key] = node;
        pushFront(node);
        minFreq = 1;   // a new key is always the least frequent
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LFUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1) == 1, "hit");
    c.put(3, 3);
    check(c.get(2) == -1 && c.get(3) == 3, "evict least freq");
    c.put(4, 4);
    check(c.get(1) == -1 && c.get(3) == 3 && c.get(4) == 4, "tie -> LRU");
}
