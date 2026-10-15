// LC tag · LC 2062: count substrings of word made only of vowels (a e i o u) that
// contain all five vowels. Substrings at different positions count separately.
// Follow-up: O(n).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::min;

bool isVowel(char c) { return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'; }

// Every start, extend until a consonant. T: O(n^2), S: O(1).
int countVowelSubstrings(const string& word) {
    int n = word.size();
    int total = 0;
    for (int i = 0; i < n; i++) {
        unordered_set<char> seen;   // distinct vowels in word[i..j]
        for (int j = i; j < n && isVowel(word[j]); j++) {
            seen.insert(word[j]);
            if (seen.size() == 5) { total++; }
        }
    }
    return total;
}

// follow-up: for each end r, valid starts are (lastConsonant, earliest of the five
// latest vowel positions]. T: O(n), S: O(1).
int countVowelSubstringsFast(const string& word) {
    vector<int> last(26, -1);   // last[c] = latest index of letter c
    int lastConsonant = -1;     // latest non-vowel index
    int total = 0;
    int n = word.size();
    for (int r = 0; r < n; r++) {
        if (!isVowel(word[r])) {
            lastConsonant = r;
            continue;
        }
        last[word[r] - 'a'] = r;
        // a start must be at or before every vowel's latest position
        int minLast = r;
        for (char v : string("aeiou")) { minLast = min(minLast, last[v - 'a']); }
        if (minLast > lastConsonant) { total += minLast - lastConsonant; }
    }
    return total;
}

void test(const string& label, const string& word, int want) {
    cout << label << (countVowelSubstrings(word) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "aeiouu", 2);
    test("consonants", "cuaieuouac", 7);
    test("missing vowel", "aeio", 0);
}
