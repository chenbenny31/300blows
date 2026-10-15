// LC 763 Partition Labels: split lowercase s into as many parts as possible so each
// letter appears in at most one part; return the part sizes.
// Trick: record each letter's last index; extend end = max(last[c]); cut when i == end
// Recognize: partition so groups never straddle a cut -> last-occurrence sweep
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// A part can close only once every letter in it has had its last occurrence.
// T: O(n), S: O(1).
vector<int> partitionLabels(const string& s) {
    int n = s.size();
    vector<int> last(26, 0);   // last[c] = last index of letter c in s
    for (int i = 0; i < n; i++) { last[s[i] - 'a'] = i; }
    vector<int> sizes;
    int start = 0, end = 0;   // current part is s[start..end] so far
    for (int i = 0; i < n; i++) {
        end = max(end, last[s[i] - 'a']);
        if (i == end) {
            sizes.push_back(end - start + 1);
            start = i + 1;
        }
    }
    return sizes;
}

void test(const string& label, const string& s, const vector<int>& want) {
    cout << label << (partitionLabels(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "ababcbacadefegdehijhklij", {9, 7, 8});
    test("lc ex2", "eccbbbbdec", {10});
}
