#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;

// T: O(loga+b), S: O(loga+b)
vector<string> buildPath(long long a, long long b) {
    vector<string> ops;
    if (a == b) {
        return ops;
    }

    while (a > 1) {
        ops.push_back("split");
        a /= 2;
    }

    if (b % 2 == 0) {
        ops.push_back("dub");
        a = 2;
    }

    while (a < b) {
        ops.push_back("add");
        a += 2;
    }
    return ops;
}

// Test helper. T: O(k), S: O(1), where k = number of operations.
long long replay(long long a, const vector<string>& ops) {
    for (const string& op : ops) {
        if (op == "add") {
            a += 2;
        } else if (op == "dub") {
            a *= 2;
        } else {
            assert(op == "split");
            a /= 2;
        }
        assert(a > 0);
    }
    return a;
}

int main() {
    assert(replay(5, buildPath(5, 6)) == 6); // Even target.
    assert(replay(8, buildPath(8, 7)) == 7); // Odd target.
    assert(replay(10, buildPath(10, 1)) == 1); // Target is 1.
    assert(buildPath(4, 4).empty()); // Already equal.
}
