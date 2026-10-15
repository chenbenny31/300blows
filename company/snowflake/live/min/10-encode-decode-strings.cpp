// B05 · LC 271: encode a list of arbitrary byte strings into one string and decode it back exactly ({} and {""} must differ).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

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
        i++;                              // skip '#'
        out.push_back(s.substr(i, len));
        i += len;                         // jump over the payload, never scan inside it
    }
    return out;
}

void test(const string& label, const vector<string>& strs) {
    vector<string> got = decode(encode(strs));
    cout << (got == strs ? "PASS " : "FAIL ") << label << "\n";
}

void testEncode(const vector<string>& strs, const string& expect) {
    string got = encode(strs);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    test("lc1", {"Hello", "World"});                               // target example
    test("empty list", {});                                        // must not decode to {""}
    test("all empty", {"", "", ""});                               // consecutive empty strings
    test("hash and digits", {"12#ab", "1#", "#", "0", "#3#"});     // payload looks like a header
    test("long", {string(12, 'x'), string(100, '#'), "y"});        // multi-digit length prefix

    testEncode({"hello", "world"}, "5#hello5#world");              // exact format
    testEncode({""}, "0#");                                        // {""} differs from {}
    testEncode({"3#a", ","}, "3#3#a1#,");                          // '#' in payload kept raw
}
