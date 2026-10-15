// A12 · no direct LC (cf. LC 692): recordSearch(userId, word); getTopK(k) returns the k words with the most distinct users,
// ties by word ascending; a user searching the same word again counts once.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::set;
using std::pair;

class SearchRanker {
public:
    // T: O(log W) average (set update only when the user is new for this word)
    void recordSearch(int userId, const string& word) {
        unordered_set<int>& users = usersByWord[word];
        if (users.count(userId) != 0) { return; }   // repeat by the same user does not count
        int before = users.size();
        users.insert(userId);
        ranked.erase({-before, word});              // no-op when before == 0
        ranked.insert({-(before + 1), word});
    }

    // collect and sort every word on demand, T: O(W log W), S: O(W)
    vector<string> getTopK(int k) {
        vector<pair<int, string>> all;              // (distinct users, word)
        for (const auto& [word, users] : usersByWord) {
            all.push_back({(int)users.size(), word});
        }
        std::sort(all.begin(), all.end(), [](const auto& a, const auto& b) {
            if (a.first != b.first) { return a.first > b.first; }
            return a.second < b.second;
        });
        vector<string> result;
        for (int i = 0; i < k && i < (int)all.size(); i++) {
            result.push_back(all[i].second);
        }
        return result;
    }

    // ordered set kept up to date on every new (user, word), T: O(k), S: O(W)
    vector<string> getTopK2(int k) {
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

bool check(SearchRanker& r, int k, const vector<string>& expected, const string& label) {
    vector<string> got = r.getTopK(k);
    vector<string> got2 = r.getTopK2(k);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << ": got size " << got.size() << " / " << got2.size()
         << ", expected size " << expected.size() << "\n";
    return false;
}

bool testRanking() {
    bool ok = true;
    SearchRanker r;
    r.recordSearch(1, "apple");
    r.recordSearch(2, "apple");
    r.recordSearch(1, "banana");
    r.recordSearch(1, "banana");                   // same user three times: still 1 distinct user
    r.recordSearch(1, "banana");
    r.recordSearch(3, "cherry");
    r.recordSearch(2, "cherry");
    ok &= check(r, 1, {"apple"}, "top1 tie broken by word");
    ok &= check(r, 2, {"apple", "cherry"}, "top2 tie on 2 users");
    ok &= check(r, 3, {"apple", "cherry", "banana"}, "repeat searches not counted");
    ok &= check(r, 10, {"apple", "cherry", "banana"}, "k larger than word count");
    ok &= check(r, 0, {}, "k == 0");
    r.recordSearch(4, "banana");
    r.recordSearch(5, "banana");                   // banana climbs to 3 users
    ok &= check(r, 3, {"banana", "apple", "cherry"}, "rank updates after new users");
    return ok;
}

bool testEmpty() {
    SearchRanker r;
    return check(r, 3, {}, "no searches yet");
}

bool testSingle() {
    SearchRanker r;
    r.recordSearch(7, "x");
    r.recordSearch(7, "x");
    return check(r, 1, {"x"}, "single word, repeated by one user");
}

int main() {
    bool ok = true;
    ok &= testRanking();   // given rules: distinct users desc, word asc, repeats ignored, rank changes
    ok &= testEmpty();     // smallest input
    ok &= testSingle();    // one word, duplicate record must not create a second entry
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
