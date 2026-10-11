// Design addTask(taskId, priority, timestamp), executeTask()
// same taskId can be added many times, executeTask runs the highest priority record and returns its id
// an id runs at most once ever, later records of it are skipped, -1 if nothing runnable

// priority direction, larger runs first?
// tie-break on equal priority, earlier timestamp then earlier add?
// timestamp a sort key or just recorded?
// duplicate add a new record or overwrite the old priority?
// execution can fail? failure counts as done?
// re-add an done id, ignore or error?

#include <vector>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <cassert>
#include <iostream>

// T: addTask O(logR), executeTask amortized O(logR), R: records added
// S: O(R + X), X: done ids

class TaskExecutor {
private:
    std::priority_queue<std::tuple<int, long long, int, int>> maxHeap; // (priority, -ts, -seq, taskId)
    std::unordered_set<int> done; // ids that ran, permanent
    int seq = 0; // add order, last tie-break so the order is total

public:
    void addTask(int taskId, int priority, long long timestamp) {
        if (done.count(taskId) != 0) {
            return; // already ran, never runs again
        }
        maxHeap.push({priority, -timestamp, -seq, taskId}); // negate so max-maxHeap prefers earlier ts, earlier add
        seq++;
    }

    int executeTask() {
        while (!maxHeap.empty()) {
            int taskId = std::get<3>(maxHeap.top());
            maxHeap.pop();
            if (done.count(taskId) != 0) {
                continue; // duplicate record of an id that already ran
            }
            done.insert(taskId);
            return taskId;
        }
        return -1;
    }
};

int main() {
    TaskExecutor ex;
    assert(ex.executeTask() == -1); // empty

    ex.addTask(1, 5, 10);
    ex.addTask(2, 9, 11);
    ex.addTask(1, 9, 12); // duplicate id, higher priority record
    ex.addTask(3, 9, 11); // ties 2 on priority and ts, added later

    assert(ex.executeTask() == 2); // priority 9, ts 11, added first
    assert(ex.executeTask() == 3); // same priority and ts, added later
    assert(ex.executeTask() == 1); // priority 9 record of the duplicate id
    assert(ex.executeTask() == -1); // only (1, 5) left, id 1 already ran

    ex.addTask(1, 100, 20); // done id never runs again
    assert(ex.executeTask() == -1);

    std::cout << "all tests passed\n";
    return 0;
}
