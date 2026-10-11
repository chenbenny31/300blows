// encode a list of strings into one string and decode it back exactly (LC271, J18280)
// strings may be empty and may contain any character, including the ones the encoding uses

// any byte allowed in a string? '\0', non-ASCII?
// empty strings? empty list?
// decode only our own output, or must it reject malformed input?
// size limits? streaming decode?

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class Codec {
public:
    // length prefix "len#" before each string, T: O(L), S: O(L), L: total characters
    string encode_1(const vector<string>& strs) {
        string out;
        for (const string& s : strs) {
            out += std::to_string(s.size()) + "#" + s;
        }
        return out;
    }

    vector<string> decode_1(const string& data) {
        vector<string> strs;
        size_t i = 0;
        while (i < data.size()) {
            size_t hash = data.find('#', i); // first '#' after i ends the length; content may hold '#'
            size_t len = std::stoul(data.substr(i, hash - i));
            strs.push_back(data.substr(hash + 1, len));
            i = hash + 1 + len; // jump over the content, never scan inside it
        }
        return strs;
    }

    // escaping: '\' -> "\\", ',' -> "\,", every string ends with an unescaped ','
    // T: O(L), S: O(L)
    string encode_2(const vector<string>& strs) {
        string out;
        for (const string& s : strs) {
            for (char c : s) {
                if (c == '\\' || c == ',') {
                    out += '\\';
                }
                out += c;
            }
            out += ','; // terminator, not separator, so {} and {""} encode differently
        }
        return out;
    }

    vector<string> decode_2(const string& data) {
        vector<string> strs;
        string cur; // string being rebuilt
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i] == '\\') {
                cur += data[i + 1]; // escaped char taken literally
                i++;
            } else if (data[i] == ',') {
                strs.push_back(cur);
                cur.clear();
            } else {
                cur += data[i];
            }
        }
        return strs;
    }
};

int main() {
    Codec c;
    vector<string> want;

    want = {"hello", "world"}; // target
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    want = {"3#ab", "a,\\b"}; // edge: encoding chars and digits inside the content
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    want = {"", ""}; // edge: empty strings
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    want = {}; // null: empty list must not come back as {""}
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    std::cout << "all tests passed\n";
    return 0;
}
