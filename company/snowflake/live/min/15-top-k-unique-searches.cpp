// A12 · no direct LC (cf. LC 692): recordSearch(userId, word); getTopK(k) = k words with most distinct users, ties by word ascending. Follow-up: frequent queries.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::set;
using std::pair;

class SearchRanker {
public:
    // T: O(1) average
    void recordSearch(int userId, const string& word) { usersByWord[word].insert(userId); }

    // collect and sort every word on demand, T: O(W log W), S: O(W)
    vector<string> getTopK(int k) {
        vector<pair<int, string>> all;              // (distinct users, word)
        for (const auto& [word, users] : usersByWord) { all.push_back({(int)users.size(), word}); }
        std::sort(all.begin(), all.end(), [](const auto& a, const auto& b) {
            if (a.first != b.first) { return a.first > b.first; }
            return a.second < b.second;
        });
        vector<string> result;
        for (int i = 0; i < k && i < (int)all.size(); i++) { result.push_back(all[i].second); }
        return result;
    }

private:
    unordered_map<string, unordered_set<int>> usersByWord;   // word -> distinct users who searched it
};

// follow-up: ordered set kept current on every new (user, word), S: O(U + W)
class SearchRankerSet {
public:
    // delete old key, insert new, T: O(log W)
    void recordSearch(int userId, const string& word) {
        unordered_set<int>& users = usersByWord[word];
        if (users.count(userId)) { return; }       // repeat by the same user does not count
        int before = users.size();
        users.insert(userId);
        ranked.erase({-before, word});              // erase old key first, else word appears twice
        ranked.insert({-(before + 1), word});
    }

    // first k of the set, T: O(k), S: O(k)
    vector<string> getTopK(int k) {
        vector<string> result;
        for (const auto& [negUsers, word] : ranked) {
            if ((int)result.size() >= k) { break; }
            result.push_back(word);
        }
        return result;
    }

private:
    unordered_map<string, unordered_set<int>> usersByWord;   // word -> distinct users who searched it
    set<pair<int, string>> ranked;                           // (-distinct users, word), best first
};

void check(const vector<string>& got, const vector<string>& expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    SearchRanker r;
    check(r.getTopK(3), {}, "no searches yet");                                          // smallest input
    r.recordSearch(1, "apple");
    r.recordSearch(2, "apple");
    r.recordSearch(1, "banana");
    r.recordSearch(1, "banana");                                                         // repeat: still 1 user
    r.recordSearch(3, "cherry");
    r.recordSearch(2, "cherry");
    check(r.getTopK(2), {"apple", "cherry"}, "tie on 2 users -> word asc");
    check(r.getTopK(10), {"apple", "cherry", "banana"}, "repeats not counted, k > words");
    check(r.getTopK(0), {}, "k == 0");

    SearchRankerSet s;
    s.recordSearch(1, "b");
    s.recordSearch(1, "b");                                                              // repeat: still 1 user
    s.recordSearch(2, "a");
    check(s.getTopK(5), {"a", "b"}, "set: tie -> word asc, repeat ignored");
    s.recordSearch(2, "b");
    s.recordSearch(3, "b");
    check(s.getTopK(5), {"b", "a"}, "set: rank update, old key erased");                // no duplicate b
}
