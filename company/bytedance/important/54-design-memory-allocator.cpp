// ByteDance / LC 2502: memory of n units. allocate(size, mID) takes the leftmost block
// of size consecutive free units and returns its first index (-1 if none);
// freeMemory(mID) frees every unit owned by mID and returns how many were freed.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

class Allocator {
    vector<int> mem;   // mem[i] = mID owning unit i, 0 = free

public:
    Allocator(int n) : mem(n, 0) {}

    // first fit: track the free run ending at i. T: O(n).
    int allocate(int size, int mID) {
        int run = 0;   // consecutive free units ending at i
        for (int i = 0; i < (int)mem.size(); i++) {
            run = mem[i] == 0 ? run + 1 : 0;
            if (run == size) {
                for (int j = i - size + 1; j <= i; j++) { mem[j] = mID; }
                return i - size + 1;
            }
        }
        return -1;
    }

    // T: O(n).
    int freeMemory(int mID) {
        int freed = 0;
        for (int& owner : mem) {
            if (owner == mID) {
                owner = 0;
                freed++;
            }
        }
        return freed;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Allocator a(10);
    check(a.allocate(1, 1) == 0 && a.allocate(1, 2) == 1 && a.allocate(1, 3) == 2,
          "sequential");
    check(a.freeMemory(2) == 1 && a.allocate(3, 4) == 3, "skip small hole");
    check(a.allocate(1, 1) == 1 && a.allocate(1, 1) == 6 && a.freeMemory(1) == 3,
          "free scattered");
    check(a.allocate(10, 2) == -1 && a.freeMemory(7) == 0, "no room");
}
