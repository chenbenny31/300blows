// ByteDance (hack2hire): design a list with lpush/lpop/rpush/rpop in O(1) amortized and
// index(i) (i-th element from the left) in O(1). Pop or index on a bad position throws.
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using std::cout;
using std::string;
using std::vector;
using std::out_of_range;

class CircularList {
    vector<int> arr;   // ring buffer; logical i lives at arr[(head + i) % cap]
    int head = 0;      // physical index of logical element 0
    int size = 0;      // number of stored elements

    // double the buffer, copying in logical order so head becomes 0. T: O(n).
    void grow() {
        int cap = arr.size();
        vector<int> bigger(2 * cap);
        for (int i = 0; i < size; i++) { bigger[i] = arr[(head + i) % cap]; }
        arr = bigger;
        head = 0;
    }

public:
    // capacity must be >= 1 so doubling makes room
    CircularList(int capacity = 4) : arr(capacity) {}

    // T: O(1) amortized.
    void rpush(int x) {
        if (size == (int)arr.size()) { grow(); }
        arr[(head + size) % arr.size()] = x;
        size++;
    }

    // T: O(1) amortized.
    void lpush(int x) {
        if (size == (int)arr.size()) { grow(); }
        int cap = arr.size();
        head = (head - 1 + cap) % cap;   // + cap keeps it non-negative
        arr[head] = x;
        size++;
    }

    // T: O(1).
    int lpop() {
        if (size == 0) { throw out_of_range("lpop on empty"); }
        int x = arr[head];
        head = (head + 1) % arr.size();
        size--;
        return x;
    }

    // T: O(1).
    int rpop() {
        if (size == 0) { throw out_of_range("rpop on empty"); }
        size--;
        return arr[(head + size) % arr.size()];
    }

    // T: O(1).
    int index(int i) {
        if (i < 0 || i >= size) { throw out_of_range("bad index"); }
        return arr[(head + i) % arr.size()];
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    CircularList l;
    l.rpush(1);
    l.rpush(2);
    l.lpush(0);
    check(l.index(0) == 0 && l.index(2) == 2, "push both ends");
    check(l.lpop() == 0 && l.rpop() == 2 && l.index(0) == 1, "pop both ends");
    CircularList g(1);
    g.rpush(2);
    g.lpush(1);
    g.lpush(0);
    g.rpush(3);
    check(g.index(0) == 0 && g.index(1) == 1 && g.index(2) == 2 && g.index(3) == 3,
          "grow + wrap");
}
