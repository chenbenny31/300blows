// LC 394 Decode String: decode k[encoded] = encoded repeated k times; brackets may nest.
// Input is valid, digits appear only as repeat counts.
// Trick: at '[' push (count, text so far); at ']' pop and append cur repeated count times
// Recognize: nested brackets with an outer context to resume -> stack of saved states
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Two stacks saved at each '['. T: O(output length), S: O(output length).
string decodeString(const string& s) {
    vector<int> counts;       // repeat count of each open bracket
    vector<string> prevs;     // text built before each open bracket
    string cur;               // text inside the innermost open bracket so far
    int num = 0;
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '[') {
            counts.push_back(num);
            prevs.push_back(cur);
            num = 0;
            cur = "";
        } else if (c == ']') {
            string outer = prevs.back();
            prevs.pop_back();
            for (int i = 0; i < counts.back(); i++) { outer += cur; }
            counts.pop_back();
            cur = outer;
        } else {
            cur += c;
        }
    }
    return cur;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (decodeString(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flat", "3[a]2[bc]", "aaabcbc");
    test("nested", "3[a2[c]]", "accaccacc");
    test("trailing text", "2[abc]3[cd]ef", "abcabccdcdcdef");
}
