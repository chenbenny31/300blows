// LC 271 Encode and Decode Strings: encode a list of strings into one string and decode
// it back exactly; strings may contain any character, including '#' and digits.
// Trick: frame each string as "len#str"; decode reads len, then takes len raw chars
// Recognize: no delimiter is safe for arbitrary strings -> length-prefix framing
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::to_string;

// Length prefix + '#'. T: O(total chars), S: O(total chars).
string encode(const vector<string>& strs) {
    string result;
    for (const string& s : strs) { result += to_string(s.size()) + "#" + s; }
    return result;
}

// read digits up to '#', then copy len chars without looking at them, so a '#' or
// digit inside s is never mistaken for framing. T: O(total chars), S: O(total chars).
vector<string> decode(const string& s) {
    vector<string> result;
    int n = s.size();
    int i = 0;
    while (i < n) {
        int len = 0;
        while (s[i] != '#') {
            len = len * 10 + (s[i] - '0');
            i++;
        }
        i++;   // skip '#'
        result.push_back(s.substr(i, len));
        i += len;
    }
    return result;
}

void test(const string& label, const vector<string>& strs, const string& want) {
    cout << label << (encode(strs) == want ? " PASS\n" : " FAIL\n");
}

void testRoundTrip(const string& label, const vector<string>& strs) {
    cout << label << (decode(encode(strs)) == strs ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {"lint", "code"}, "4#lint4#code");
    testRoundTrip("tricky chars", {"", "#", "12#ab"});
    testRoundTrip("empty list", {});
}
