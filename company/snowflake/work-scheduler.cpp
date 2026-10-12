#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;

class ScheduleBuilder {
private:
    void dfs(string& schedule, const vector<int>& slots, int index,
             int remain, int dayHours, vector<string>& result) {
        int left = static_cast<int>(slots.size()) - index;
        if (remain < 0 || remain > left * dayHours) { return; }

        if (left == 0) {
            result.push_back(schedule);
            return;
        }

        for (int h = 0; h <= dayHours && h <= remain; h++) {
            schedule[slots[index]] = static_cast<char>('0' + h);
            dfs(schedule, slots, index + 1, remain - h, dayHours, result);
        }
    }

public:
    // T: O((D+1)^m), S: O(m), D=dayHours, m = num-'?'
    vector<string> findSchedules(string pattern, int workHours, int dayHours) {
        vector<int> slots;
        int remain = workHours;

        for (int i = 0; i < static_cast<int>(pattern.size()); i++) {
            if (pattern[i] == '?') {
                slots.push_back(i);
            } else {
                remain -= pattern[i] - '0';
            }
        }

        vector<string> result;
        dfs(pattern, slots, 0, remain, dayHours, result);
        return result;
    }
};

int main() {
    ScheduleBuilder builder;

    vector<string> expected = {"0120000", "1020000"};
    assert(builder.findSchedules("??20000", 3, 2) == expected);
    // Fixed hours deducted; results sorted.

    expected = {"1111111"};
    assert(builder.findSchedules("1111111", 7, 2) == expected);
    // No unknown days.

    assert(builder.findSchedules("2000000", 1, 2).empty());
    // Fixed hours already exceed the target.
    assert(builder.findSchedules("?000000", 3, 2).empty());
    // Remaining capacity is insufficient.

    expected = {"0000000"};
    assert(builder.findSchedules("?000000", 0, 0) == expected);
    // Zero allowed hours.
}
