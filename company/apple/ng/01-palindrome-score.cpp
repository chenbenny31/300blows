// Apple (hack2hire): score 0..100 for how palindromic a string is. Default strict
// policy: split into grapheme clusters, normalize, compare each mirrored pair; a pair
// counts 1 only when both clusters are identical. score = 100 * matched / pairs,
// and 100 when pairs == 0.
// Report mismatches as grapheme indices, not byte offsets.
// Follow-up: an explicit, versioned opt-in policy (positional weights, partial credit for
// the same base letter with different accents).
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::fabs;

struct ScoreReport {
    double score;
    // (left, right) grapheme indices of unequal pairs
    vector<pair<int, int>> mismatches;
};

// byte length of the UTF-8 code point that starts with lead byte c
int codePointLen(unsigned char c) {
    if (c < 0x80) { return 1; }
    if ((c >> 5) == 0x6) { return 2; }
    if ((c >> 4) == 0xE) { return 3; }
    return 4;
}

// combining diacritics U+0300..U+036F (2-byte UTF-8) attach to the previous base
bool isCombining(const string& cp) {
    if (cp.size() != 2) { return false; }
    int code = ((cp[0] & 0x1F) << 6) | (cp[1] & 0x3F);
    return code >= 0x300 && code <= 0x36F;
}

// approximate grapheme clusters: base code point + following combining marks,
// default normalization = ASCII lowercase. T: O(n), S: O(n).
vector<string> graphemes(const string& s) {
    vector<string> out;   // out[g] = bytes of grapheme g
    int n = s.size();
    for (int i = 0; i < n;) {
        int len = codePointLen(s[i]);
        string cp = s.substr(i, len);
        for (char& c : cp) {
            if (c >= 'A' && c <= 'Z') { c = c - 'A' + 'a'; }
        }
        if (!out.empty() && isCombining(cp)) {
            out.back() += cp;
        } else {
            out.push_back(cp);
        }
        i += len;
    }
    return out;
}

// strict policy v0: two pointers over clusters, equal weight per pair. T: O(n), S: O(n).
ScoreReport palindromeScore(const string& s) {
    vector<string> g = graphemes(s);
    int m = g.size();
    int pairs = m / 2, matched = 0;
    ScoreReport report{100.0, {}};
    for (int l = 0, r = m - 1; l < r; l++, r--) {
        if (g[l] == g[r]) {
            matched++;
        } else {
            report.mismatches.push_back({l, r});
        }
    }
    // empty or one grapheme: nothing to compare, keep 100
    if (pairs > 0) { report.score = 100.0 * matched / pairs; }
    return report;
}

// follow-up policy, versioned so a stored score can always be recomputed and explained
struct Policy {
    string version;      // e.g. "v1-weighted-accent"
    bool outerHeavier;   // pair k from the outside weighs pairs - k, else 1
    double baseCredit;   // credit when only the base letters match (e vs e + accent)
};

// weighted average of per-pair similarity in [0, 1]. T: O(n), S: O(n).
double palindromeScore(const string& s, const Policy& policy) {
    vector<string> g = graphemes(s);
    int m = g.size();
    int pairs = m / 2;
    if (pairs == 0) { return 100.0; }
    double got = 0, total = 0;
    for (int k = 0; k < pairs; k++) {
        const string& a = g[k];
        const string& b = g[m - 1 - k];
        double sim = 0;
        if (a == b) {
            sim = 1;
        } else if (a.substr(0, codePointLen(a[0])) == b.substr(0, codePointLen(b[0]))) {
            sim = policy.baseCredit;
        }
        double w = policy.outerHeavier ? pairs - k : 1;
        got += w * sim;
        total += w;
    }
    return 100.0 * got / total;
}

void test(const string& label, const string& s, double want,
          const vector<pair<int, int>>& wantMismatches) {
    ScoreReport r = palindromeScore(s);
    bool ok = fabs(r.score - want) < 1e-9 && r.mismatches == wantMismatches;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("abcca", "abcca", 50.0, {{1, 3}});
    test("racecar", "Racecar", 100.0, {});
    test("abcd", "abcd", 0.0, {{0, 3}, {1, 2}});
    test("empty", "", 100.0, {});
    test("combining", "e\xCC\x81" "ba", 0.0, {{0, 2}});
    test("accent pal", "e\xCC\x81" "be\xCC\x81", 100.0, {});
}
