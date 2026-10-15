// ByteDance (hack2hire) / LC 1654: a bug starts at 0 and jumps a forward or b backward;
// it can't jump backward twice in a row, land on a forbidden position, or go below 0.
// Return the min jumps to reach x, or -1.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::max;

// BFS on (position, last jump was backward). T: O(B), S: O(B); B = position bound.
int minimumJumps(const vector<int>& forbidden, int a, int b, int x) {
    int maxF = 0;
    for (int f : forbidden) { maxF = max(maxF, f); }
    // past this bound going further forward never helps
    int bound = max(x, maxF) + a + b;
    vector<bool> blocked(bound + 1, false);   // blocked[p] = p is forbidden
    for (int f : forbidden) { blocked[f] = true; }
    // seen[p][back] = state reached; back = 1 if it was reached by a backward jump
    vector<vector<bool>> seen(bound + 1, vector<bool>(2, false));
    queue<pair<int, int>> q;   // (position, back) at the current distance
    q.push({0, 0});
    seen[0][0] = true;
    for (int jumps = 0; !q.empty(); jumps++) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [cur, back] = q.front();
            q.pop();
            if (cur == x) { return jumps; }
            int fwd = cur + a;
            if (fwd <= bound && !blocked[fwd] && !seen[fwd][0]) {
                seen[fwd][0] = true;
                q.push({fwd, 0});
            }
            int bwd = cur - b;
            // no backward jump right after a backward jump
            if (!back && bwd >= 0 && !blocked[bwd] && !seen[bwd][1]) {
                seen[bwd][1] = true;
                q.push({bwd, 1});
            }
        }
    }
    return -1;
}

void test(const string& label, const vector<int>& forbidden, int a, int b, int x,
          int want) {
    cout << label << (minimumJumps(forbidden, a, b, x) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {14, 4, 18, 1, 15}, 3, 15, 9, 3);
    test("unreachable", {8, 3, 16, 6, 12, 20}, 15, 13, 11, -1);
    test("back jump", {1, 6, 2, 14, 5, 17, 4}, 16, 9, 7, 2);
    test("forward only", {}, 3, 2, 9, 3);
}
