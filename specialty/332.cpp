// LC 332 Reconstruct Itinerary: use every ticket [from, to] exactly once from "JFK";
// among valid itineraries return the lexicographically smallest. One always exists.
// Trick: Hierholzer - DFS smallest ticket first, add airport on the way out, reverse
// Recognize: "use every edge exactly once" -> Eulerian path, not backtracking
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::map;
using std::multiset;
using std::reverse;

// an airport is appended only after all its tickets are used, so a dead end found
// early lands at the END of the route instead of cutting it short
void visit(map<string, multiset<string>>& graph, const string& cur,
           vector<string>& route) {
    while (!graph[cur].empty()) {
        // take and burn the smallest remaining ticket out of cur
        string nxt = *graph[cur].begin();
        graph[cur].erase(graph[cur].begin());
        visit(graph, nxt, route);
    }
    route.push_back(cur);
}

// Hierholzer's post-order DFS. T: O(E log E), S: O(V+E).
vector<string> findItinerary(const vector<vector<string>>& tickets) {
    map<string, multiset<string>> graph;   // graph[a] = unused ticket destinations from a
    for (const auto& t : tickets) { graph[t[0]].insert(t[1]); }
    vector<string> route;                  // airports in reverse visiting order
    visit(graph, "JFK", route);
    reverse(route.begin(), route.end());
    return route;
}

void test(const string& label, const vector<vector<string>>& tickets,
          const vector<string>& want) {
    cout << label << (findItinerary(tickets) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{"MUC", "LHR"}, {"JFK", "MUC"}, {"SFO", "SJC"}, {"LHR", "SFO"}},
         {"JFK", "MUC", "LHR", "SFO", "SJC"});
    test("lc ex2", {{"JFK", "SFO"}, {"JFK", "ATL"}, {"SFO", "ATL"}, {"ATL", "JFK"},
                    {"ATL", "SFO"}},
         {"JFK", "ATL", "JFK", "SFO", "ATL", "SFO"});
    test("dead end first", {{"JFK", "KUL"}, {"JFK", "NRT"}, {"NRT", "JFK"}},
         {"JFK", "NRT", "JFK", "KUL"});
}
