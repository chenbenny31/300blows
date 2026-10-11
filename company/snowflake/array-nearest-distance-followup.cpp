#include <cassert>
#include <vector>

using std::vector;

// T: O(n), S: O(1)
int closestPair(const vector<int>& a) {
    int n = a.size();
    int last1 = -1, last2 = -1;
    int minDist = -1;

    for (int i = 0; i < n; i++) {
        int dist = -1;

        if (a[i] == 1) {
            last1 = i;
            if (last2 != -1) { dist = i - last2; }
        } else if (a[i] == 2) {
            last2 = i;
            if (last1 != -1) { dist = i - last1; }
        }

        if (dist != -1 && (minDist == -1 || dist < minDist)) {
            minDist = dist;
        }
    }
    return minDist;
}

int main() {
    assert(closestPair({1, 0, 0, 2, 0, 1}) == 2);

    // {1, 1, 0, 2}: 2
    // {2, 0, 1}: 2
    // {1, 2}: 1
    // {1, 0, 1}: -1
    // {2, 0, 2}: -1
    // {}: -1

}
