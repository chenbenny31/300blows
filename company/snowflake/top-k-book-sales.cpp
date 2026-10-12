#include <cassert>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

class BookRanker {
private:
    unordered_map<string, long long> sales; // book -> its acc sales
    set<pair<long long, string>> rank; // {-sales, name}, larger sale first

public:
    // T: O(mlogn+k), S: O(n+k), m=batch-size, n=distinct-books
    vector<string> bestSellers(const vector<string>& books,
                               const vector<int>& increments, int k) {
        for (int i = 0; i < static_cast<int>(books.size()); i++) {
            const string& book = books[i];
            long long& total = sales[book];

            rank.erase({-total, book});
            total += increments[i];
            rank.insert({-total, book});
        }

        vector<string> result;
        for (auto it = rank.begin(); it != rank.end() && static_cast<int>(result.size()) < k; it++) {
            result.push_back(it->second);
        }
        return result;
    }
};

int main() {
    BookRanker ranker;

    vector<string> expected = {"a", "b"};
    assert(ranker.bestSellers({"b", "a"}, {5, 5}, 2) == expected);
    // Equal sales: alphabetical order.

    expected = {"b"};
    assert(ranker.bestSellers({"b", "b"}, {1, 2}, 1) == expected);
    // Repeated book within a batch; totals persist across calls.

    assert(ranker.bestSellers({"a"}, {4}, 0).empty());
    // k = 0 still applies updates: a now has 9, b has 8.

    expected = {"a", "b"};
    assert(ranker.bestSellers({}, {}, 10) == expected);
    // Empty batch queries existing ranking; k may exceed book count.
}
