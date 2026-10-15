// LC 49: group the strings that are anagrams of each other; any order of groups
// and of words within a group.
// Follow-up: avoid the per-word sort with a letter-count key.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;
using std::to_string;

// Key = the word's letters sorted. T: O(n * k log k), S: O(n * k); k = max word length.
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups;   // sorted letters -> words
    for (const auto& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) { result.push_back(words); }
    return result;
}

// follow-up: key = 26 letter counts joined with '#'. T: O(n * k), S: O(n * k).
vector<vector<string>> groupAnagramsCount(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups;   // count key -> words
    for (const auto& s : strs) {
        vector<int> cnt(26, 0);   // cnt[i] = occurrences of letter 'a' + i
        for (char c : s) { cnt[c - 'a']++; }
        string key;
        // separator keeps counts 1,11 distinct from 11,1
        for (int x : cnt) { key += to_string(x) + '#'; }
        groups[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& [key, words] : groups) { result.push_back(words); }
    return result;
}

void test(const string& label, const vector<string>& strs, vector<vector<string>> want) {
    vector<vector<string>> got = groupAnagrams(strs);
    // canonical form: sort words inside each group, then sort the groups
    for (auto& g : got) { sort(g.begin(), g.end()); }
    sort(got.begin(), got.end());
    for (auto& g : want) { sort(g.begin(), g.end()); }
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {"eat", "tea", "tan", "ate", "nat", "bat"},
         {{"bat"}, {"nat", "tan"}, {"ate", "eat", "tea"}});
    test("empty word", {""}, {{""}});
    test("single", {"a"}, {{"a"}});
}
