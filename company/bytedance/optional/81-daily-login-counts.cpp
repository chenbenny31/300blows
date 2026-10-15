// ByteDance (hack2hire): records (timestamp "YYYY-MM-DD HH:MM:SS", date "YYYY-MM-DD",
// username). Skip invalid records (bad format or calendar value, timestamp day != date,
// empty username); count valid logins per (username, date), sorted by user then date.
// Follow-up: also report each invalid record's index and reason.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::tuple;

struct Record {
    string timestamp, date, user;
};

// fmt: 'd' = any digit, every other char must match exactly
bool matches(const string& s, const string& fmt) {
    if (s.size() != fmt.size()) { return false; }
    for (int i = 0; i < (int)s.size(); i++) {
        if (fmt[i] == 'd') {
            if (s[i] < '0' || s[i] > '9') { return false; }
        } else if (s[i] != fmt[i]) {
            return false;
        }
    }
    return true;
}

// value of the len digits starting at pos (already checked to be digits)
int num(const string& s, int pos, int len) {
    int v = 0;
    for (int i = pos; i < pos + len; i++) { v = v * 10 + (s[i] - '0'); }
    return v;
}

bool validDate(const string& d) {
    if (!matches(d, "dddd-dd-dd")) { return false; }
    int y = num(d, 0, 4), m = num(d, 5, 2), day = num(d, 8, 2);
    if (m < 1 || m > 12) { return false; }
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   // non-leap year
    bool leap = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
    int maxDay = (m == 2 && leap) ? 29 : days[m - 1];
    return day >= 1 && day <= maxDay;
}

bool validTimestamp(const string& ts) {
    if (!matches(ts, "dddd-dd-dd dd:dd:dd")) { return false; }
    if (!validDate(ts.substr(0, 10))) { return false; }
    return num(ts, 11, 2) < 24 && num(ts, 14, 2) < 60 && num(ts, 17, 2) < 60;
}

// "" when r is valid, else the first problem found
string invalidReason(const Record& r) {
    if (!validTimestamp(r.timestamp)) { return "bad timestamp"; }
    if (!validDate(r.date)) { return "bad date"; }
    if (r.timestamp.substr(0, 10) != r.date) { return "date mismatch"; }
    if (r.user.empty()) { return "empty username"; }
    return "";
}

// Validate each record, count in an ordered map. T: O(n log n), S: O(n).
vector<tuple<string, string, int>> dailyLogins(const vector<Record>& records) {
    map<pair<string, string>, int> cnt;   // (user, date) -> valid logins, sorted
    for (const auto& r : records) {
        if (!invalidReason(r).empty()) { continue; }
        cnt[{r.user, r.date}]++;
    }
    vector<tuple<string, string, int>> result;
    for (const auto& [key, c] : cnt) { result.push_back({key.first, key.second, c}); }
    return result;
}

// follow-up: (index, reason) for every invalid record. T: O(n), S: O(n).
vector<pair<int, string>> invalidRecords(const vector<Record>& records) {
    vector<pair<int, string>> bad;   // (record index, why it was skipped)
    for (int i = 0; i < (int)records.size(); i++) {
        string why = invalidReason(records[i]);
        if (!why.empty()) { bad.push_back({i, why}); }
    }
    return bad;
}

void test(const string& label, const vector<Record>& records,
          const vector<tuple<string, string, int>>& want) {
    cout << label << (dailyLogins(records) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", {
        {"2024-06-01 09:00:00", "2024-06-01", "alice"},
        {"2024-06-01 18:30:00", "2024-06-01", "alice"},
        {"2024-06-02 08:00:00", "2024-06-02", "alice"},
        {"2024-06-01 10:00:00", "2024-06-01", "bob"},
        {"2024-06-01 25:00:00", "2024-06-01", "bob"},
        {"2024-06-03 10:00:00", "2024-06-02", "carol"},
    }, {{"alice", "2024-06-01", 2}, {"alice", "2024-06-02", 1},
        {"bob", "2024-06-01", 1}});
    test("feb 29", {
        {"2023-02-29 10:00:00", "2023-02-29", "alice"},
        {"2024-02-29 10:00:00", "2024-02-29", "alice"},
    }, {{"alice", "2024-02-29", 1}});
}
