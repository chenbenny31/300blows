// B13 · LC 39/216 backtracking pattern: 7-char pattern of digits and '?'; fill each '?' with 0..dayHours so the days sum to workHours; all schedules, lexicographic.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

void dfs(string& cur, const vector<int>& slots, int idx, int remaining, int dayHours, vector<string>& result) {
    int left = (int)slots.size() - idx;   // '?' days not yet filled
    if (remaining < 0 || remaining > left * dayHours) { return; }   // prune: left days hold 0..left * dayHours
    if (left == 0) {
        result.push_back(cur);            // prune above guarantees remaining == 0
        return;
    }
    for (int h = 0; h <= dayHours && h <= remaining; h++) {
        cur[slots[idx]] = '0' + h;        // overwritten on the next try, no undo needed
        dfs(cur, slots, idx + 1, remaining - h, dayHours, result);
    }
}

// backtrack '?' slots left to right, digits ascending -> output already sorted, T: O(7 + A * (m * D + 7)), S: O(m) + output
vector<string> findSchedules(const string& pattern, int workHours, int dayHours) {
    vector<int> slots;           // indices of '?' in pattern
    int remaining = workHours;   // hours still to place on '?' days
    for (int i = 0; i < 7; i++) {
        if (pattern[i] == '?') {
            slots.push_back(i);
        } else {
            remaining -= pattern[i] - '0';
        }
    }
    string cur = pattern;        // pattern with slots[0..idx) filled
    vector<string> result;       // finished schedules, lexicographic
    dfs(cur, slots, 0, remaining, dayHours, result);
    return result;
}

void test(const string& label, const string& pattern, int workHours, int dayHours, const vector<string>& expect) {
    vector<string> got = findSchedules(pattern, workHours, dayHours);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    test("example", "??2??00", 3, 2, {"0020100", "0021000", "0120000", "1020000"});   // target, sorted order
    test("no ?", "1111111", 6, 8, {});                                                // fixed days don't match
    test("full", "???????", 63, 9, {"9999999"});                                      // max total, every day full
    test("day cap", "??00000", 3, 2, {"1200000", "2100000"});                         // no "0300000"
    test("fixed > cap", "9??????", 10, 1, {"9000001", "9000010", "9000100", "9001000", "9010000", "9100000"});   // fixed 9 kept
}
