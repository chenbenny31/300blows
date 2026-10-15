// ByteDance (hack2hire) / LC 151: reverse the order of words in s (words = runs of
// non-space chars); output single-spaced with no leading/trailing spaces.
// Follow-up 1: keep the original spacing (leading, each gap, trailing); only move words.
// Follow-up 2: in place on a mutable string, O(1) extra space.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::reverse;

// Char-loop split into words, then join them in reverse. T: O(n), S: O(n).
string reverseWords(const string& s) {
    vector<string> words;   // words of s in original order
    string word;            // word being read
    // trailing sentinel space flushes the last word
    for (char c : s + ' ') {
        if (c != ' ') {
            word += c;
        } else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    string result;
    for (int i = (int)words.size() - 1; i >= 0; i--) {
        if (!result.empty()) { result += ' '; }
        result += words[i];
    }
    return result;
}

// follow-up 1: collect words and the gaps around them, then refill the gap template
// with the words reversed. T: O(n), S: O(n).
string reverseWordsKeepSpacing(const string& s) {
    vector<string> words;     // words of s in original order
    vector<string> gaps{""};  // gaps[i] = spaces before words[i]; gaps.back() = trailing
    int n = s.size();
    for (int i = 0; i < n; i++) {
        if (s[i] == ' ') {
            gaps.back() += ' ';
            continue;
        }
        // first char of a word opens the word and the gap after it
        if (i == 0 || s[i - 1] == ' ') {
            words.push_back("");
            gaps.push_back("");
        }
        words.back() += s[i];
    }
    int w = words.size();
    string result = gaps[0];
    for (int i = 0; i < w; i++) { result += words[w - 1 - i] + gaps[i + 1]; }
    return result;
}

// follow-up 2: reverse all, then reverse each word while compacting spaces with a
// write pointer. T: O(n), S: O(1).
void reverseWordsInPlace(string& s) {
    reverse(s.begin(), s.end());
    int n = s.size();
    int w = 0;   // s[0..w) = finished output; w <= i, so writes never pass reads
    for (int i = 0; i < n; i++) {
        if (s[i] == ' ') { continue; }
        if (w > 0) { s[w++] = ' '; }   // one space between words
        int start = w;
        while (i < n && s[i] != ' ') { s[w++] = s[i++]; }
        reverse(s.begin() + start, s.begin() + w);
    }
    s.resize(w);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (reverseWords(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "the sky is blue", "blue is sky the");
    test("trim", "  hello world  ", "world hello");
    test("inner gaps", "a good   example", "example good a");
}
