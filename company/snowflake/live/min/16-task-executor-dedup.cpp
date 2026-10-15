// A19 · no direct LC (cf. LC 3408): addTask(id, priority, ts) may repeat an id; executeTask() runs the best record (priority desc, ts asc, id asc), each id once, -1 if none.
#include <iostream>
#include <string>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::priority_queue;
using std::set;
using std::tuple;
using std::unordered_map;
using std::unordered_set;

class TaskExecutor {
public:
    // T: O(log R)
    void addTask(int taskId, int priority, int timestamp) {
        if (done.count(taskId)) { return; }      // executed ids are never queued again
        heap.push({priority, -timestamp, -taskId});
    }

    // max-heap with lazy skip of stale records, T: O(log R) amortized, S: O(R + X)
    int executeTask() {
        while (!heap.empty()) {                  // while, not if: may pop several stale records
            int taskId = -std::get<2>(heap.top());
            heap.pop();
            if (done.count(taskId)) { continue; }   // older duplicate of an id that already ran
            done.insert(taskId);
            return taskId;
        }
        return -1;
    }

private:
    priority_queue<tuple<int, int, int>> heap;   // (priority, -timestamp, -taskId) per record, best on top
    unordered_set<int> done;                     // ids already executed
};

// follow-up: one pending record per id in an ordered set, no heap bloat, T: O(log P), S: O(P + X)
class CompactTaskExecutor {
public:
    // keep only each id's best pending record, T: O(log P)
    void addTask(int taskId, int priority, int timestamp) {
        if (done.count(taskId)) { return; }
        tuple<int, int, int> key = {-priority, timestamp, taskId};
        auto it = bestKey.find(taskId);
        if (it != bestKey.end()) {
            if (it->second <= key) { return; }   // pending record already runs first
            pending.erase(it->second);
        }
        pending.insert(key);
        bestKey[taskId] = key;
    }

    // smallest key is best, T: O(log P)
    int executeTask() {
        if (pending.empty()) { return -1; }
        int taskId = std::get<2>(*pending.begin());
        pending.erase(pending.begin());
        bestKey.erase(taskId);
        done.insert(taskId);
        return taskId;
    }

private:
    set<tuple<int, int, int>> pending;                  // (-priority, timestamp, taskId), one per pending id
    unordered_map<int, tuple<int, int, int>> bestKey;   // pending id -> its key in pending
    unordered_set<int> done;                            // ids already executed
};

void check(int got, int expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << " got " << got << "\n";
}

int main() {
    TaskExecutor ex;
    check(ex.executeTask(), -1, "empty executor");
    ex.addTask(1, 5, 10);
    ex.addTask(3, 9, 11);
    ex.addTask(2, 9, 11);                        // full tie with id 3, added later
    ex.addTask(4, 9, 5);
    check(ex.executeTask(), 4, "priority 9, earliest timestamp");
    check(ex.executeTask(), 2, "full tie -> smaller id, not insertion order");
    ex.addTask(7, 1, 1);
    ex.addTask(7, 10, 2);                        // better record for the same id
    check(ex.executeTask(), 7, "best record of id 7");
    check(ex.executeTask(), 3, "remaining priority 9");
    check(ex.executeTask(), 1, "priority 5");
    check(ex.executeTask(), -1, "stale (7, priority 1) skipped");
    ex.addTask(7, 100, 4);
    check(ex.executeTask(), -1, "re-added executed id ignored");

    CompactTaskExecutor cx;
    cx.addTask(7, 1, 1);
    cx.addTask(7, 10, 2);                        // replaces the priority-1 record
    cx.addTask(7, 3, 0);                         // worse than the kept record, ignored
    cx.addTask(3, 10, 2);
    check(cx.executeTask(), 3, "compact full tie -> smaller id");
    check(cx.executeTask(), 7, "compact best record of id 7");
    check(cx.executeTask(), -1, "compact no stale records left");
    cx.addTask(7, 100, 4);
    check(cx.executeTask(), -1, "compact re-added executed id ignored");
}
