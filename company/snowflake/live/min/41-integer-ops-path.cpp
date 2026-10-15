// B21 · LC 397 weak prototype: turn positive a into positive b with +2, *2, floor /2, every value >= 1; return ANY valid sequence. Follow-up: constructive, no value bound.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;

// BFS over [1, 2 * max(a, b) + 2] with parent pointers, T: O(M), S: O(M)
vector<string> findPath(int a, int b) {
    int limit = 2 * std::max(a, b) + 2;   // halving path stays <= max(a, b); slack lets 2b, 2b+1 halve onto b
    vector<int> parent(limit + 1, -1);    // value -> value it came from, -1 = unvisited
    vector<string> how(limit + 1);        // value -> op that reached it
    string names[] = {"+2", "*2", "/2"};
    std::queue<int> q;
    parent[a] = a;
    q.push(a);
    while (!q.empty() && parent[b] == -1) {
        int x = q.front();
        q.pop();
        int nexts[] = {x + 2, x * 2, x / 2};
        for (int d = 0; d < 3; d++) {
            int y = nexts[d];
            if (y < 1 || y > limit || parent[y] != -1) { continue; }
            parent[y] = x;
            how[y] = names[d];
            q.push(y);
        }
    }
    vector<string> ops;   // b back to a, reversed at the end
    if (parent[b] == -1) { return ops; }   // can't happen for positive a, b
    for (int v = b; v != a; v = parent[v]) { ops.push_back(how[v]); }
    std::reverse(ops.begin(), ops.end());
    return ops;
}

// follow-up: halve to 1, double to 2 if b is even, then +2 up to b, T: O(log a + b), S: output
vector<string> findPath2(long long a, long long b) {
    vector<string> ops;
    if (a == b) { return ops; }
    while (a > 1) {
        ops.push_back("/2");   // a >= 2 so a / 2 >= 1, never 0
        a /= 2;
    }
    if (b % 2 == 0) {
        ops.push_back("*2");
        a = 2;
    }
    while (a < b) {
        ops.push_back("+2");
        a += 2;
    }
    return ops;
}

// replay ops from a; -1 if a value drops below 1
long long replay(long long a, const vector<string>& ops) {
    for (const string& op : ops) {
        if (op == "+2") { a += 2; } else if (op == "*2") { a *= 2; } else { a /= 2; }
        if (a < 1) { return -1; }
    }
    return a;
}

void test(int a, int b) {
    vector<string> ops = findPath(a, b);
    long long got = (a == b && !ops.empty()) ? -1 : replay(a, ops);
    cout << (got == b ? "PASS " : "FAIL ") << got << " expect " << b << "\n";
}

void test2(long long a, long long b) {
    vector<string> ops = findPath2(a, b);
    long long got = (a == b && !ops.empty()) ? -1 : replay(a, ops);
    cout << (got == b ? "PASS " : "FAIL ") << got << " expect " << b << "\n";
}

int main() {
    test(5, 6);          // target: odd a to even b
    test(4, 4);          // a == b -> empty
    test(10, 1);         // must stop at 1, never 0
    test(1, 17);         // shortest path overshoots b

    test2(5, 6);         // target
    test2(1, 1);         // a == b -> empty
    test2(10, 1);        // halving only
    test2(7, 1000);      // long +2 chain
}
