#include <cassert>
#include <unordered_set>

using std::unordered_set;

class StreamFilter {
private:
    unordered_set<int> banned;
    unordered_set<int> seen;

public:
    // T: O(1), S: O(B+S)
    bool process(int x) {
        seen.insert(x);
        return !banned.count(x);
    }

    // T: O(1)
    bool ban(int x) {
        bool added = banned.insert(x).second;
        return added && seen.count(x);
    }

    // T: O(1)
    bool unban(int x) {
        bool removed = banned.erase(x) > 0;
        return removed && seen.count(x);
    }
};

int main() {
    StreamFilter f;

    assert(f.process(1));
    assert(f.process(1)); // Emit repeated non-banned occurrences.
    assert(f.ban(1));     // Seen value becomes hidden.
    assert(!f.ban(1));    // Repeated ban changes nothing.
    assert(!f.process(1));

    assert(!f.ban(2));    // Unseen value: no visibility event.
    assert(!f.process(2));// Still record that 2 was seen.
    assert(f.unban(2));   // Seen value becomes visible.
    assert(!f.unban(2));  // Repeated unban changes nothing.
    assert(f.process(2));

    assert(!f.ban(3));
    assert(!f.unban(3));  // Still unseen: no event.
}
