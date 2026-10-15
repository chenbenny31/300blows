// B21 (no direct LC; 397 is only a weak prototype): turn positive a into positive b using +2, *2, floor /2;
// return ANY valid op sequence (every value stays >= 1), not necessarily the shortest.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::queue;

class OpsPathFinder {
public:
    // BFS over values in [1, 2 * max(a, b) + 2] with parent pointers, T: O(M), S: O(M), M = max(a, b)
    vector<string> findPath(int a, int b) {
        int limit = 2 * std::max(a, b) + 2;
        vector<int> parent(limit + 1, -1);    // value -> value it was reached from, -1 = unvisited
        vector<string> how(limit + 1);        // value -> op that reached it
        vector<string> names = {"+2", "*2", "/2"};   // op d, 0 <= d < 3
        queue<int> q;                          // values discovered, not yet expanded
        parent[a] = a;
        q.push(a);
        while (!q.empty() && parent[b] == -1) {
            int x = q.front();
            q.pop();
            vector<int> nexts = {x + 2, x * 2, x / 2};   // value after op d
            for (int d = 0; d < 3; d++) {
                int y = nexts[d];
                if (y < 1 || y > limit || parent[y] != -1) { continue; }
                parent[y] = x;
                how[y] = names[d];
                q.push(y);
            }
        }
        vector<string> ops;   // ops from b back to a, reversed at the end
        if (parent[b] == -1) { return ops; }   // unreachable; can't happen for positive a, b
        for (int v = b; v != a; v = parent[v]) { ops.push_back(how[v]); }
        std::reverse(ops.begin(), ops.end());
        return ops;
    }

    // constructive: halve down to 1, double to 2 if b is even, then +2 up to b
    // T: O(log a + b), the output length, S: O(log a + b)
    vector<string> findPath2(long long a, long long b) {
        vector<string> ops;
        if (a == b) { return ops; }
        while (a > 1) {
            ops.push_back("/2");   // a >= 2 so a / 2 >= 1
            a /= 2;
        }
        if (b % 2 == 0) {
            ops.push_back("*2");   // 1 -> 2, now +2 steps keep b's parity
            a = 2;
        }
        while (a < b) {
            ops.push_back("+2");
            a += 2;
        }
        return ops;
    }
};

// replay ops from a; -1 if a value drops below 1 or an op is unknown
long long replay(long long a, const vector<string>& ops) {
    for (const string& op : ops) {
        if (op == "+2") {
            a += 2;
        } else if (op == "*2") {
            a *= 2;
        } else if (op == "/2") {
            a /= 2;
        } else {
            return -1;
        }
        if (a < 1) { return -1; }
    }
    return a;
}

bool test(int a, int b) {
    OpsPathFinder pf;
    vector<string> got = pf.findPath(a, b);
    vector<string> got2 = pf.findPath2(a, b);
    bool pass = replay(a, got) == b && replay(a, got2) == b;
    if (a == b) { pass = pass && got.empty() && got2.empty(); }
    if (pass) { return true; }
    cout << "FAIL a=" << a << " b=" << b << ": replay gives " << replay(a, got) << " / " << replay(a, got2) << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test(5, 6);        // odd a to even b
    ok &= test(8, 7);        // even a to odd b, no *2 needed
    ok &= test(1, 2);        // smallest a, one *2
    ok &= test(1, 1);        // smallest, a == b -> empty sequence
    ok &= test(4, 4);        // a == b
    ok &= test(10, 1);       // b = 1: halving only, must stop at 1, never 0
    ok &= test(3, 2);        // going down by one needs /2 then *2 or similar
    ok &= test(1, 17);       // shortest path overshoots b (needs values above b)
    ok &= test(7, 1000);     // far target, long +2 chain in findPath2
    ok &= test(1000, 999);   // larger a, adjacent odd target
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
