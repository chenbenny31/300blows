#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;

bool isVowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' ||
           c == 'o' || c == 'u';
}


// T: O(n), S: O(1)
long long countVowelSubstrings(const string& word) {
    vector<int> last(26, -1);
    int start = 0; // start of vowel-only
    int total = 0;

    for (int right = 0; right < static_cast<int>(word.size()); right++) {
        if (!isVowel(word[right])) {
            start = right + 1;
            continue;
        }

        last[word[right] - 'a'] = right;

        int minLast = right;
        for (char v : string("aeiou")) {
            minLast = std::min(minLast, last[v - 'a']);
        }
        if (minLast >= start) {
            total += minLast - start + 1;
        }
    }
    return total;
}


int main() {
    assert(countVowelSubstrings("aeiouu") == 2); // Multiple endings.
    assert(countVowelSubstrings("aaeiou") == 2); // Multiple starts.
    assert(countVowelSubstrings("aeioubuaeio") == 2); // Separate runs.
    assert(countVowelSubstrings("aeio") == 0); // Missing vowel.
    assert(countVowelSubstrings("") == 0);
}
