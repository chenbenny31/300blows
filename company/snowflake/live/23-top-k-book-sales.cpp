// B12 (LC 347 / 692 prototypes): bestSellers(books, counts, k) adds this batch to running totals,
// then returns up to k book names, total sales descending, equal totals by name ascending.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::set;
using std::pair;
using std::priority_queue;

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
        for (auto it = ranking.begin(); it != ranking.end() && (int)top.size() < k; ++it) {
            top.push_back(it->second);
        }
        return top;
    }

    // totals only + size-k heap at query time, T: O(b + U*logk) per call, S: O(U)
    vector<string> bestSellers2(const vector<string>& books, const vector<int>& counts, int k) {
        int b = books.size();
        for (int i = 0; i < b; i++) { sales[books[i]] += counts[i]; }
        if (k <= 0) { return {}; }
        priority_queue<pair<long long, string>> worst;   // (-total, book): top is the worst of the kept k
        for (const auto& [book, total] : sales) {
            worst.push({-total, book});
            if ((int)worst.size() > k) { worst.pop(); }
        }
        vector<string> top;
        while (!worst.empty()) {
            top.push_back(worst.top().second);
            worst.pop();
        }
        std::reverse(top.begin(), top.end());   // popped worst first
        return top;
    }

private:
    unordered_map<string, long long> sales;   // book -> total sold so far
    set<pair<long long, string>> ranking;     // (-total, book): best seller first, ties by name
};

// runs the same batch on a method-1 ranker and a method-2 ranker
bool step(BookRanker& r1, BookRanker& r2, const vector<string>& books, const vector<int>& counts, int k,
          const vector<string>& expected, const string& label) {
    vector<string> got = r1.bestSellers(books, counts, k);
    vector<string> got2 = r2.bestSellers2(books, counts, k);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << ": got";
    for (const auto& s : got) { cout << " " << s; }
    cout << " /";
    for (const auto& s : got2) { cout << " " << s; }
    cout << "\n";
    return false;
}

bool testRunningTotals() {
    bool ok = true;
    BookRanker r1, r2;
    ok &= step(r1, r2, {"b", "a", "c"}, {5, 5, 3}, 2, {"a", "b"}, "tie by name");          // a = b = 5
    ok &= step(r1, r2, {"c"}, {4}, 2, {"c", "a"}, "totals carry over");                     // c = 7
    ok &= step(r1, r2, {"a", "a"}, {1, 2}, 3, {"a", "c", "b"}, "same book twice in batch"); // a = 8
    ok &= step(r1, r2, {}, {}, 10, {"a", "c", "b"}, "empty batch, k > distinct");
    ok &= step(r1, r2, {}, {}, 0, {}, "k = 0");
    return ok;
}

bool testSingleBook() {
    bool ok = true;
    BookRanker r1, r2;
    ok &= step(r1, r2, {"solo"}, {1}, 1, {"solo"}, "one book, k = 1");
    return ok;
}

bool testTotalDecreases() {
    bool ok = true;
    BookRanker r1, r2;
    ok &= step(r1, r2, {"x", "y"}, {5, 4}, 1, {"x"}, "x leads");
    ok &= step(r1, r2, {"x"}, {-2}, 2, {"y", "x"}, "return drops x to 3");   // stale (-5, x) entry must be gone
    ok &= step(r1, r2, {"z"}, {0}, 3, {"y", "x", "z"}, "zero-sale book listed");
    return ok;
}

bool testOverflow() {
    bool ok = true;
    BookRanker r1, r2;
    ok &= step(r1, r2, {"big", "small"}, {2000000000, 1}, 2, {"big", "small"}, "big first batch");
    ok &= step(r1, r2, {"big"}, {2000000000}, 2, {"big", "small"}, "big = 4e9 > INT_MAX");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testRunningTotals();   // given-style sequence: ties, carry-over, duplicates in batch, k bounds
    ok &= testSingleBook();      // smallest input
    ok &= testTotalDecreases();  // adversarial: stale ranking entry when a total changes
    ok &= testOverflow();        // boundary: totals beyond int
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
