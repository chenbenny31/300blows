#include <cassert>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using std::pair;
using std::priority_queue;
using std::unordered_map;
using std::vector;

class TaskManager {
private:
    unordered_map<int, pair<int, int>> tasks; // task-id -> {user-id, priority}
    priority_queue<pair<int, int>> heap; // {priority, task-id}

public:
    // T: O(nlogn), S: O(n)
    TaskManager(const vector<vector<int>>& initials) {
        for (const auto& task : initials) {
            add(task[0], task[1], task[2]);
        }
    }

    // T: O(logn)
    void add(int userId, int taskId, int priority) {
        tasks[taskId] = {userId, priority};
        heap.push({priority, taskId});
    }

    // T: O(log n).
    void edit(int taskId, int newPriority) {
        auto it = tasks.find(taskId);
        if (it == tasks.end()) {
            return;
        }
        it->second.second = newPriority;
        heap.push({newPriority, taskId});
    }

    // T: O(1) expected.
    void rmv(int taskId) {
        tasks.erase(taskId);
    }

    // T: O(n log n) worst case; each heap entry is removed only once.
    int execTop() {
        while (!heap.empty()) {
            auto [priority, taskId] = heap.top();
            heap.pop();

            auto it = tasks.find(taskId);
            if (it == tasks.end() || it->second.second != priority) {
                continue; // Removed task or outdated priority.
            }

            int userId = it->second.first;
            tasks.erase(it);
            return userId;
        }
        return -1;
    }
};

int main() {
    TaskManager manager({{1, 101, 10}, {2, 102, 20}, {3, 103, 10}});

    manager.edit(102, 5);
    assert(manager.execTop() == 3); // Skip stale priority 20; larger ID wins tie.

    manager.rmv(101);
    assert(manager.execTop() == 2); // Skip removed task 101.

    manager.add(4, 104, 7);
    assert(manager.execTop() == 4);
    assert(manager.execTop() == -1); // Empty.
}
