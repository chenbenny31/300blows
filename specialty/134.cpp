// LC 134 Gas Station: circular route, gas[i] at station i, cost[i] to drive to i+1.
// Return the unique start index that completes the loop with an empty tank, else -1.
// Trick: if the tank goes negative at i, no start in [start, i] works; restart at i+1
// Recognize: circular route with gains/costs and one valid start.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Greedy one pass: total >= 0 guarantees a start exists, and every start before the
// last reset was ruled out (each arrived at its failure point with tank >= 0).
// T: O(n), S: O(1).
int canCompleteCircuit(const vector<int>& gas, const vector<int>& cost) {
    int n = gas.size();
    int total = 0;   // net gas over the whole loop
    int tank = 0;    // net gas since the current start
    int start = 0;
    for (int i = 0; i < n; i++) {
        total += gas[i] - cost[i];
        tank += gas[i] - cost[i];
        if (tank < 0) {
            start = i + 1;
            tank = 0;
        }
    }
    return total < 0 ? -1 : start;
}

void test(const string& label, const vector<int>& gas, const vector<int>& cost,
          int want) {
    cout << label << (canCompleteCircuit(gas, cost) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, 3, 4, 5}, {3, 4, 5, 1, 2}, 3);
    test("lc ex2", {2, 3, 4}, {3, 4, 3}, -1);
    test("one station", {5}, {4}, 0);
}
