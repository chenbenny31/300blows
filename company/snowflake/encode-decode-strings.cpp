#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

using std::size_t;
using std::string;
using std::vector;

class Codec {
public:
    // T: O(l), S: O(l), l = encoded length
    string encode(const vector<string>& strings) {
        string result;
        for (const auto& s : strings) {
            result += std::to_string(s.size());
            result += '#';
            result += s;
        }
        return result;
    }

    // T: O(l), S: O(l)
    vector<string> decode(const string& data) {
        vector<string> result;
        size_t i = 0;
        while (i < data.size()) {
            size_t sep_pos = data.find('#', i);
            size_t length = std::stoull(data.substr(i, sep_pos - i));
            result.push_back(data.substr(sep_pos + 1, length));
            i = sep_pos + 1 + length;
        }
        return result;
    }
};


int main() {
    Codec codec;

    vector<string> input = {"hi", "", "3#a,b", string("x\0y", 3)};
    assert(codec.decode(codec.encode(input)) == input);
    // Normal, empty, delimiter/digits, and embedded null byte.

    assert(codec.decode(codec.encode({})).empty());
    // Empty list differs from a list containing one empty string.

    assert(codec.encode({""}) == "0#");
}
