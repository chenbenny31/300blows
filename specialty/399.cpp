// LC 399 Evaluate Division: equations a / b = v; answer each query x / y, or -1.0 if it
// cannot be determined (unknown variable or no path).
// Trick: a / b = v is edge a->b (v) and b->a (1/v); x / y = product of weights on a path
// Recognize: ratios / exchange rates that chain transitively -> weighted graph search
// Follow-up: weighted union-find (weight to root) answers each query in near O(1).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::fabs;

// returns cur / target along some path, or -1.0 if target is unreachable
double dfs(unordered_map<string, vector<pair<string, double>>>& graph,
           const string& cur, const string& target, unordered_set<string>& seen) {
    if (cur == target) { return 1.0; }
    seen.insert(cur);
    for (const auto& [nxt, w] : graph[cur]) {
        if (seen.count(nxt)) { continue; }
        double rest = dfs(graph, nxt, target, seen);
        if (rest >= 0) { return w * rest; }
    }
    return -1.0;
}

// DFS per query multiplying edge weights. T: O(Q * (V+E)), S: O(V+E).
vector<double> calcEquation(const vector<vector<string>>& equations,
                            const vector<double>& values,
                            const vector<vector<string>>& queries) {
    // graph[a] = (b, a / b) for every known ratio, both directions
    unordered_map<string, vector<pair<string, double>>> graph;
    for (int i = 0; i < (int)equations.size(); i++) {
        const string& a = equations[i][0];
        const string& b = equations[i][1];
        graph[a].push_back({b, values[i]});
        graph[b].push_back({a, 1.0 / values[i]});
    }
    vector<double> result;
    for (const auto& q : queries) {
        // x / x = 1 only for a known x; an unknown variable has no value at all
        if (graph.count(q[0]) == 0 || graph.count(q[1]) == 0) {
            result.push_back(-1.0);
            continue;
        }
        unordered_set<string> seen;   // variables visited in this query's DFS
        result.push_back(dfs(graph, q[0], q[1], seen));
    }
    return result;
}

// follow-up: each variable stores its ratio to its parent; find compresses to the root
class WeightedUnionFind {
    unordered_map<string, string> parent;   // parent[x] = next node toward x's root
    unordered_map<string, double> weight;   // weight[x] = x / parent[x]

public:
    // path compression; afterwards weight[x] = x / root. T: O(alpha) amortized.
    string find(const string& x) {
        if (parent[x] == x) { return x; }
        string p = parent[x];
        string root = find(p);
        weight[x] *= weight[p];   // weight[p] is now p / root
        parent[x] = root;
        return root;
    }

    // record a / b = v by hanging a's root under b's root
    void unite(const string& a, const string& b, double v) {
        if (parent.count(a) == 0) {
            parent[a] = a;
            weight[a] = 1.0;
        }
        if (parent.count(b) == 0) {
            parent[b] = b;
            weight[b] = 1.0;
        }
        string ra = find(a), rb = find(b);
        if (ra == rb) { return; }
        parent[ra] = rb;
        // ra / rb = (a / b) * (b / rb) / (a / ra)
        weight[ra] = v * weight[b] / weight[a];
    }

    // x / y = (x / root) / (y / root) when both share a root. T: O(alpha) amortized.
    double query(const string& x, const string& y) {
        if (parent.count(x) == 0 || parent.count(y) == 0) { return -1.0; }
        if (find(x) != find(y)) { return -1.0; }
        return weight[x] / weight[y];
    }
};

void test(const string& label, const vector<vector<string>>& equations,
          const vector<double>& values, const vector<vector<string>>& queries,
          const vector<double>& want) {
    vector<double> got = calcEquation(equations, values, queries);
    bool ok = got.size() == want.size();
    for (int i = 0; ok && i < (int)want.size(); i++) {
        ok = fabs(got[i] - want[i]) < 1e-9;
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{"a", "b"}, {"b", "c"}}, {2.0, 3.0},
         {{"a", "c"}, {"b", "a"}, {"a", "e"}, {"a", "a"}, {"x", "x"}},
         {6.0, 0.5, -1.0, 1.0, -1.0});
    test("disconnected", {{"a", "b"}, {"c", "d"}}, {2.0, 4.0},
         {{"a", "d"}, {"d", "c"}}, {-1.0, 0.25});
}
