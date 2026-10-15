// ByteDance / LC 721: accounts[i] = [name, emails...]. Two accounts belong to the same
// person if they share an email. Merge them: [name, sorted emails...], any order.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

// returns the root account of a, compressing the path
int findRoot(vector<int>& parent, int a) {
    if (parent[a] != a) { parent[a] = findRoot(parent, parent[a]); }
    return parent[a];
}

// Union-find over account indices, joined through shared emails.
// T: O(E log E) for sorting, E = total emails; S: O(E).
vector<vector<string>> accountsMerge(const vector<vector<string>>& accounts) {
    int n = accounts.size();
    vector<int> parent(n);   // parent[i] = parent account of i in the union-find
    for (int i = 0; i < n; i++) { parent[i] = i; }
    unordered_map<string, int> owner;   // email -> first account that listed it
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            auto it = owner.find(email);
            if (it == owner.end()) {
                owner[email] = i;
            } else {
                parent[findRoot(parent, i)] = findRoot(parent, it->second);
            }
        }
    }
    unordered_map<int, vector<string>> groups;   // root account -> its distinct emails
    for (const auto& [email, acc] : owner) {
        groups[findRoot(parent, acc)].push_back(email);
    }
    vector<vector<string>> result;
    for (auto& [root, emails] : groups) {
        sort(emails.begin(), emails.end());
        vector<string> merged{accounts[root][0]};
        merged.insert(merged.end(), emails.begin(), emails.end());
        result.push_back(merged);
    }
    return result;
}

void test(const string& label, const vector<vector<string>>& accounts,
          vector<vector<string>> want) {
    vector<vector<string>> got = accountsMerge(accounts);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1",
         {{"John", "johnsmith@mail.com", "john_newyork@mail.com"},
          {"John", "johnsmith@mail.com", "john00@mail.com"},
          {"Mary", "mary@mail.com"},
          {"John", "johnnybravo@mail.com"}},
         {{"John", "john00@mail.com", "john_newyork@mail.com", "johnsmith@mail.com"},
          {"Mary", "mary@mail.com"},
          {"John", "johnnybravo@mail.com"}});
}
