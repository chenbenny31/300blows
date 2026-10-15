// A17 (prototype LC 207/210): discover every course reachable from the entries via prevCourses, then take courses
// Kahn-style; hasFailed(course) true -> retry it later (back of the queue), false -> record it and unlock dependents.

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
using std::unordered_map;
using std::unordered_set;

class CourseRetryScheduler {
public:
    // BFS discovery + deduped edges + Kahn's queue with requeue on failure, T: O(V + R + F), S: O(V + E)
    vector<string> completionOrder(const vector<string>& entries,
                                   const unordered_map<string, vector<string>>& prevCourses,
                                   const std::function<bool(const string&)>& hasFailed) {
        return schedule(entries, prevCourses, hasFailed, INT_MAX);
    }

    // same, but a course that fails maxAttempts times is dropped and its dependents never run, T/S as above
    vector<string> completionOrderWithLimit(const vector<string>& entries,
                                            const unordered_map<string, vector<string>>& prevCourses,
                                            const std::function<bool(const string&)>& hasFailed, int maxAttempts) {
        return schedule(entries, prevCourses, hasFailed, maxAttempts);
    }

private:
    vector<string> schedule(const vector<string>& entries,
                            const unordered_map<string, vector<string>>& prevCourses,
                            const std::function<bool(const string&)>& hasFailed, int maxAttempts) {
        vector<string> courses;          // every reachable course, in discovery order
        unordered_set<string> seen;      // courses already in `courses`
        std::queue<string> frontier;     // discovered courses whose prereqs are not yet explored
        for (const auto& e : entries) {
            if (seen.insert(e).second) {
                courses.push_back(e);
                frontier.push(e);
            }
        }
        while (!frontier.empty()) {
            string c = frontier.front();
            frontier.pop();
            auto it = prevCourses.find(c);
            if (it == prevCourses.end()) { continue; }   // not a key: no prereqs
            for (const auto& p : it->second) {
                if (seen.insert(p).second) {
                    courses.push_back(p);
                    frontier.push(p);
                }
            }
        }

        unordered_map<string, vector<string>> dependents;   // course -> courses that list it as a prereq
        unordered_map<string, int> waiting;                 // course -> distinct prereqs not yet passed
        for (const auto& c : courses) {
            waiting[c] = 0;
            auto it = prevCourses.find(c);
            if (it == prevCourses.end()) { continue; }
            unordered_set<string> linked;   // prereqs of c already turned into an edge
            for (const auto& p : it->second) {
                if (!linked.insert(p).second) { continue; }   // duplicate reference: one edge only
                dependents[p].push_back(c);
                waiting[c]++;
            }
        }

        std::queue<string> ready;            // courses with all prereqs passed, in try order
        for (const auto& c : courses) {
            if (waiting[c] == 0) { ready.push(c); }
        }
        unordered_map<string, int> failures; // course -> failed attempts so far
        vector<string> passed;
        while (!ready.empty()) {
            string c = ready.front();
            ready.pop();
            if (hasFailed(c)) {
                failures[c]++;
                if (failures[c] < maxAttempts) { ready.push(c); }   // retry after everything already waiting
                continue;
            }
            passed.push_back(c);
            auto it = dependents.find(c);
            if (it == dependents.end()) { continue; }
            for (const auto& d : it->second) {
                waiting[d]--;
                if (waiting[d] == 0) { ready.push(d); }
            }
        }
        return passed;
    }
};

bool runCase(const string& label, const vector<string>& entries, const unordered_map<string, vector<string>>& prev,
             const unordered_map<string, int>& failScript, int maxAttempts,
             const vector<string>& expected, int expectedCalls) {
    unordered_map<string, int> failsLeft = failScript;   // course -> failures still scripted before it passes
    int calls = 0;
    auto hasFailed = [&failsLeft, &calls](const string& course) {
        calls++;
        auto it = failsLeft.find(course);
        if (it == failsLeft.end() || it->second == 0) { return false; }
        it->second--;
        return true;
    };
    CourseRetryScheduler sched;
    vector<string> got = maxAttempts == INT_MAX ? sched.completionOrder(entries, prev, hasFailed)
                                                : sched.completionOrderWithLimit(entries, prev, hasFailed, maxAttempts);
    if (got == expected && calls == expectedCalls) { return true; }
    cout << "FAIL " << label << ": got";
    for (const auto& c : got) { cout << " " << c; }
    cout << " (" << calls << " calls), expected";
    for (const auto& c : expected) { cout << " " << c; }
    cout << " (" << expectedCalls << " calls)\n";
    return false;
}

bool test(const string& label, const vector<string>& entries, const unordered_map<string, vector<string>>& prev,
          const unordered_map<string, int>& failScript, const vector<string>& expected, int expectedCalls) {
    return runCase(label, entries, prev, failScript, INT_MAX, expected, expectedCalls);
}

bool testLimit(const string& label, const vector<string>& entries, const unordered_map<string, vector<string>>& prev,
               const unordered_map<string, int>& failScript, int maxAttempts,
               const vector<string>& expected, int expectedCalls) {
    return runCase("limit " + label, entries, prev, failScript, maxAttempts, expected, expectedCalls);
}

int main() {
    bool ok = true;
    ok &= test("chain", {"C"}, {{"C", {"B"}}, {"B", {"A"}}}, {}, {"A", "B", "C"}, 3);            // discovery walks back to A
    ok &= test("empty", {}, {}, {}, {}, 0);                                                         // no entry courses
    ok &= test("requeue", {"X", "Y"}, {}, {{"X", 1}}, {"Y", "X"}, 3);                               // failed course goes behind Y
    ok &= test("diamond", {"D"}, {{"D", {"B", "C"}}, {"B", {"A"}}, {"C", {"A"}}},
               {{"B", 2}}, {"A", "C", "B", "D"}, 6);                                                // D waits for the retried B
    ok &= test("dup refs", {"B", "B", "A"}, {{"B", {"A", "A"}}}, {}, {"A", "B"}, 2);                // double-counted edge would block B
    ok &= test("prereq-only", {"A", "C"}, {{"C", {"A", "B"}}}, {{"A", 1}}, {"B", "A", "C"}, 4);     // B is not a map key
    ok &= test("unreachable", {"A"}, {{"Z", {"Y"}}}, {}, {"A"}, 1);                                 // Z, Y never discovered or tried
    ok &= testLimit("blocked chain", {"C"}, {{"C", {"B"}}, {"B", {"A"}}}, {{"A", 5}}, 3, {}, 3);    // A dropped after 3 tries
    ok &= testLimit("retry within", {"A", "X"}, {{"A", {"B"}}}, {{"B", 1}}, 2, {"X", "B", "A"}, 4); // 1 failure < 2 attempts
    ok &= testLimit("one shot", {"A", "X"}, {{"A", {"B"}}}, {{"B", 1}}, 1, {"X"}, 2);               // B dropped, A never ready
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
