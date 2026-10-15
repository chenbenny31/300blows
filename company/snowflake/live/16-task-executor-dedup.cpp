// A19 · no direct LC (cf. LC 3408): addTask(taskId, priority, timestamp) may add the same id many times; executeTask() runs the best
// pending record (higher priority, then earlier timestamp, then smaller id) and returns its id; an executed id never runs again; -1 if none.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <set>
#include <tuple>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::priority_queue;
using std::set;
using std::tuple;

class TaskExecutor {
public:
    // T: O(log R)
    void addTask(int taskId, int priority, int timestamp) {
        if (done.count(taskId) != 0) { return; }   // executed ids are never queued again
        heap.push({priority, -timestamp, -taskId});
    }

    // max-heap with lazy skip of stale records, T: O(log R) amortized, S: O(R + X)
    int executeTask() {
        while (!heap.empty()) {
            int taskId = -std::get<2>(heap.top());
            heap.pop();
            if (done.count(taskId) != 0) { continue; }   // older duplicate of an id that already ran
            done.insert(taskId);
            return taskId;
        }
        return -1;
    }

private:
    priority_queue<tuple<int, int, int>> heap;   // (priority, -timestamp, -taskId) per added record, best on top
    unordered_set<int> done;                     // ids that have been executed
};

class CompactTaskExecutor {
public:
    // keep only each id's best pending record, T: O(log P)
    void addTask(int taskId, int priority, int timestamp) {
        if (done.count(taskId) != 0) { return; }
        tuple<int, int, int> key = {-priority, timestamp, taskId};
        auto it = bestKey.find(taskId);
        if (it != bestKey.end()) {
            if (it->second <= key) { return; }   // the pending record already runs first
            pending.erase(it->second);
        }
        pending.insert(key);
        bestKey[taskId] = key;
    }

    // ordered set, no stale entries, T: O(log P), S: O(P + X)
    int executeTask() {
        if (pending.empty()) { return -1; }
        int taskId = std::get<2>(*pending.begin());
        pending.erase(pending.begin());
        bestKey.erase(taskId);
        done.insert(taskId);
        return taskId;
    }

private:
    set<tuple<int, int, int>> pending;                  // (-priority, timestamp, taskId), one per pending id, best first
    unordered_map<int, tuple<int, int, int>> bestKey;   // pending id -> its key in pending
    unordered_set<int> done;                            // ids that have been executed
};

bool check(int got, int expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool testOrdering() {
    bool ok = true;
    TaskExecutor ex;
    ok &= check(ex.executeTask(), -1, "empty executor");
    ex.addTask(1, 5, 10);
    ex.addTask(3, 9, 11);
    ex.addTask(2, 9, 11);                           // ties id 3 on priority and timestamp, added later
    ex.addTask(4, 9, 5);
    ex.addTask(5, -3, 0);
    ok &= check(ex.executeTask(), 4, "priority 9, earliest timestamp");
    ok &= check(ex.executeTask(), 2, "full tie -> smaller id, not insertion order");
    ok &= check(ex.executeTask(), 3, "remaining priority 9");
    ok &= check(ex.executeTask(), 1, "priority 5");
    ok &= check(ex.executeTask(), 5, "negative priority runs last");
    ok &= check(ex.executeTask(), -1, "drained");
    return ok;
}

bool testDuplicates() {
    bool ok = true;
    TaskExecutor ex;
    ex.addTask(7, 1, 1);
    ex.addTask(7, 10, 2);                           // better record for the same id
    ex.addTask(8, 5, 3);
    ok &= check(ex.executeTask(), 7, "best record of id 7");
    ok &= check(ex.executeTask(), 8, "next id");
    ok &= check(ex.executeTask(), -1, "stale (7, priority 1) skipped");
    ex.addTask(7, 100, 4);
    ok &= check(ex.executeTask(), -1, "re-added executed id ignored");
    ex.addTask(9, 0, 5);
    ok &= check(ex.executeTask(), 9, "new id after re-add");
    return ok;
}

bool testCompact() {
    bool ok = true;
    CompactTaskExecutor ex;
    ok &= check(ex.executeTask(), -1, "compact empty");
    ex.addTask(7, 1, 1);
    ex.addTask(7, 10, 2);                           // replaces the priority-1 record
    ex.addTask(7, 3, 0);                            // worse than the kept record, ignored
    ex.addTask(3, 10, 2);
    ex.addTask(8, 5, 3);
    ok &= check(ex.executeTask(), 3, "compact full tie -> smaller id");
    ok &= check(ex.executeTask(), 7, "compact best record of id 7");
    ok &= check(ex.executeTask(), 8, "compact next id");
    ok &= check(ex.executeTask(), -1, "compact no stale records left");
    ex.addTask(7, 100, 4);
    ok &= check(ex.executeTask(), -1, "compact re-added executed id ignored");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testOrdering();     // empty, priority / timestamp / id tie-breaks, negative priority
    ok &= testDuplicates();   // duplicate ids, stale records, re-add after execution
    ok &= testCompact();      // follow-up: one pending record per id, same contract
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
