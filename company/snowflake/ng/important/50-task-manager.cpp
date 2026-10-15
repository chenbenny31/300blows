// LC 3408: TaskManager(tasks [userId, taskId, priority]) with add, edit, rmv(taskId),
// and execTop(): remove the highest-priority task (ties -> larger taskId) and return its
// userId, -1 if there are no tasks.
// Follow-up: stale heap entries pile up with edits -> ordered set, true O(log n) removal.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::set;
using std::unordered_map;

// Lazy deletion: edit / rmv leave stale heap entries that execTop skips.
// T: add / edit O(log M), rmv O(1), execTop O(log M) amortized; M = heap entries.
class TaskManager {
    unordered_map<int, pair<int, int>> tasks;   // taskId -> (userId, current priority)
    // (priority, taskId): max-heap top = next to run; may hold stale entries
    priority_queue<pair<int, int>> heap;

public:
    TaskManager(const vector<vector<int>>& initial) {
        for (const auto& t : initial) { add(t[0], t[1], t[2]); }
    }

    void add(int userId, int taskId, int priority) {
        tasks[taskId] = {userId, priority};
        heap.push({priority, taskId});
    }

    void edit(int taskId, int newPriority) {
        tasks[taskId].second = newPriority;
        heap.push({newPriority, taskId});   // the old entry goes stale
    }

    void rmv(int taskId) { tasks.erase(taskId); }

    int execTop() {
        while (!heap.empty()) {
            auto [priority, taskId] = heap.top();
            heap.pop();
            auto it = tasks.find(taskId);
            // stale: task removed, or this entry holds an old priority
            if (it == tasks.end() || it->second.second != priority) { continue; }
            int userId = it->second.first;
            tasks.erase(it);
            return userId;
        }
        return -1;
    }
};

// follow-up: ordered set, every entry is live. T: O(log n) for every operation.
class TaskManagerSet {
    unordered_map<int, pair<int, int>> info;   // taskId -> (userId, priority)
    set<pair<int, int>> order;                 // (priority, taskId); last = next to run

public:
    TaskManagerSet(const vector<vector<int>>& initial) {
        for (const auto& t : initial) { add(t[0], t[1], t[2]); }
    }

    void add(int userId, int taskId, int priority) {
        info[taskId] = {userId, priority};
        order.insert({priority, taskId});
    }

    void edit(int taskId, int newPriority) {
        int userId = info[taskId].first;
        rmv(taskId);
        add(userId, taskId, newPriority);
    }

    void rmv(int taskId) {
        order.erase({info[taskId].second, taskId});
        info.erase(taskId);
    }

    int execTop() {
        if (order.empty()) { return -1; }
        int taskId = order.rbegin()->second;
        int userId = info[taskId].first;
        rmv(taskId);
        return userId;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TaskManager tm({{1, 101, 10}, {2, 102, 20}, {3, 103, 15}});
    tm.add(4, 104, 5);
    tm.edit(102, 8);
    check(tm.execTop() == 3, "skip stale edit");
    tm.rmv(101);
    tm.add(5, 105, 15);
    check(tm.execTop() == 5, "skip removed");
    check(tm.execTop() == 2 && tm.execTop() == 4 && tm.execTop() == -1, "drain");
    TaskManager tie({{1, 7, 3}, {2, 9, 3}});
    check(tie.execTop() == 2, "tie by taskId");
}
