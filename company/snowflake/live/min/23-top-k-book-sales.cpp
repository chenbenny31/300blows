// B12 · LC 347 / 692 prototypes: bestSellers(books, counts, k) adds the batch to running totals, returns top k by total desc, ties by name asc.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::set;
using std::pair;

class BookRanker {
public:
    // ordered set kept in sync with totals, T: O(b*logU + k) per call, S: O(U)
    vector<string> bestSellers(const vector<string>& books, const vector<int>& counts, int k) {
        int b = books.size();
        for (int i = 0; i < b; i++) {
            const string& book = books[i];
            auto it = sales.find(book);
            if (it != sales.end()) { ranking.erase({-it->second, book}); }   // erase the old entry before the total changes
            long long total = (it == sales.end() ? 0 : it->second) + counts[i];
            sales[book] = total;
            ranking.insert({-total, book});
        }
        vector<string> top;
        for (auto it = ranking.begin(); it != ranking.end() && (int)top.size() < k; ++it) { top.push_back(it->second); }
        return top;
    }

private:
    unordered_map<string, long long> sales;   // book -> total sold so far
    set<pair<long long, string>> ranking;     // (-total, book): best seller first, ties by name
};

void check(const vector<string>& got, const vector<string>& expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    BookRanker r;
    check(r.bestSellers({"b", "a", "c"}, {5, 5, 3}, 2), {"a", "b"}, "tie by name");                // a = b = 5
    check(r.bestSellers({"c"}, {4}, 2), {"c", "a"}, "totals carry over");                         // c = 7
    check(r.bestSellers({"a", "a"}, {1, 2}, 3), {"a", "c", "b"}, "same book twice in a batch");   // a = 8
    check(r.bestSellers({}, {}, 0), {}, "k = 0");                                                 // smallest k

    BookRanker d;
    check(d.bestSellers({"x", "y"}, {5, 4}, 1), {"x"}, "x leads");
    check(d.bestSellers({"x"}, {-2}, 2), {"y", "x"}, "return drops x");                          // stale (-5, x) must be gone
    check(d.bestSellers({"x", "x"}, {2000000000, 2000000000}, 1), {"x"}, "beyond int");           // int would wrap below y
}
