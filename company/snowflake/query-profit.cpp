#include <cassert>
#include <utility>
#include <vector>

using std::pair;
using std::vector;

// T: O(n), S: O(1)
pair<long long, int> maxProfit(const vector<int>& time,
                               const vector<int>& profits, long long K) {
    long long bestProfit = 0;
    int bestIdx = -1;

    for (int i = 0; i < static_cast<int>(time.size()); i++) {
        if (time[i] > K) { continue; }
        
        long long total = (K / time[i]) * profits[i];
        if (bestIdx == -1 || total > bestProfit) {
            bestProfit = total;
            bestIdx = i;
        }
    }
    return {bestProfit, bestIdx};
}

int main() {
    auto answer = maxProfit({3, 5}, {10, 15}, 5);
    assert(answer.first == 15 && answer.second == 1);
    // Better ratio loses because of unused time.

    answer = maxProfit({2, 4}, {2, 4}, 4);
    assert(answer.first == 4 && answer.second == 0); // Tie: first index.

    answer = maxProfit({1}, {0}, 3);
    assert(answer.first == 0 && answer.second == 0); // Fits but earns zero.

    answer = maxProfit({6}, {9}, 5);
    assert(answer.first == 0 && answer.second == -1); // Nothing fits.

    answer = maxProfit({}, {}, 5);
    assert(answer.first == 0 && answer.second == -1); // No query types.
}
