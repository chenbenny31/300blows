// LC 3408: TaskManager(tasks [userId, taskId, priority]) with add, edit, rmv(taskId),
// and execTop(): remove the highest-priority task (ties -> higher taskId) and return its
// userId, -1 if there are no tasks.
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::set;
using std::unordered_map;

class TaskManager {
    unordered_map<int, pair<int, int>> info;   // taskId -> (userId, priority)
    set<pair<int, int>> order;                 // (priority, taskId); last = next to run

public:
    // T: O(n log n).
    TaskManager(const vector<vector<int>>& tasks) {
        for (const auto& t : tasks) { add(t[0], t[1], t[2]); }
    }

    // T: O(log n) for every operation.
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
        // pair order: the largest is max priority, ties -> max taskId
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
    check(tm.execTop() == 3, "after edit");
    tm.rmv(101);
    tm.add(5, 105, 15);
    check(tm.execTop() == 5, "after rmv");
    check(tm.execTop() == 2 && tm.execTop() == 4 && tm.execTop() == -1, "drain");
    TaskManager tie({{1, 1, 5}, {2, 2, 5}});
    check(tie.execTop() == 2, "tie by taskId");
}
