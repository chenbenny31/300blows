// A12 (cf. LC 692): recordSearch(userId, word); getTopK(k) = the k words with the most
// distinct users, ties by word ascending. A user repeating a word counts once.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::sort;
using std::reverse;
using std::priority_queue;

class SearchRanker {
    // users[word] = distinct user ids who searched word
    unordered_map<string, unordered_set<int>> users;

public:
    // The set ignores a repeat by the same user. T: O(1) average.
    void recordSearch(int userId, const string& word) { users[word].insert(userId); }

    // Sort every word at query time. T: O(W log W), S: O(W); W = distinct words.
    vector<string> getTopK(int k) {
        // (-distinct users, word): default pair order = users desc, then word asc
        vector<pair<int, string>> all;
        for (const auto& [word, ids] : users) { all.push_back({-(int)ids.size(), word}); }
        sort(all.begin(), all.end());
        vector<string> result;
        for (int i = 0; i < k && i < (int)all.size(); i++) {
            result.push_back(all[i].second);
        }
        return result;
    }

    // follow-up: keep only k candidates in a heap whose top is the WORST one.
    // T: O(W log k), S: O(k).
    vector<string> getTopKHeap(int k) {
        // max-heap of (-users, word): top = fewest users, ties -> largest word = worst
        priority_queue<pair<int, string>> heap;
        for (const auto& [word, ids] : users) {
            heap.push({-(int)ids.size(), word});
            if ((int)heap.size() > k) { heap.pop(); }   // drop the worst candidate
        }
        vector<string> result;
        while (!heap.empty()) {
            result.push_back(heap.top().second);
            heap.pop();
        }
        reverse(result.begin(), result.end());   // popped worst-first
        return result;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SearchRanker r;
    check(r.getTopK(3).empty(), "no searches");
    r.recordSearch(1, "apple");
    r.recordSearch(2, "apple");
    r.recordSearch(1, "banana");
    r.recordSearch(1, "banana");
    r.recordSearch(1, "banana");
    r.recordSearch(3, "cherry");
    r.recordSearch(2, "cherry");
    check(r.getTopK(2) == vector<string>{"apple", "cherry"}, "tie by word");
    check(r.getTopK(10) == vector<string>{"apple", "cherry", "banana"},
          "repeats count once");
    r.recordSearch(4, "banana");
    r.recordSearch(5, "banana");
    check(r.getTopK(1) == vector<string>{"banana"}, "rank updates");
}
