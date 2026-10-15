// LC 857: hire exactly k workers; pay is proportional to quality within the group and
// each worker gets at least their wage. Return the minimum total pay.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::sort;
using std::min;
using std::fabs;

// Sort by wage/quality; the current ratio sets the rate, so keep the k smallest qualities
// with a max-heap. T: O(n log n), S: O(n).
double mincostToHireWorkers(const vector<int>& quality, const vector<int>& wage, int k) {
    int n = quality.size();
    vector<pair<double, int>> workers;   // (wage / quality, quality), lowest ratio first
    for (int i = 0; i < n; i++) {
        workers.push_back({(double)wage[i] / quality[i], quality[i]});
    }
    sort(workers.begin(), workers.end());
    priority_queue<int> heap;   // qualities in the group; top = largest, dropped first
    int sumQ = 0;               // sum of qualities in the heap
    double best = 1e18;
    for (auto [ratio, q] : workers) {
        heap.push(q);
        sumQ += q;
        if ((int)heap.size() > k) {
            sumQ -= heap.top();
            heap.pop();
        }
        // ratio is the largest so far, so everyone is paid at least their wage
        if ((int)heap.size() == k) { best = min(best, ratio * sumQ); }
    }
    return best;
}

void test(const string& label, const vector<int>& quality, const vector<int>& wage, int k,
          double want) {
    bool ok = fabs(mincostToHireWorkers(quality, wage, k) - want) < 1e-9;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {10, 20, 5}, {70, 50, 30}, 2, 105.0);
    test("lc ex2", {3, 1, 10, 10, 1}, {4, 8, 2, 2, 7}, 3, 92.0 / 3);
    test("k = 1", {10, 20}, {70, 50}, 1, 50.0);
}
