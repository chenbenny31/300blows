// B21 (no direct LC; 397 weak prototype): turn positive a into positive b using +2, *2,
// floor /2, every value staying >= 1; return ANY valid op sequence (not the shortest).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::max;
using std::reverse;

// BFS over values [1, 2*max(a,b)+2] with parent pointers. T: O(M), S: O(M), M = max(a,b).
// b is always reachable in range: halve to 1, *2 if b is even, then +2 up to b.
vector<string> findPath(int a, int b) {
    int limit = 2 * max(a, b) + 2;
    vector<int> parent(limit + 1, -1);   // parent[v] = value v came from; -1 = unseen
    vector<string> how(limit + 1);       // how[v] = op that reached v
    string names[] = {"+2", "*2", "/2"};
    queue<int> q;                        // values found, not yet expanded
    parent[a] = a;
    q.push(a);
    while (!q.empty() && parent[b] == -1) {
        int cur = q.front();
        q.pop();
        int cand[] = {cur + 2, cur * 2, cur / 2};   // cand[d] = value after op d
        for (int d = 0; d < 3; d++) {
            int nxt = cand[d];
            if (nxt < 1 || nxt > limit || parent[nxt] != -1) { continue; }
            parent[nxt] = cur;
            how[nxt] = names[d];
            q.push(nxt);
        }
    }
    vector<string> ops;   // ops from b back to a, reversed at the end
    for (int v = b; v != a; v = parent[v]) { ops.push_back(how[v]); }
    reverse(ops.begin(), ops.end());
    return ops;
}

// Apply ops to a; -1 if any value drops below 1.
int replay(int a, const vector<string>& ops) {
    for (const string& op : ops) {
        if (op == "+2") {
            a += 2;
        } else if (op == "*2") {
            a *= 2;
        } else {
            a /= 2;
        }
        if (a < 1) { return -1; }
    }
    return a;
}

void test(const string& label, int a, int b) {
    cout << label << (replay(a, findPath(a, b)) == b ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd to even", 5, 6);
    test("down to 1", 10, 1);
    test("same", 4, 4);
}
