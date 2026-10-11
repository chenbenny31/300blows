// positive integers a, b; operations: add (+2), dub (x2), split (floor /2)
// return ANY sequence turning a into b with every intermediate value positive, not the shortest (Q44, J10691)

// any valid sequence, or shortest?
// a == b: empty sequence?
// every intermediate must stay positive?
// very large b: is a long output acceptable?

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class OpsPath {
public:
    // split down to 1, then dub to 2 if b is even, then add up to b; each phase only moves one way
    // T: O(log a + b), the output length, S: O(log a + b)
    vector<string> buildPath(long long a, long long b) {
        vector<string> ops;
        if (a == b) {
            return ops;
        }
        while (a > 1) {
            ops.push_back("split"); // floor(a / 2) >= 1 while a > 1, stays positive
            a /= 2;
        }
        if (b % 2 == 0) {
            ops.push_back("dub"); // 1 -> 2, now adds keep the parity of b
            a = 2;
        }
        while (a < b) {
            ops.push_back("add");
            a += 2;
        }
        return ops;
    }
};

// test helper: replay ops from a, -1 if any value goes non-positive
long long runOps(long long a, const vector<string>& ops) {
    for (const string& op : ops) {
        if (op == "add") {
            a += 2;
        } else if (op == "dub") {
            a *= 2;
        } else {
            a /= 2;
        }
        if (a <= 0) {
            return -1;
        }
    }
    return a;
}

int main() {
    OpsPath op;

    assert(runOps(5, op.buildPath(5, 6)) == 6); // target: odd a, even b
    assert(runOps(8, op.buildPath(8, 7)) == 7); // edge: even a, odd b, no dub
    assert(runOps(10, op.buildPath(10, 1)) == 1); // edge: b = 1, splits only
    assert(op.buildPath(4, 4).empty()); // null: a == b

    std::cout << "all tests passed\n";
    return 0;
}
