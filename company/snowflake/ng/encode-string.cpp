#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::to_string;
using std::stoi;

string encode(const vector<string>& strs) {
    string result;
    for (const string& s : strs) {
        result += to_string(s.size());
        result += '#'; // s may contains digits
        result += s;
    }
    return result;
}

vector<string> decode(const string& s) {
    vector<string> result;
    int n = static_cast<int>(s.size());
    int i = 0;

    while (i < n) {
        int sep = s.find('#', i);
        int len = stoi(s.substr(i, sep - i));
        result.push_back(s.substr(sep + 1, len));
        i = sep + 1 + len;
    }
    return result;
}

void test(const string& label, const vector<string>& strs) {
    cout << label << (decode(encode(strs)) == strs ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tricky", {"hello", "", "3#ab"});
    test("empty list", {});
    test("one empty", {""});
}
