// A17 · LC 207/210 prototype: discover courses reachable from entries via prevCourses, take them Kahn-style; hasFailed -> retry at the back of the queue. Follow-up: give up after maxAttempts.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <functional>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

// BFS discovery with deduped edges + Kahn's queue, requeue on failure, T: O(V + R + F), S: O(V + E)
vector<string> completionOrder(const vector<string>& entries, const unordered_map<string, vector<string>>& prevCourses,
                               const std::function<bool(const string&)>& hasFailed) {
    vector<string> courses;                             // reachable courses, in discovery order
    unordered_set<string> seen;                         // courses already in `courses`
    unordered_map<string, vector<string>> dependents;   // course -> courses that list it as a prereq
    unordered_map<string, int> waiting;                 // course -> distinct prereqs not yet passed
    for (const auto& e : entries) {
        if (seen.insert(e).second) { courses.push_back(e); }
    }
    for (int i = 0; i < (int)courses.size(); i++) {     // courses grows while we scan: BFS
        string c = courses[i];
        waiting[c] = 0;
        auto it = prevCourses.find(c);
        if (it == prevCourses.end()) { continue; }      // not a key: no prereqs
        unordered_set<string> linked;                   // prereqs of c already turned into an edge
        for (const auto& p : it->second) {
            if (seen.insert(p).second) { courses.push_back(p); }
            if (!linked.insert(p).second) { continue; } // duplicate reference: one edge only
            dependents[p].push_back(c);
            waiting[c]++;
        }
    }
    std::queue<string> ready;                           // courses with all prereqs passed, in try order
    for (const auto& c : courses) {
        if (waiting[c] == 0) { ready.push(c); }
    }
    vector<string> passed;
    while (!ready.empty()) {
        string c = ready.front();
        ready.pop();
        if (hasFailed(c)) {
            ready.push(c);                              // retry after everything already waiting
            continue;
        }
        passed.push_back(c);
        for (const auto& d : dependents[c]) {
            if (--waiting[d] == 0) { ready.push(d); }
        }
    }
    return passed;
}

// follow-up: same, a course failing maxAttempts times is dropped and its dependents never run, T/S as above
vector<string> completionOrderWithLimit(const vector<string>& entries, const unordered_map<string, vector<string>>& prevCourses,
                                        const std::function<bool(const string&)>& hasFailed, int maxAttempts) {
    vector<string> courses;                             // reachable courses, in discovery order
    unordered_set<string> seen;                         // courses already in `courses`
    unordered_map<string, vector<string>> dependents;   // course -> courses that list it as a prereq
    unordered_map<string, int> waiting;                 // course -> distinct prereqs not yet passed
    for (const auto& e : entries) {
        if (seen.insert(e).second) { courses.push_back(e); }
    }
    for (int i = 0; i < (int)courses.size(); i++) {
        string c = courses[i];
        waiting[c] = 0;
        auto it = prevCourses.find(c);
        if (it == prevCourses.end()) { continue; }
        unordered_set<string> linked;                   // prereqs of c already turned into an edge
        for (const auto& p : it->second) {
            if (seen.insert(p).second) { courses.push_back(p); }
            if (!linked.insert(p).second) { continue; }
            dependents[p].push_back(c);
            waiting[c]++;
        }
    }
    std::queue<string> ready;                           // courses with all prereqs passed, in try order
    for (const auto& c : courses) {
        if (waiting[c] == 0) { ready.push(c); }
    }
    unordered_map<string, int> failures;                // course -> failed attempts so far
    vector<string> passed;
    while (!ready.empty()) {
        string c = ready.front();
        ready.pop();
        if (hasFailed(c)) {
            if (++failures[c] < maxAttempts) { ready.push(c); }
            continue;
        }
        passed.push_back(c);
        for (const auto& d : dependents[c]) {
            if (--waiting[d] == 0) { ready.push(d); }
        }
    }
    return passed;
}

// fails: course -> how many times it fails before passing
void test(const string& label, const vector<string>& entries, const unordered_map<string, vector<string>>& prev,
          unordered_map<string, int> fails, const vector<string>& expect) {
    vector<string> got = completionOrder(entries, prev, [&fails](const string& c) { return fails[c]-- > 0; });
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

void testLimit(const string& label, const vector<string>& entries, const unordered_map<string, vector<string>>& prev,
               unordered_map<string, int> fails, int maxAttempts, const vector<string>& expect) {
    vector<string> got = completionOrderWithLimit(entries, prev, [&fails](const string& c) { return fails[c]-- > 0; }, maxAttempts);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    test("chain", {"C"}, {{"C", {"B"}}, {"B", {"A"}}}, {}, {"A", "B", "C"});            // discovery walks back to A
    test("empty", {}, {}, {}, {});                                                      // no entry courses
    test("diamond", {"D"}, {{"D", {"B", "C"}}, {"B", {"A"}}, {"C", {"A"}}}, {{"B", 2}},
         {"A", "C", "B", "D"});                                                         // failed B goes behind C, D waits
    test("dup refs", {"B", "B", "A"}, {{"B", {"A", "A"}}}, {}, {"A", "B"});             // double-counted edge would block B

    testLimit("blocked", {"C"}, {{"C", {"B"}}, {"B", {"A"}}}, {{"A", 5}}, 3, {});       // A dropped, chain never runs
    testLimit("within", {"A", "X"}, {{"A", {"B"}}}, {{"B", 1}}, 2, {"X", "B", "A"});    // 1 failure < 2 attempts
    testLimit("one shot", {"A", "X"}, {{"A", {"B"}}}, {{"B", 1}}, 1, {"X"});            // B dropped, A never ready
}
