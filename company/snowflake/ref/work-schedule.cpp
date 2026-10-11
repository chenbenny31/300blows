// pattern of 7 chars, digits are fixed hours, '?' takes 0..dayHours
// return every filled pattern whose 7 days sum to exactly workHours, in lexicographic order (Q36)

// pattern always 7 chars of '0'-'9' and '?'?
// dayHours <= 9 so each day stays one digit?
// fixed digit larger than dayHours: allowed as is?
// no valid schedule: empty list?

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class ScheduleBuilder {
public:
    // DFS over '?' slots left to right, digits 0..D ascending -> output already sorted
    // T: O((D+1)^m + 7A), m: '?' count, A: answers; S: O(m) recursion + O(7A) output
    vector<string> findSchedules(const string& pattern, int workHours, int dayHours) {
        vector<int> slots; // indices of '?'
        int remaining = workHours; // hours still to place on '?' days
        for (int i = 0; i < static_cast<int>(pattern.size()); i++) {
            if (pattern[i] == '?') {
                slots.push_back(i);
            } else {
                remaining -= pattern[i] - '0';
            }
        }
        string cur = pattern;
        vector<string> result;
        fill(cur, slots, 0, remaining, dayHours, result);
        return result;
    }

private:
    void fill(string& cur, const vector<int>& slots, int idx, int remaining, int dayHours, vector<string>& result) {
        int left = static_cast<int>(slots.size()) - idx; // '?' days not yet filled
        if (remaining < 0 || remaining > left * dayHours) {
            return; // prune: total can no longer be hit exactly
        }
        if (left == 0) {
            result.push_back(cur); // remaining == 0 here, the prune rejected everything else
            return;
        }
        for (int h = 0; h <= dayHours && h <= remaining; h++) {
            cur[slots[idx]] = static_cast<char>('0' + h); // overwritten on every try, no restore needed
            fill(cur, slots, idx + 1, remaining - h, dayHours, result);
        }
    }
};

int main() {
    ScheduleBuilder sb;
    vector<string> want;

    want = {"0020100", "0021000", "0120000", "1020000"};
    assert(sb.findSchedules("??2??00", 3, 2) == want); // target: doc example, already sorted
    want = {"1111111"};
    assert(sb.findSchedules("1111111", 7, 8) == want); // null: no '?', fixed days already match
    assert(sb.findSchedules("8??????", 5, 2).empty()); // fault: fixed hours already exceed the total
    want = {"0000000"};
    assert(sb.findSchedules("?000000", 0, 0) == want); // edge: dayHours = 0

    std::cout << "all tests passed\n";
    return 0;
}
