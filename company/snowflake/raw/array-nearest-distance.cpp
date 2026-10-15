#include <cassert>
#include <vector>

using std::vector;

// T: O(n), S: O(n)
vector<int> nearestTwo(const vector<int>& a) {
    int n = a.size();
    vector<int> dist(n, -1);
    int last = -1; // current nearest index of 2

    // closest 2 to the left
    for (int i = 0; i < n; i++) {
        if (a[i] == 2) {
            last = i;
        } else if (a[i] == 1 && last != -1) {
            dist[i] = i - last;
        }
    }

    // closest 2 to the right
    last = -1;
    for (int i = n - 1; i >= 0; i--) {
        if (a[i] == 2) {
            last = i;
        } else if (a[i] == 1 && last != -1) {
            if (dist[i] == -1 || last - i < dist[i]) {
                dist[i] = last - i;
            }
        }
    }

    vector<int> result;
    for (int i = 0; i < n; i++) {
        if (a[i] == 1) { result.push_back(dist[i]); }
    }
    return result;
}

int main() {
    vector<int> a = {1, 0, 0, 2, 1, 0, 0, 2, 1};
    vector<int> want = {3, 1, 1};

    assert(nearestTwo(a) == want);
    // (1, 0, 2, 0, 1, 1): {2, 2, 3}
    // (2, 0, 0, 1, 2): {1}
    // (2, 0, 1, 0, 2): {2}
    // (1, 1, 2): {2, 1}
    // (1, 0, 1), no 2: {-1, -1}
    // (2, 0, 2), no 1: {}
}
