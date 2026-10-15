// B13 (no direct LC; 39 / 216 backtracking pattern): 7-char pattern of digits and '?'; fill each '?' with 0..dayHours
// so the 7 days sum to exactly workHours; return every schedule in lexicographic order.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class ScheduleBuilder {
public:
    // backtrack over '?' slots left to right, digits ascending -> output already sorted
    // T: O(7 + A * (m * D + 7)), A: schedules returned, m: '?' count, D: dayHours; S: O(m) recursion + O(7A) output
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
        vector<string> result;       // finished schedules, in lexicographic order
        fill(cur, slots, 0, remaining, dayHours, result);
        return result;
    }

private:
    void fill(string& cur, const vector<int>& slots, int idx, int remaining, int dayHours, vector<string>& result) {
        int left = (int)slots.size() - idx;   // '?' days not yet filled
        // prune: the left days can place anywhere from 0 to left * dayHours
        if (remaining < 0 || remaining > left * dayHours) { return; }
        if (left == 0) {
            result.push_back(cur);   // prune above guarantees remaining == 0
            return;
        }
        for (int h = 0; h <= dayHours && h <= remaining; h++) {
            cur[slots[idx]] = '0' + h;   // overwritten on the next try, no undo needed
            fill(cur, slots, idx + 1, remaining - h, dayHours, result);
        }
    }
};

bool test(const string& pattern, int workHours, int dayHours, const vector<string>& expected) {
    ScheduleBuilder sb;
    vector<string> got = sb.findSchedules(pattern, workHours, dayHours);
    if (got == expected) { return true; }
    cout << "FAIL \"" << pattern << "\" work=" << workHours << " day=" << dayHours << ": got " << got.size()
         << " schedules, expected " << expected.size() << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("??2??00", 3, 2, {"0020100", "0021000", "0120000", "1020000"});   // given example, sorted order
    ok &= test("1111111", 7, 8, {"1111111"});                                     // no '?', fixed days match
    ok &= test("1111111", 6, 8, {});                                              // no '?', fixed days don't match
    ok &= test("8??????", 5, 2, {});                                              // fixed hours already exceed total
    ok &= test("?000000", 0, 0, {"0000000"});                                     // dayHours = 0
    ok &= test("???????", 63, 9, {"9999999"});                                    // max total, every day full
    ok &= test("???????", 64, 9, {});                                             // one more than capacity
    ok &= test("??00000", 3, 2, {"1200000", "2100000"});                          // per-day cap: no "0300000"
    ok &= test("9??????", 10, 1, {"9000001", "9000010", "9000100", "9001000", "9010000", "9100000"});   // fixed 9 > dayHours kept
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
