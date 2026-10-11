// Design add(userId, taskId, priority), edit(taskId, newPriority), rmv(taskId), execTop()
// execTop runs the highest priority task (tie: larger taskId), removes it, returns its userId or -1

// active taskId unique? edit/rmv on a missing id?
// priority direction, larger runs first?
// tie-break on equal priority?
// execTop return when no task left?

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <cassert>
#include <iostream>

using std::vector;
using std::pair;

// T: add/edit O(logM), rmv O(1), execTop amortized O(logM), M: maxHeap entries incl. stale
// S: O(T + M), T: active tasks

class TaskManager {
private:
    std::unordered_map<int, pair<int, int>> tasks; // taskId -> (userId, current priority), active tasks
    std::priority_queue<pair<int, int>> maxHeap; // (priority, taskId), max-maxHeap, stale old task, all tasks

public:
    TaskManager(const vector<vector<int>>& tasks) {
        for (const auto& t : tasks) {
            add(t[0], t[1], t[2]);
        }
    }

    void add(int userId, int taskId, int priority) {
        tasks[taskId] = {userId, priority};
        maxHeap.push({priority, taskId});
    }

    void edit(int taskId, int newPriority) {
        auto it = tasks.find(taskId);
        if (it == tasks.end()) {
            return;
        }
        tasks[taskId].second = newPriority;
        maxHeap.push({newPriority, taskId}); // old entry goes stale, skipped on pop
    }

    void rmv(int taskId) {
        tasks.erase(taskId); // its maxHeap entries go stale
    }

    int execTop() {
        while (!maxHeap.empty()) {
            auto [priority, taskId] = maxHeap.top();
            maxHeap.pop();
            auto it = tasks.find(taskId);
            if (it != tasks.end() && it->second.second == priority) { // still active and current priority
                int userId = it->second.first;
                tasks.erase(it);
                return userId;
            }
        }
        return -1;
    }
};

int main() {
    TaskManager tm({{1, 101, 10}, {2, 102, 20}, {3, 103, 15}});
    tm.add(4, 104, 5);
    tm.edit(102, 8);
    assert(tm.execTop() == 3); // 103 at 15, stale (20, 102) skipped first
    tm.rmv(101);
    tm.add(5, 105, 15);
    assert(tm.execTop() == 5);

    assert(tm.execTop() == 2); // 102 at 8
    assert(tm.execTop() == 4); // 104 at 5
    assert(tm.execTop() == -1); // only stale entries left

    TaskManager tie({{1, 7, 3}, {2, 9, 3}});
    assert(tie.execTop() == 2); // same priority, larger taskId first

    std::cout << "all tests passed\n";
    return 0;
}
