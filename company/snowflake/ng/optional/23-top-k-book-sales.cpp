// B12: bestSellers(books, counts, k) adds this batch to the running totals, then
// returns the top k book names by total sales desc, equal totals by name asc.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::sort;

class BookRanker {
    unordered_map<string, long long> totals;   // book -> total sold so far

public:
    // Add the batch, then sort every book at query time.
    // T: O(b + U log U), S: O(U); b = batch size, U = distinct books.
    vector<string> bestSellers(const vector<string>& books, const vector<int>& counts,
                               int k) {
        int b = books.size();
        for (int i = 0; i < b; i++) { totals[books[i]] += counts[i]; }
        // ranked = (-total, book) pairs: sorted gives best first, ties by name
        vector<pair<long long, string>> ranked;
        for (const auto& [book, total] : totals) { ranked.push_back({-total, book}); }
        sort(ranked.begin(), ranked.end());
        vector<string> top;
        for (int i = 0; i < k && i < (int)ranked.size(); i++) {
            top.push_back(ranked[i].second);
        }
        return top;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    BookRanker r;
    check(r.bestSellers({"emma", "dune", "odyssey"}, {5, 5, 3}, 2) ==
          vector<string>{"dune", "emma"}, "tie by name");
    check(r.bestSellers({"odyssey"}, {4}, 2) ==
          vector<string>{"odyssey", "dune"}, "running totals");
    check(r.bestSellers({"emma", "emma"}, {1, 2}, 5) ==
          vector<string>{"emma", "odyssey", "dune"}, "repeat in batch, k > books");
}
