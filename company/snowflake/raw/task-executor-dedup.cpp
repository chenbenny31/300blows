#include <cassert>
#include <queue>
#include <unordered_set>
#include <vector>

using std::vector;

struct Task {
    int id;
    int priority;
    long long timestamp;
    long long seq;
};

struct TaskCmp { // larger pri > smaller ts > samller seq
    bool operator()(const Task& a, const Task& b) const {
        if (a.priority != b.priority) { return a.priority < b.priority; }
        if (a.timestamp != b.timestamp) { return a.timestamp > b.timestamp; }
        return a.seq > b.seq;
    }
};

auto cmp = [](const Task& a, const Task& b) {
    if (a.priority != b.priority) { return a.priority < b.priority; }
    if (a.timestamp != b.timestamp) { return a.timestamp > b.timestamp; }
    return a.seq > b.seq;
};

class TaskExecutor {
private:
    std::priority_queue<Task, vector<Task>, TaskCmp> heap; // or decltype(cmp) + heap(cmp)
    std::unordered_set<int> done;
    long long seq = 0;

public:
    // T: O(logn), S: O(n), n: total storage
    void addTask(int id, int priority, long long timestamp) {
        if (done.count(id)) { return; }
        heap.push({id, priority, timestamp, seq});
        seq++;
    }

    // T: O(nlogn) worst case, S: O(1)
    int executeTask() {
        while (!heap.empty()) {
            int id = heap.top().id;
            heap.pop();

            if (done.insert(id).second) { return id; }
        }
        return -1;
    }
};

int main() {
    TaskExecutor exec;
    exec.addTask(1, 5, 10);
    exec.addTask(2, 9, 11);
    exec.addTask(1, 9, 12); // same id, higher-priority

    assert(exec.executeTask() == 2); // smaller ts
    assert(exec.executeTask() == 1); // exec 1 once
    assert(exec.executeTask() == -1); // skip dup2

    // re-added done: -1
    // all tie: by-seq
    // empty: -1
}
