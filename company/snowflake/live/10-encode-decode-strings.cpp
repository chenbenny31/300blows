// B05 / LC 271: encode a list of arbitrary strings into one string and decode it back exactly.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class StringCodec {
public:
    // length prefix "len#payload" per string, T: O(L), S: O(L), L = total characters
    string encode(const vector<string>& strs) {
        string out;
        for (const auto& str : strs) {
            out += std::to_string(str.size());
            out += '#';
            out += str;
        }
        return out;
    }

    // read digits up to the first '#', then jump over exactly len chars, T: O(L), S: O(L)
    vector<string> decode(const string& s) {
        vector<string> out;
        int n = s.size();
        int i = 0;
        while (i < n) {
            int len = 0;
            while (s[i] != '#') {
                len = len * 10 + (s[i] - '0');
                i++;
            }
            i++;   // skip '#'
            out.push_back(s.substr(i, len));
            i += len;   // jump over the payload, never scan inside it
        }
        return out;
    }

    // escape '\' and ',' with '\', end every string with ',', T: O(L), S: O(L)
    string encode2(const vector<string>& strs) {
        string out;
        for (const auto& str : strs) {
            for (char c : str) {
                if (c == '\\' || c == ',') { out += '\\'; }
                out += c;
            }
            out += ',';   // terminator, not separator, so {} and {""} differ
        }
        return out;
    }

    // '\' takes the next char literally, bare ',' ends a string, T: O(L), S: O(L)
    vector<string> decode2(const string& s) {
        vector<string> out;
        string cur;   // string being rebuilt
        int n = s.size();
        for (int i = 0; i < n; i++) {
            if (s[i] == '\\') {
                cur += s[i + 1];
                i++;
            } else if (s[i] == ',') {
                out.push_back(cur);
                cur.clear();
            } else {
                cur += s[i];
            }
        }
        return out;
    }
};

// round trip through both schemes
bool test(const string& label, const vector<string>& strs) {
    StringCodec codec;
    vector<string> got = codec.decode(codec.encode(strs));
    vector<string> got2 = codec.decode2(codec.encode2(strs));
    if (got == strs && got2 == strs) { return true; }
    cout << "FAIL " << label << ": sizes " << got.size() << " / " << got2.size()
         << ", expected " << strs.size() << "\n";
    return false;
}

// exact encoded form, hand-computed
bool testEncoded(const string& label, const vector<string>& strs, const string& expected, const string& expected2) {
    StringCodec codec;
    string got = codec.encode(strs);
    string got2 = codec.encode2(strs);
    if (got == expected && got2 == expected2) { return true; }
    cout << "FAIL " << label << ": got \"" << got << "\" / \"" << got2
         << "\", expected \"" << expected << "\" / \"" << expected2 << "\"\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("lc1", {"Hello", "World"});                                     // LC example 1
    ok &= test("lc2", {""});                                                   // LC example 2: one empty string
    ok &= test("empty list", {});                                              // smallest: must not decode to {""}
    ok &= test("all empty", {"", "", ""});                                     // consecutive empty strings
    ok &= test("hash and digits", {"12#ab", "1#", "#", "0", "#3#"});           // payload looks like a header
    ok &= test("long", {string(12, 'x'), string(100, '#'), "y"});              // multi-digit length prefix
    ok &= test("escape chars", {"a\\", "b", ",", "\\,", ""});                  // trailing backslash, escaped terminator
    ok &= test("raw bytes", {string("x\0y", 3), string(1, '\xff'), "\n"});    // embedded NUL and non-ASCII byte
    ok &= testEncoded("given", {"hello", "world"}, "5#hello5#world", "hello,world,");   // exact format
    ok &= testEncoded("empty list", {}, "", "");                                      // {} encodes to ""
    ok &= testEncoded("one empty", {""}, "0#", ",");                                  // {""} differs from {}
    ok &= testEncoded("two empty", {"", ""}, "0#0#", ",,");                           // empty strings counted
    ok &= testEncoded("specials", {"3#a", ","}, "3#3#a1#,", "3#a,\\,,");               // '#' kept raw, ',' escaped
    ok &= testEncoded("backslash", {"a\\b"}, "3#a\\b", "a\\\\b,");                     // escape the escape char
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
