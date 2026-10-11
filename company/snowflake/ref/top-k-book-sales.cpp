// bestSellers(books, counts, k): add this batch's sales to the running totals, then return up to k books
// order: higher total first, equal totals by name ascending (Q35)

// same book twice in one batch?
// counts only positive, or returns (negative) too?
// k = 0? k > number of distinct books?
// count 0 for a new book: listed with 0 sales?

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;
using std::pair;

// T: O(log U) per sale record, query O(min(k, U)), U: distinct books
// S: O(U)

class BookRanker {
private:
    std::unordered_map<string, long long> sales; // book -> total sold so far
    std::set<pair<long long, string>> ranking; // (-total, book): most sold first, ties by name

public:
    vector<string> bestSellers(const vector<string>& books, const vector<int>& counts, int k) {
        for (int i = 0; i < static_cast<int>(books.size()); i++) {
            const string& book = books[i];
            auto it = sales.find(book);
            long long old = (it == sales.end()) ? 0 : it->second;
            if (it != sales.end()) {
                ranking.erase({-old, book}); // remove the stale entry before the total changes
            }
            long long total = old + counts[i];
            sales[book] = total;
            ranking.insert({-total, book});
        }

        vector<string> top;
        for (auto it = ranking.begin(); it != ranking.end() && static_cast<int>(top.size()) < k; ++it) {
            top.push_back(it->second);
        }
        return top;
    }
};

int main() {
    BookRanker r;
    vector<string> want;

    want = {"a", "b"};
    assert(r.bestSellers({"b", "a"}, {5, 5}, 2) == want); // target + edge: equal totals by name
    want = {"c", "a"};
    assert(r.bestSellers({"c"}, {7}, 2) == want); // target: totals carry over across calls
    want = {"a", "c"};
    assert(r.bestSellers({"a", "a"}, {1, 2}, 2) == want); // edge: same book twice in one batch, a = 8
    want = {"a", "c", "b"};
    assert(r.bestSellers({}, {}, 5) == want); // null: empty batch only reads, k > distinct books
    assert(r.bestSellers({}, {}, 0).empty()); // fault: k = 0

    std::cout << "all tests passed\n";
    return 0;
}
