// LC 470 Implement Rand10() Using Rand7(): build a uniform 1..10 generator from a
// uniform 1..7 generator only.
// Trick: (rand7()-1)*7 + rand7() is uniform on 1..49; reject > 40, fold 1..40 mod 10
// Recognize: turn a small uniform RNG into a different-range one -> rejection sampling
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::rand;

// stand-in for the given API: uniform on 1..7
int rand7() { return rand() % 7 + 1; }

// 40 accepted outcomes = 4 copies of 1..10, so each value has prob 4/40.
// T: O(1) expected (accept prob 40/49), S: O(1).
int rand10() {
    while (true) {
        // (row, col) of a 7x7 grid, numbered 1..49
        int idx = (rand7() - 1) * 7 + rand7();
        if (idx <= 40) { return 1 + (idx - 1) % 10; }
    }
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<int> seen(11, 0);   // seen[v] = times rand10 returned v
    bool inRange = true;
    for (int t = 0; t < 10000; t++) {
        int r = rand10();
        if (r < 1 || r > 10) {
            inRange = false;
            continue;
        }
        seen[r]++;
    }
    check(inRange, "range 1..10");
    bool all = true;
    for (int v = 1; v <= 10; v++) {
        if (seen[v] == 0) { all = false; }
    }
    check(all, "every value");
}
