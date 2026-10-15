// LC 855: n seats in a row. seat() picks the seat farthest from the closest student
// (ties -> lowest index; seat 0 when the room is empty); leave(p) frees seat p.
#include <iostream>
#include <string>
#include <set>
using std::cout;
using std::string;
using std::set;

class ExamRoom {
    int n;            // seats 0..n-1
    set<int> taken;   // occupied seats, sorted

public:
    ExamRoom(int seats) : n(seats) {}

    // scan the left end, every gap between neighbors, then the right end.
    // T: O(p), p = students seated.
    int seat() {
        int best = 0;
        if (!taken.empty()) {
            // sitting at 0: distance to the first student
            int bestDist = *taken.begin();
            int prev = *taken.begin();
            for (int s : taken) {
                // strict > keeps the lowest index on ties
                if ((s - prev) / 2 > bestDist) {
                    bestDist = (s - prev) / 2;
                    best = prev + bestDist;
                }
                prev = s;
            }
            if (n - 1 - prev > bestDist) { best = n - 1; }
        }
        taken.insert(best);
        return best;
    }

    // T: O(log p).
    void leave(int p) { taken.erase(p); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ExamRoom room(10);
    check(room.seat() == 0, "empty room");
    check(room.seat() == 9, "right end");
    check(room.seat() == 4 && room.seat() == 2, "middle gaps");
    room.leave(4);
    check(room.seat() == 5, "after leave");
}
