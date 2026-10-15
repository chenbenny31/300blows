// A19 (cf. LC 3408): addTask(taskId, priority, timestamp) may repeat an id;
// executeTask() runs the best pending record (higher priority, then earlier timestamp,
// then smaller id) and returns its id, -1 if none. An executed id never runs again.
#include <iostream>
#include <string>
#include <queue>
#include <tuple>
#include <unordered_set>
using std::cout;
using std::string;
using std::priority_queue;
using std::tuple;
using std::get;
using std::unordered_set;

class TaskExecutor {
    // (priority, -timestamp, -taskId) per added record; negation makes earlier / smaller
    // win in a max-heap, so no custom comparator
    priority_queue<tuple<int, int, int>> heap;
    unordered_set<int> done;   // ids that have been executed

public:
    // T: O(log R); R = records added.
    void addTask(int taskId, int priority, int timestamp) {
        if (done.count(taskId)) { return; }
        heap.push({priority, -timestamp, -taskId});
    }

    // Max-heap with lazy skip of stale records. T: O(log R) amortized, S: O(R + X).
    int executeTask() {
        // while, not if: several stale records may sit on top
        while (!heap.empty()) {
            int taskId = -get<2>(heap.top());
            heap.pop();
            // older duplicate record of an id that already ran
            if (done.count(taskId)) { continue; }
            done.insert(taskId);
            return taskId;
        }
        return -1;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TaskExecutor ex;
    check(ex.executeTask() == -1, "empty");
    ex.addTask(7, 1, 1);
    ex.addTask(8, 9, 2);
    ex.addTask(7, 9, 2);
    ex.addTask(6, 9, 1);
    ex.addTask(5, 5, 0);
    check(ex.executeTask() == 6, "earlier timestamp");
    check(ex.executeTask() == 7, "smaller id on full tie");
    check(ex.executeTask() == 8 && ex.executeTask() == 5, "priority order");
    check(ex.executeTask() == -1, "stale record skipped");
    ex.addTask(7, 100, 3);
    check(ex.executeTask() == -1, "executed id ignored");
}
