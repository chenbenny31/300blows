// ByteDance (hack2hire): likes[i] = name of the KOL who got the i-th like. Return the
// k KOLs with the most likes, best first; ties by name ascending.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::priority_queue;
using std::reverse;

// Count, then keep a size-k heap whose top is the worst kept candidate.
// T: O(N + U log k), S: O(U); U = distinct KOLs.
vector<string> topKKols(const vector<string>& likes, int k) {
    unordered_map<string, int> cnt;   // cnt[name] = likes received
    for (const string& name : likes) { cnt[name]++; }
    // max-heap of (-likes, name): top = fewest likes, ties -> largest name = worst
    priority_queue<pair<int, string>> heap;
    for (const auto& [name, c] : cnt) {
        heap.push({-c, name});
        if ((int)heap.size() > k) { heap.pop(); }   // drop the worst candidate
    }
    vector<string> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    reverse(result.begin(), result.end());   // popped worst-first
    return result;
}

void test(const string& label, const vector<string>& likes, int k,
          const vector<string>& want) {
    cout << label << (topKKols(likes, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tie by name", {"A", "B", "A", "C", "B", "A", "D", "B"}, 2, {"A", "B"});
    test("likes beat name", {"B", "A", "B"}, 1, {"B"});
}
