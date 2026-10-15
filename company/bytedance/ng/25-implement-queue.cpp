// ByteDance (hack2hire): implement a FIFO queue from scratch with a singly linked list:
// enqueue, dequeue (returns the front value), front, isEmpty; empty dequeue/front throw.
// Follow-up: circular-array queue that doubles its capacity when full.
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using std::cout;
using std::string;
using std::vector;
using std::out_of_range;

class Queue {
    struct Node {
        int val;
        Node* next;
    };
    Node* head = nullptr;   // front: dequeue end
    Node* tail = nullptr;   // back: enqueue end
    int size = 0;           // number of nodes

public:
    Queue() = default;

    ~Queue() {
        while (head) {
            Node* nxt = head->next;
            delete head;
            head = nxt;
        }
    }

    Queue(const Queue&) = delete; // copy construction
    Queue& operator=(const Queue&) = delete; // copy assignment

    // append after the tail. T: O(1).
    void enqueue(int x) {
        Node* node = new Node{x, nullptr};
        if (tail) {
            tail->next = node;
        } else {
            head = node;
        }
        tail = node;
        size++;
    }

    // unlink the head. T: O(1).
    int dequeue() {
        if (!head) { throw out_of_range("dequeue on empty queue"); }
        Node* node = head;
        int val = node->val;
        head = head->next;
        // removed the last node: tail must not dangle
        if (!head) { tail = nullptr; }
        delete node;
        size--;
        return val;
    }

    // T: O(1).
    int front() {
        if (!head) { throw out_of_range("front on empty queue"); }
        return head->val;
    }

    bool isEmpty() { return size == 0; }
};

// follow-up: ring buffer; double it when full.
class CircularQueue {
    vector<int> data;   // ring buffer; capacity = data.size()
    int head = 0;       // index of the front element
    int size = 0;       // number of elements stored

    // copy into a buffer twice as large, front moved to index 0. T: O(n).
    void grow() {
        int cap = data.size();
        vector<int> bigger(2 * cap);
        for (int i = 0; i < size; i++) { bigger[i] = data[(head + i) % cap]; }
        data = bigger;
        head = 0;
    }

public:
    CircularQueue() : data(4) {}

    // T: O(1) amortized.
    void enqueue(int x) {
        if (size == (int)data.size()) { grow(); }
        data[(head + size) % data.size()] = x;
        size++;
    }

    // T: O(1).
    int dequeue() {
        if (size == 0) { throw out_of_range("dequeue on empty queue"); }
        int val = data[head];
        head = (head + 1) % data.size();
        size--;
        return val;
    }

    int front() {
        if (size == 0) { throw out_of_range("front on empty queue"); }
        return data[head];
    }

    bool isEmpty() { return size == 0; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Queue q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    check(q.front() == 1 && q.dequeue() == 1 && q.dequeue() == 2 && q.dequeue() == 3,
          "FIFO");
    q.enqueue(4);
    check(q.front() == 4 && q.dequeue() == 4 && q.isEmpty(), "reuse after empty");
    bool threw = false;
    try {
        q.dequeue();
    } catch (const out_of_range&) {
        threw = true;
    }
    check(threw, "empty throws");
}
