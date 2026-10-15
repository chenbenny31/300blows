// B13 (no direct LC; 39 / 216 backtracking): 7-char pattern of digits and '?'. Fill each
// '?' with 0..dayHours so the days sum to exactly workHours; return every schedule in
// lexicographic order.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Fill slots[idx..] left to right with ascending hours, so results come out sorted.
void backtrack(string& cur, const vector<int>& slots, int idx, int remaining,
               int dayHours, vector<string>& result) {
    int left = (int)slots.size() - idx;   // '?' days not yet filled
    // prune: the left days can hold anywhere from 0 to left * dayHours hours
    if (remaining < 0 || remaining > left * dayHours) { return; }
    if (left == 0) {
        result.push_back(cur);   // prune above guarantees remaining == 0
        return;
    }
    for (int h = 0; h <= dayHours && h <= remaining; h++) {
        cur[slots[idx]] = '0' + h;   // overwritten by the next h, no undo needed
        backtrack(cur, slots, idx + 1, remaining - h, dayHours, result);
    }
}

// Subtract fixed days, then backtrack over '?'. Every node kept by the prune leads to
// an answer. T: O(A * m * D), S: O(m) + output; A = answers, m = '?' count, D = dayHours.
vector<string> findSchedules(const string& pattern, int workHours, int dayHours) {
    int n = pattern.size();
    vector<int> slots;           // indices of '?' in pattern
    int remaining = workHours;   // hours still to place on '?' days
    for (int i = 0; i < n; i++) {
        if (pattern[i] == '?') {
            slots.push_back(i);
        } else {
            remaining -= pattern[i] - '0';
        }
    }
    string cur = pattern;        // pattern with slots[0..idx) filled
    vector<string> result;       // finished schedules, in lexicographic order
    backtrack(cur, slots, 0, remaining, dayHours, result);
    return result;
}

void test(const string& label, const string& pattern, int workHours, int dayHours,
          const vector<string>& want) {
    vector<string> got = findSchedules(pattern, workHours, dayHours);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", "??2??00", 3, 2, {"0020100", "0021000", "0120000", "1020000"});
    test("day cap", "??00000", 3, 2, {"1200000", "2100000"});
    test("no ?", "1111111", 7, 8, {"1111111"});
    test("fixed too big", "8??????", 5, 2, {});
}
