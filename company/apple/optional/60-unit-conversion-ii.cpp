// LC 3535: n units, conversions[i] = [source, target, factor]: one source = factor
// targets; they form a tree rooted at unit 0. For each query [a, b] return how many b
// equal one a, as p * q^-1 mod 1e9+7.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::queue;

const long long MOD = 1000000007;

// fast power; base^(MOD-2) is the modular inverse (Fermat). T: O(log e).
long long power(long long base, long long e) {
    long long result = 1;
    base %= MOD;
    while (e > 0) {
        if (e & 1) { result = result * base % MOD; }
        base = base * base % MOD;
        e >>= 1;
    }
    return result;
}

// BFS from unit 0: val[u] = units of u in one unit 0; one a = val[b] / val[a] of b.
// T: O(V + Q log MOD), S: O(V).
vector<int> queryConversions(const vector<vector<int>>& conversions,
                             const vector<vector<int>>& queries) {
    int n = conversions.size() + 1;
    vector<vector<pair<int, int>>> graph(n);   // graph[s] = (t, f): one s = f units of t
    for (const auto& c : conversions) { graph[c[0]].push_back({c[1], c[2]}); }
    vector<long long> val(n, 0);   // val[u] = units of u in one unit 0, mod MOD
    val[0] = 1;
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (auto [nxt, f] : graph[cur]) {
            val[nxt] = val[cur] * f % MOD;
            q.push(nxt);
        }
    }
    vector<int> result;
    for (const auto& qr : queries) {
        result.push_back(val[qr[1]] * power(val[qr[0]], MOD - 2) % MOD);
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& conversions,
          const vector<vector<int>>& queries, const vector<int>& want) {
    bool ok = queryConversions(conversions, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{0, 1, 2}, {0, 2, 6}}, {{1, 2}, {1, 0}}, {3, 500000004});
    test("lc ex2", {{0, 1, 2}, {0, 2, 6}, {0, 3, 8}, {2, 4, 2}, {2, 5, 4}, {3, 6, 3}},
         {{1, 2}, {0, 4}, {6, 5}, {4, 6}, {6, 1}}, {3, 12, 1, 2, 83333334});
}
