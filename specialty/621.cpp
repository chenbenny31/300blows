// LC 621 Task Scheduler: tasks are letters A-Z, one unit each; same letter needs >= n
// units between runs (idle allowed). Return the min total units.
// Trick: answer = max(#tasks, (maxCnt-1)*(n+1) + #letters having maxCnt)
// Recognize: cooldown scheduling where only counts matter, not the actual order
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::max_element;
using std::count;

// Frame: maxCnt-1 blocks of n+1 slots, plus a last row of the max-count letters.
// T: O(T), S: O(1).
int leastInterval(const vector<char>& tasks, int n) {
    vector<int> cnt(26, 0);   // cnt[c] = occurrences of letter c
    for (char t : tasks) { cnt[t - 'A']++; }
    int maxCnt = *max_element(cnt.begin(), cnt.end());
    int numMax = count(cnt.begin(), cnt.end(), maxCnt);   // letters with maxCnt
    // frame smaller than the task count: slots overflow, no idle is ever needed
    return max((int)tasks.size(), (maxCnt - 1) * (n + 1) + numMax);
}

void test(const string& label, const vector<char>& tasks, int n, int want) {
    cout << label << (leastInterval(tasks, n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {'A', 'A', 'A', 'B', 'B', 'B'}, 2, 8);
    test("n 0", {'A', 'A', 'A', 'B', 'B', 'B'}, 0, 6);
    test("lc ex3", {'A', 'A', 'A', 'B', 'B', 'B'}, 3, 10);
}
