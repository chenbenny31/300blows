// Seen in Snowflake interview (variant of the last1 / last2 scan in nearest-two):
// text = "one quick brown fox ...", positions are word indices.
// 1 query word  -> every index where it appears.
// 2 query words (w1, w2) + k -> every index i of w1 that has a w2 at j > i with j - i <= k.
#include <iostream>
#include <vector>
#include <string>
#include <deque>
using std::cout;
using std::string;
using std::vector;
using std::deque;

// Split on spaces with a char loop (no stringstream). T: O(len).
vector<string> splitWords(const string& text) {
    vector<string> words;
    string word;
    for (char c : text) {
        if (c != ' ') {
            word += c;
            continue;
        }
        if (!word.empty()) { words.push_back(word); }
        word.clear();
    }
    if (!word.empty()) { words.push_back(word); }
    return words;
}

// One word: linear scan. T: O(n) words.
vector<int> findWord(const string& text, const string& w) {
    vector<string> words = splitWords(text);
    vector<int> result;
    for (int i = 0; i < (int)words.size(); i++) {
        if (words[i] == w) { result.push_back(i); }
    }
    return result;
}

// Two words: deque of w1 indices still waiting for a w2; prune lazily when a w2 shows up.
// Each index enters and leaves the deque once. T: O(n), S: O(n).
vector<int> findPair(const string& text, const string& w1, const string& w2, int k) {
    vector<string> words = splitWords(text);
    deque<int> pending;   // w1 indices not yet matched, increasing
    vector<int> result;
    for (int i = 0; i < (int)words.size(); i++) {
        if (words[i] == w2) {
            // too far from this w2 -> also too far from every later w2
            while (!pending.empty() && i - pending.front() > k) { pending.pop_front(); }
            for (int p : pending) { result.push_back(p); }
            pending.clear();   // each w1 is reported once
        }
        // push after the w2 check, so a word never pairs with itself when w1 == w2
        if (words[i] == w1) { pending.push_back(i); }
    }
    return result;
}

void testWord(const string& label, const string& text, const string& w,
              const vector<int>& want) {
    cout << label << (findWord(text, w) == want ? " PASS\n" : " FAIL\n");
}

void testPair(const string& label, const string& text, const string& w1,
              const string& w2, int k, const vector<int>& want) {
    cout << label << (findPair(text, w1, w2, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    string text = "one quick brown fox jumps over quick dog";
    testWord("one word", text, "quick", {1, 6});
    testPair("within k", text, "quick", "fox", 2, {1});
    testPair("too far", text, "quick", "fox", 1, {});
    testPair("stale pruned", "quick a b quick fox", "quick", "fox", 1, {3});
}
