#include <cassert>
#include <unordered_map>
#include <utility>

using std::pair;
using std::unordered_map;

struct Node {
    int key;
    int value;
    Node* prev = nullptr;;
    Node* next = nullptr;;
};

class LRUCache {
private:
    int cap;
    Node head{0, 0}; // most recent: head.next 
    Node tail{0, 0}; // least recent: tail.prev
    unordered_map<int, Node*> nodes;

    // T: O(1), S: O(1)
    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // T: O(1), S: O(1)
    void addFront(Node* node) {
        node->prev = &head;
        node->next = head.next;
        head.next->prev = node;
        head.next = node;
    }

    // T: O(k), S: O(1)
    void evict() {
        while (static_cast<int>(nodes.size()) > cap) {
            Node* oldest = tail.prev;
            remove(oldest);
            nodes.erase(oldest->key);
            delete oldest;
        }
    }

public:
    // T: O(1), S: O(1)
    explicit LRUCache(int cap) : cap(cap) {
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

    // T: O(1), S: O(1)
    int get(int key) {
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }

        Node* node = it->second;
        remove(node);
        addFront(node);
        return node->value;
    }

    // T: O(1), S: O(1)
    void put(int key, int value) {
        auto it = nodes.find(key);

        if (it != nodes.end()) {
            Node* node = it->second;
            node->value = value;
            remove(node);
            addFront(node);
            return;
        }

        if (cap == 0) { return; }
        Node* node = new Node{key, value};
        nodes[key] = node;
        addFront(node);
        evict();
    }

    // T: O(k), S: O(1)
    void resize(int newCap) {
        cap = newCap;
        evict();
    }
};

int main() {
    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);

    assert(cache.get(1) == 10);
    cache.put(3, 30);
    assert(cache.get(2) == -1); // evict least recent 2

    cache.put(1, 100);
    cache.put(4, 40);
    assert(cache.get(3) == -1);
    assert(cache.get(1) == 100);

    cache.resize(1);
    assert(cache.get(4) == -1);
}
