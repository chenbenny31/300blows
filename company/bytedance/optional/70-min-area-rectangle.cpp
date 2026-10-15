// ByteDance / LC 939: distinct points with 0 <= x, y <= 40000; return the min area of a
// rectangle with sides parallel to the axes and all 4 corners in the set, else 0.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::abs;

long long key(int x, int y) { return 1LL * x * 40001 + y; }

// Each pair as a diagonal; look up the other two corners. T: O(n^2), S: O(n).
int minAreaRect(const vector<vector<int>>& points) {
    unordered_set<long long> pts;   // key(x, y) of every point
    for (const auto& p : points) { pts.insert(key(p[0], p[1])); }
    int n = points.size();
    int best = 0;                   // 0 = no rectangle found yet
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int x1 = points[i][0], y1 = points[i][1];
            int x2 = points[j][0], y2 = points[j][1];
            // same row or column cannot be a diagonal
            if (x1 == x2 || y1 == y2) { continue; }
            if (!pts.count(key(x1, y2)) || !pts.count(key(x2, y1))) { continue; }
            int area = abs(x1 - x2) * abs(y1 - y2);
            if (best == 0 || area < best) { best = area; }
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& points, int want) {
    cout << label << (minAreaRect(points) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{1, 1}, {1, 3}, {3, 1}, {3, 3}, {2, 2}}, 4);
    test("lc ex2", {{1, 1}, {1, 3}, {3, 1}, {3, 3}, {4, 1}, {4, 3}}, 2);
    test("none", {{1, 1}, {1, 3}, {3, 1}}, 0);
}
