// LC 443: compress chars in place: each run becomes the char plus its count if > 1
// (count written digit by digit). Return the new length.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::reverse;

// Read pointer scans runs, write pointer overwrites the front. T: O(n), S: O(1).
int compress(vector<char>& chars) {
    int n = chars.size();
    int write = 0;   // chars[0..write) = compressed output so far
    for (int read = 0; read < n;) {
        char c = chars[read];
        int start = read;
        while (read < n && chars[read] == c) { read++; }
        int cnt = read - start;
        chars[write++] = c;
        if (cnt == 1) { continue; }
        // digits come out least significant first; reverse them in place
        int digitStart = write;
        for (; cnt > 0; cnt /= 10) { chars[write++] = '0' + cnt % 10; }
        reverse(chars.begin() + digitStart, chars.begin() + write);
    }
    return write;
}

void test(const string& label, vector<char> chars, const string& want) {
    int len = compress(chars);
    string got(chars.begin(), chars.begin() + len);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {'a', 'a', 'b', 'b', 'c', 'c', 'c'}, "a2b2c3");
    test("single", {'a'}, "a");
    test("count 12", {'a', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'},
         "ab12");
}
