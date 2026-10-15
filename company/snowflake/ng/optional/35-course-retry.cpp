// A17: discover every course reachable from the entries via prevCourses (course ->
// its prereqs), then take them Kahn-style; a failed course retries at the back.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;

// Fake for the real API: a course fails while it has scripted failures left.
bool hasFailed(const string& course, unordered_map<string, int>& failsLeft) {
    if (failsLeft[course] == 0) { return false; }
    failsLeft[course]--;
    return true;
}

// BFS discovery with deduped edges, then Kahn's queue; assumes no cycle.
// T: O(V + R + F), S: O(V + E); R = prereq references, F = total failures.
vector<string> completionOrder(const vector<string>& entries,
                               const unordered_map<string, vector<string>>& prevCourses,
                               unordered_map<string, int> failsLeft) {
    vector<string> courses;                        // reachable courses, discovery order
    unordered_set<string> seen;                    // courses already in `courses`
    unordered_map<string, vector<string>> graph;   // graph[p] = courses that need p
    unordered_map<string, int> indeg;              // indeg[c] = c's prereqs not passed
    for (const string& e : entries) {
        if (seen.insert(e).second) { courses.push_back(e); }
    }
    // courses grows while we scan it, so it doubles as the BFS queue
    for (int i = 0; i < (int)courses.size(); i++) {
        string cur = courses[i];   // copy: push_back below may reallocate
        auto it = prevCourses.find(cur);
        if (it == prevCourses.end()) { continue; }   // not a key: no prereqs
        unordered_set<string> linked;                // prereqs of cur already an edge
        for (const string& nxt : it->second) {
            if (!linked.insert(nxt).second) { continue; }
            graph[nxt].push_back(cur);
            indeg[cur]++;
            if (seen.insert(nxt).second) { courses.push_back(nxt); }
        }
    }
    queue<string> q;   // courses whose prereqs all passed, in try order
    for (const string& c : courses) {
        if (indeg[c] == 0) { q.push(c); }
    }
    vector<string> order;   // courses in the order they passed
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        if (hasFailed(cur, failsLeft)) {
            q.push(cur);   // retry after everything already waiting
            continue;
        }
        order.push_back(cur);
        for (const string& nxt : graph[cur]) {
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return order;
}

void test(const string& label, const vector<string>& entries,
          const unordered_map<string, vector<string>>& prevCourses,
          const unordered_map<string, int>& failsLeft, const vector<string>& want) {
    auto got = completionOrder(entries, prevCourses, failsLeft);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond retry", {"D"}, {{"D", {"B", "C"}}, {"B", {"A"}}, {"C", {"A"}}},
         {{"B", 1}}, {"A", "C", "B", "D"});
    test("duplicate refs", {"B", "B"}, {{"B", {"A", "A"}}}, {}, {"A", "B"});
    test("requeue, unreachable", {"X", "Y"}, {{"Z", {"X"}}}, {{"X", 1}}, {"Y", "X"});
}
