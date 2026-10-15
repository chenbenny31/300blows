// LC 787: n cities, flights[i] = [from, to, price]. Return the cheapest price from src
// to dst with at most k stops, or -1 if there is no such route.
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;

// Bellman-Ford with k+1 rounds (k stops = k+1 edges). T: O(k*E), S: O(V).
int findCheapestPrice(int n, const vector<vector<int>>& flights, int src, int dst,
                      int k) {
    vector<int> dist(n, INT_MAX);   // dist[c] = cheapest price to c using <= round edges
    dist[src] = 0;
    for (int round = 0; round <= k; round++) {
        // relax from last round's copy so a path grows by at most one edge per round
        vector<int> prev = dist;
        for (const auto& f : flights) {
            if (prev[f[0]] == INT_MAX) { continue; }
            dist[f[1]] = min(dist[f[1]], prev[f[0]] + f[2]);
        }
    }
    return dist[dst] == INT_MAX ? -1 : dist[dst];
}

void test(const string& label, int n, const vector<vector<int>>& flights, int src,
          int dst, int k, int want) {
    bool ok = findCheapestPrice(n, flights, src, dst, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one stop", 3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}}, 0, 2, 1, 200);
    test("zero stops", 3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}}, 0, 2, 0, 500);
    test("lc ex1", 4, {{0, 1, 100}, {1, 2, 100}, {2, 0, 100}, {1, 3, 600}, {2, 3, 200}},
         0, 3, 1, 700);
    test("unreachable", 2, {}, 0, 1, 1, -1);
}
