// LC 739 Daily Temperatures: for each day, the number of days until a warmer
// temperature; 0 if there is none.
// Trick: decreasing stack of unresolved days; a warmer day resolves every colder top
// Recognize: "next greater element" for every index.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Monotonic decreasing stack; each day is pushed and popped once. T: O(n), S: O(n).
vector<int> dailyTemperatures(const vector<int>& temps) {
    int n = temps.size();
    vector<int> wait(n, 0);   // wait[i] = days until warmer than day i, 0 if never
    vector<int> stk;          // days still waiting, temps non-increasing bottom to top
    for (int i = 0; i < n; i++) {
        // day i is the first warmer day for every colder day on top
        while (!stk.empty() && temps[stk.back()] < temps[i]) {
            wait[stk.back()] = i - stk.back();
            stk.pop_back();
        }
        stk.push_back(i);
    }
    return wait;
}

void test(const string& label, const vector<int>& temps, const vector<int>& want) {
    cout << label << (dailyTemperatures(temps) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {73, 74, 75, 71, 69, 72, 76, 73}, {1, 1, 4, 2, 1, 1, 0, 0});
    test("increasing", {30, 60, 90}, {1, 1, 0});
}
