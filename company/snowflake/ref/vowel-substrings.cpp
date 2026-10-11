// count substrings made only of vowels (a e i o u) that contain all five (LC2062)

// lowercase letters only?
// substrings counted by position (same text at two places counts twice)?

#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::string;
using std::vector;

class VowelSubstrings {
public:
    // 1: every start, extend until a consonant, T: O(n^2), S: O(1) (set holds at most 5)
    int count_1(const string& s) {
        int n = static_cast<int>(s.size());
        int total = 0;
        for (int i = 0; i < n; i++) {
            std::unordered_set<char> vowels; // distinct vowels in s[i..j]
            for (int j = i; j < n && isVowel(s[j]); j++) {
                vowels.insert(s[j]);
                if (vowels.size() == 5) {
                    total++;
                }
            }
        }
        return total;
    }

    // 2: for each end r, valid starts are (lastConsonant, earliest of the five latest vowel spots]
    // T: O(n), S: O(1)
    int count_2(const string& s) {
        string order = "aeiou";
        vector<int> last(5, -1); // vowel -> latest index seen
        int lastConsonant = -1;
        int total = 0;
        for (int r = 0; r < static_cast<int>(s.size()); r++) {
            if (!isVowel(s[r])) {
                lastConsonant = r;
                continue;
            }
            last[order.find(s[r])] = r;
            int minLast = *std::min_element(last.begin(), last.end()); // start must reach every vowel
            if (minLast > lastConsonant) {
                total += minLast - lastConsonant; // starts lastConsonant+1 .. minLast
            }
        }
        return total;
    }

private:
    bool isVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }
};

int main() {
    VowelSubstrings vs;

    assert(vs.count_1("aeiouu") == 2); // target: "aeiou", "aeiouu"
    assert(vs.count_2("aeiouu") == 2);
    assert(vs.count_1("cuaieuouac") == 7); // edge: consonants on both sides
    assert(vs.count_2("cuaieuouac") == 7);
    assert(vs.count_1("aeio") == 0); // fault: one vowel missing
    assert(vs.count_2("aeio") == 0);
    assert(vs.count_1("") == 0); // null
    assert(vs.count_2("") == 0);

    std::cout << "all tests passed\n";
    return 0;
}
