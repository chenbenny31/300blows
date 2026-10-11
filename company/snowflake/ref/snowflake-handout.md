# Snowflake NG Coding Handout

Every problem practiced for the two Snowflake new-grad technical rounds, with the runnable C++ solution file. Sources: `Snowflake_Algo_Prep_20261006_86LC.md` (Q-numbers, C/E/H/J source tags) and LeetCode's Snowflake company tag list. Each code block is the file in this directory, verbatim; all of them compile with `-Wall -Wextra -fsanitize=address,undefined` and print `all tests passed` (checked 2026-10-08; `file-system.cpp` never frees its nodes by design, so run it with `ASAN_OPTIONS=detect_leaks=0`).


## Round format (from the recruiter email, via the prep doc)

- Two back-to-back online technical rounds, LeetCode medium-hard, no DP expected, **no AI of any kind**.
- At least 2 minutes of clarifying questions; explain the approach before coding.
- Write your own tests in a runnable C++ file (`main` + `assert`), run them, debug.
- Target: bug-free code in the 10-35 minute slot of a 60-minute round.
- Tests: one each of **target, edge, fault, null**, tiny inputs. Two solutions: the plain one first, the optimized one as the follow-up.
- Compile early, right after includes + an empty class: most bugs this prep were typos (`using std::vector:`, an extra `}`).

```bash
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined f.cpp -o f && ./f
```

Never `-DNDEBUG` (it removes every `assert`).


## Contents


**Graphs and topological order**

1. [Course Schedule II](#p1) — LC210 (+207); E05, C16
2. [All Ancestors of a Node in a DAG](#p2) — LC2192; prototype of A8 ACL / B9
3. [Parallel Courses + variable durations](#p3) — LC1136 (J10557 NG), LC2050
4. [DAG Privilege Inheritance (set / ordered)](#p4) — J9549, J12349 NG; Q04/Q05 follow-ups
5. [Possible Bipartition / Is Graph Bipartite](#p5) — LC886, LC785; Q40 contrast


**BFS: grids and crawling**

6. [Shortest Path in a Grid (any start/target)](#p6) — Q32 (C09 NG round 1), LC1091, J10351
7. [Web Crawler / Fewest Clicks](#p7) — LC1236, Q06 (H01 IC1), J15972, J12933


**Trees**

8. [Tree Height after Deleting Nodes (promotion)](#p8) — LC559 + Q01 (C02)
9. [Min Whole-Subtree Deletions to Bound Depth](#p9) — LC559 + J12962
10. [New Tree of Subtree Sums](#p10) — Q45 (J10558 NG), LC1339 part
11. [Subtree Averages](#p11) — LC2265 + Q46 (J9933)
12. [Boundary of Binary Tree (list / sum)](#p12) — LC545 + Q25 (C11, H08)
13. [Directions between Two Tree Nodes](#p13) — LC2096 (company tag)
14. [Parent-Array Forest: Delete Node + Re-index](#p14) — Q41 (J11189 NG)
15. [Throne Inheritance without a King](#p15) — LC1600 / Q08 (H01 IC1)
16. [Construct Quad Tree](#p16) — LC427 (company tag)


**Design: KV stores, caches, time windows**

17. [Design In-Memory File System](#p17) — LC588; E01, C11
18. [KV Store with TTL + list valid entries](#p18) — Q27 (C12); LC706, LC1797
19. [Transaction KV: begin / commit / rollback](#p19) — Q28 (C18)
20. [Time Based KV Store (+ out-of-order)](#p20) — LC981, J9934
21. [LRU Cache with resize](#p21) — Q33; LC146
22. [Design Hit Counter](#p22) — LC362; E01 IC1/IC2
23. [Design Log Storage System](#p23) — LC635; E01 IC1/IC2
24. [Event Store: keep latest m, distinct keys before t](#p24) — Q10 (C05), J14594
25. [Integer Stream with a Dynamic Blacklist](#p25) — Q42 (J10688)
26. [Min Stack](#p26) — LC155 (company tag)


**Heaps and ordered sets**

27. [Design Task Manager](#p27) — LC3408
28. [Priority Executor, duplicate IDs run once](#p28) — Q09 (C01), J13542
29. [Meeting Rooms II + buses with stations](#p29) — LC253, Q18 (C06)
30. [Find Median from Data Stream](#p30) — LC295
31. [Top K Frequent Elements](#p31) — LC347
32. [Top K Book Sales (running totals)](#p32) — Q35
33. [Most Frequent IDs](#p33) — LC3092 (company tag)


**Binary search**

34. [Binary Search with Repeated Queries](#p34) — J10556 NG; LC704 / 34 / 35
35. [Find Minimum in Rotated Sorted Array](#p35) — LC153 (J13422), LC154
36. [Maximum Throughput (binary search on the answer)](#p36) — Q34


**Strings and parsing**

37. [Encode and Decode Strings](#p37) — LC271 (C04, J18280)
38. [Basic Calculator I / II](#p38) — LC224, LC227
39. [Simplify Path](#p39) — LC71 (company tag)
40. [Reverse Each Alphanumeric Run](#p40) — Q43 (J10690)
41. [Count Vowel Substrings](#p41) — LC2062 (company tag)


**Arrays and two pointers**

42. [0/1/2 Array: distance from 1s to 2s](#p42) — Q19 / A11 (C09 NG round 2), J10352; LC821, LC243
43. [Trapping Rain Water](#p43) — LC42 (company tag)


**Backtracking**

44. [Word Search II](#p44) — LC212; E01, E02, C12
45. [Work Schedule: all valid schedules](#p45) — Q36


**Warm-ups**

46. [Happy Number](#p46) — LC202 (J10271)
47. [Calculate Amount Paid in Taxes](#p47) — LC2303 (E01)
48. [Maximum Profit Query Selection](#p48) — Q37
49. [Any Path with +2, x2, floor /2](#p49) — Q44 (J10691)


Deliberately not covered: Q02 min deletion set with promotion, Q38, LC1751 (DP); LC380 (weak source); SQL Q48/Q49; AI and Staff material.


---

# Graphs and topological order

<a id="p1"></a>

## 1. Course Schedule II

**Source:** LC210 (+207); E05, C16 · **File:** `course-schedule.cpp`

**Key idea:** Kahn: courses with indegree 0 go first, taking one releases its children; fewer than V taken = cycle. DFS variant: 3-state colouring, reversed postorder. Follow-up: level-by-level output = freeze each frontier.

**Watch:** `[a, b]` means b -> a. Loop `i < order.size()` re-reads the growing vector. DFS must recurse on `nxt`, not `cur`.

```cpp
// n courses, [a, b] means take b before a, return any order finishes all courses

// acyclic graph? cycle detection return?
// dup edge? self-loop?
// validate order (any?)

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class CourseScheduler {
public:
    // Kahn's topological sort, T: O(V+E), S: O(V+E)
    vector<int> findOrder_1(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> graph(n); // from -> [to,]
        vector<int> indeg(n, 0); // course -> its remaining deps cnt
        for (const auto& p : prereqs) {
            int to = p[0];
            int from = p[1];
            graph[from].push_back(to);
            indeg[to]++;
        }

        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                order.push_back(i);
            }
        }

        for (int i = 0; i < static_cast<int>(order.size()); i++) {
            int cur = order[i];
            for (int nxt : graph[cur]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) {
                    order.push_back(nxt);
                }
            }
        }

        if (static_cast<int>(order.size()) != n) { return {}; }
        return order;
    }

    // follow-up: Kahn by level, T: O(V+E), S: O(V+E)
    vector<vector<int>> findLevels(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> graph(n);
        vector<int> indeg(n, 0);
        for (const auto& p : prereqs) {
            int to = p[0];
            int from = p[1];
            graph[from].push_back(to);
            indeg[to]++;
        }

        vector<vector<int>> levels; // levels[k] = courses startable when [0..k) are done
        vector<int> frontier;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                frontier.push_back(i);
            }
        }

        int done = 0;
        while (!frontier.empty()) {
            done += static_cast<int>(frontier.size());
            vector<int> next;
            for (int cur : frontier) {
                for (int nxt : graph[cur]) {
                    indeg[nxt]--;
                    if (indeg[nxt] == 0) {
                        next.push_back(nxt);
                    }
                }
            }
            levels.push_back(frontier);
            frontier = next;
        }

        if (done != n) { return {}; }
        return levels;
    }

    // DFS with cycle detection, T: O(V+E), S: O(V+E)
    vector<int> findOrder_2(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> graph(n); // from -> [to,] 
        for (const auto& p : prereqs) {
            int to = p[0];
            int from = p[1];
            graph[from].push_back(to);
        }

        vector<int> state(n, 0); // 0: un-visited, 1: on-path, 2: visited
        vector<int> order; // post-order, reverse at end
        for (int i = 0; i < n; i++) {
            if (state[i] == 0 && !dfs(i, graph, state, order)) {
                return {};
            }
        }
        std::reverse(order.begin(), order.end());
        return order;
    }

private:
    bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state, vector<int>& order) {
        state[cur] = 1;
        for (int nxt : graph[cur]) {
            if (state[nxt] == 1) {
                return false;
            }
            if (state[nxt] == 0 && !dfs(nxt, graph, state, order)) {
                return false;
            }
        }
        state[cur] = 2;
        order.push_back(cur); // post-order, need reverse
        return true;
    }
};

bool isValid(int n, const vector<vector<int>>& prereqs, const vector<int>& order) {
    if (static_cast<int>(order.size()) != n) {
        return false;
    }
    vector<int> pos(n); // course -> its final idx in order
    for (int i = 0; i < n; i++) {
        pos[order[i]] = i;
    }
    for (const auto& p : prereqs) {
        int to = p[0];
        int from = p[1];
        if (pos[from] > pos[to]) {
            return false;
        }
    }
    return true;
}

int main() {
    CourseScheduler cs;
    vector<vector<int>> diamond = {{1,0}, {2,0}, {3,1}, {3,2}};
    vector<vector<int>> cycle = {{1,0}, {0, 1}};

    assert(isValid(4, diamond, cs.findOrder_1(4, diamond)));
    assert(isValid(4, diamond, cs.findOrder_2(4, diamond)));
    assert(isValid(1, {}, cs.findOrder_1(1, {}))); // no edges
    assert(isValid(1, {}, cs.findOrder_2(1, {})));
    assert(cs.findOrder_1(2, cycle).empty());
    assert(cs.findOrder_1(3, cycle).empty()); // cycle plus a free node
    assert(cs.findOrder_2(2, cycle).empty());
    assert(cs.findOrder_2(3, cycle).empty());

    vector<vector<int>> wantLevels = {{0}, {1, 2}, {3}};
    assert(cs.findLevels(4, diamond) == wantLevels);
    assert(cs.findLevels(3, cycle).empty());
    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p2"></a>

## 2. All Ancestors of a Node in a DAG

**Source:** LC2192; prototype of A8 ACL / B9 · **File:** `ancestor_list.cpp`

**Key idea:** DFS from every source in increasing order with a `visitedBy[v] = src` stamp, so lists come out sorted with no sort. Kahn variant: when u is dequeued its set is final, each child ORs in u's set plus u.

**Watch:** Stamp starts at -1, not 0. Compare the stamp with `src`, not `cur`.

```cpp
// nodes of a DAG, edges [from, to], return for each node its ancestors in increasing order

// acyclic or cycle case?
// dup edges or self-loop?
// output sorted?
// query for all or single node?
// nodes' id range?
// root's ancestor?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

class AncestorFinder {
public:
    // DFS from every nodes as src, T: O(V*(V+E)), S: O(V+E)
    vector<vector<int>> getAncestors_1(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> graph(n); // from -> [to,]
        for (const auto& e : edges) {
            int from = e[0];
            int to = e[1];
            graph[from].push_back(to);
        }

        vector<int> visitedBy(n, -1); // node -> last node reaches it, -1 means none
        vector<vector<int>> ancestors(n); // node -> its ancestors in increasing order
        for (int src = 0; src < n; src++) {
            dfs(src, src, graph, visitedBy, ancestors);
        }
        return ancestors;
    }

private:
    void dfs(int src, int cur, const vector<vector<int>>& graph,
             vector<int>& visitedBy, vector<vector<int>>& ancestors) {
        for (int nxt : graph[cur]) {
            if (visitedBy[nxt] == src) {
                continue;
            }
            visitedBy[nxt] = src;
            ancestors[nxt].push_back(src);
            dfs(src, nxt, graph, visitedBy, ancestors);
        }
    }
    
public:
    // Kahn's order, child ORs in parent's final set, T: O(V*E+V^2), S: O(V^2)
    vector<vector<int>> getAncestors_2(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        vector<int> indeg(n, 0); // node -> parents cnt not processed
        for (const auto& e : edges) {
            int from = e[0];
            int to = e[1];
            graph[from].push_back(to);
            indeg[to]++;
        }

        vector<vector<int>> reach(n, vector<int>(n, 0)); // reach[i][j]=1 iff j is ancestor of i
        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                order.push_back(i);
            }
        }

        for (int i = 0; i < static_cast<int>(order.size()); i++) {
            int cur = order[i]; // all cur's parents are done, reach[cur] is final
            for (int nxt : graph[cur]) {
                reach[nxt][cur] = 1;
                for (int src = 0; src < n; src++) {
                    if (reach[cur][src] == 1) {
                        reach[nxt][src] = 1;
                    }
                }
                indeg[nxt]--;
                if (indeg[nxt] == 0) {
                    order.push_back(nxt);
                }
            }
        }

        vector<vector<int>> ancestors(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) { // ascending scan yields sorted order
                if (reach[i][j] == 1) {
                    ancestors[i].push_back(j);
                }
            }
        }
        return ancestors;
    }
};

int main() {
    AncestorFinder af;
    vector<vector<int>> want;

    // n = 1, no edges
    want = {{}};
    assert(af.getAncestors_1(1, {}) == want);
    assert(af.getAncestors_2(1, {}) == want);

    // diamond: 0 reaches 3 via two paths, listed once
    want = {{}, {0}, {0}, {0,1,2}};
    assert(af.getAncestors_1(4, {{0,1},{0,2},{1,3},{2,3}}) == want);
    assert(af.getAncestors_2(4, {{0,1},{0,2},{1,3},{2,3}}) == want);

    // chain 2 -> 1 -> 0: ancestors larger than the node, need sorted
    want = {{1,2}, {2}, {}};
    assert(af.getAncestors_1(3, {{2,1},{1,0}}) == want);
    assert(af.getAncestors_2(3, {{2,1},{1,0}}) == want);

    // dup edge: ancestors listed once
    want = {{}, {0}};
    assert(af.getAncestors_1(2, {{0,1},{0,1}}) == want);
    assert(af.getAncestors_2(2, {{0,1},{0,1}}) == want);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p3"></a>

## 3. Parallel Courses + variable durations

**Source:** LC1136 (J10557 NG), LC2050 · **File:** `parallel-courses.cpp`

**Key idea:** Semesters = number of Kahn levels. With durations: `start[v] = max(start[v], finish[u])` pushed along edges in topo order; answer = max finish (longest path, not greedy).

**Watch:** 1-indexed input. `total` is the max over all finishes, not the last popped course.

```cpp
// n courses 1..n, relation [prev, next]: prev must finish before next; any number of courses at once
// LC1136: each course takes one semester, return min semesters, -1 on a cycle (J10557 NG)
// LC2050 follow-up: course i takes time[i] months, return min months to finish all

// 1-indexed courses?
// unlimited courses per semester, or at most k?
// cycle: return -1?
// follow-up: next course starts right when its last prereq finishes?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class ParallelCourses {
public:
    // Kahn by level, one level = one semester, T: O(V+E), S: O(V+E)
    int minSemesters(int n, const vector<vector<int>>& relations) {
        vector<vector<int>> graph(n); // course -> courses it unlocks
        vector<int> indeg(n, 0); // course -> prereqs not yet taken
        for (const auto& r : relations) {
            graph[r[0] - 1].push_back(r[1] - 1); // input is 1-indexed
            indeg[r[1] - 1]++;
        }

        vector<int> frontier; // courses takeable this semester
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                frontier.push_back(i);
            }
        }
        int semesters = 0;
        int taken = 0;
        while (!frontier.empty()) {
            semesters++;
            taken += static_cast<int>(frontier.size());
            vector<int> nextFrontier;
            for (int cur : frontier) {
                for (int nxt : graph[cur]) {
                    indeg[nxt]--;
                    if (indeg[nxt] == 0) {
                        nextFrontier.push_back(nxt);
                    }
                }
            }
            frontier = nextFrontier;
        }
        return taken == n ? semesters : -1; // leftovers sit on or behind a cycle
    }

    // follow-up: finish[v] = time[v] + max finish over v's prereqs
    // Kahn, a course's start is final once it is popped, T: O(V+E), S: O(V+E)
    int minMonths(int n, const vector<vector<int>>& relations, const vector<int>& time) {
        vector<vector<int>> graph(n); // course -> courses it unlocks
        vector<int> indeg(n, 0); // course -> prereqs not yet finished
        for (const auto& r : relations) {
            graph[r[0] - 1].push_back(r[1] - 1);
            indeg[r[1] - 1]++;
        }

        vector<int> start(n, 0); // course -> earliest start = latest finish among prereqs seen so far
        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                order.push_back(i);
            }
        }
        int total = 0;
        for (int i = 0; i < static_cast<int>(order.size()); i++) {
            int u = order[i];
            int finish = start[u] + time[u];
            total = std::max(total, finish);
            for (int v : graph[u]) {
                start[v] = std::max(start[v], finish); // v waits for its slowest prereq
                indeg[v]--;
                if (indeg[v] == 0) {
                    order.push_back(v);
                }
            }
        }
        return static_cast<int>(order.size()) == n ? total : -1;
    }
};

int main() {
    ParallelCourses pc;

    assert(pc.minSemesters(3, {{1, 3}, {2, 3}}) == 2); // target: two prereqs taken together
    assert(pc.minSemesters(4, {{1, 2}, {2, 3}}) == 3); // edge: longest chain decides, course 4 rides along
    assert(pc.minSemesters(3, {{1, 2}, {2, 3}, {3, 1}}) == -1); // fault: cycle
    assert(pc.minSemesters(1, {}) == 1); // null: one course, no relations

    assert(pc.minMonths(3, {{1, 3}, {2, 3}}, {3, 2, 5}) == 8); // target: waits for the slower prereq
    assert(pc.minMonths(2, {}, {1, 10}) == 10); // edge: no prereqs, longest course decides
    assert(pc.minMonths(2, {{1, 2}, {2, 1}}, {1, 1}) == -1); // fault: cycle
    assert(pc.minMonths(1, {}, {4}) == 4); // null: one course

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p4"></a>

## 4. DAG Privilege Inheritance (set / ordered)

**Source:** J9549, J12349 NG; Q04/Q05 follow-ups · **File:** `dag-privilege-inheritance.cpp`

**Key idea:** Kahn order, each role pulls from its parents once they are final. Set version: `std::set` union (sorted output). Ordered version: merge parents in GRANT INPUT order, then own, keep first occurrence via a `seen` set.

**Watch:** Merge order must follow `parents[v]` built in grant order, not topo order. Fresh `seen` per role.

```cpp
// roles 0..n-1, grant [u, v] means role v inherits role u; own[r] = role r's own privileges
// J9549: each role's privileges = own + every ancestor's, as a set (sorted for output)
// J12349: ordered: merge parents' results in grant input order, then own in input order, keep first occurrence
// (NG phone screens; Q04/Q05 allow/deny versions are follow-ups)

// [u, v] = v inherits u?
// inherited first or own first? output order: sorted, or first occurrence?
// cycle: return empty, error, or merge the cycle?
// duplicate grants or duplicate privileges?
// deny rules? does a deny keep propagating, can a child re-grant?

#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class PrivilegeResolver {
public:
    // J9549: Kahn order, a role pulls from its parents once they are final
    // T: O(V + (E*P + own) * logP), S: O(V*P), P: max privileges of one role
    vector<vector<string>> resolveUnion(int n, const vector<vector<int>>& grants, const vector<vector<string>>& own) {
        vector<vector<int>> parents(n); // role -> roles it inherits, in grant input order
        vector<int> order = topoOrder(n, grants, parents);
        if (static_cast<int>(order.size()) != n) {
            return {}; // cycle
        }

        vector<std::set<string>> eff(n); // role -> effective privileges
        for (int v : order) {
            eff[v].insert(own[v].begin(), own[v].end());
            for (int p : parents[v]) {
                eff[v].insert(eff[p].begin(), eff[p].end());
            }
        }

        vector<vector<string>> result;
        for (int r = 0; r < n; r++) {
            result.push_back(vector<string>(eff[r].begin(), eff[r].end())); // set iterates sorted
        }
        return result;
    }

    // J12349: same order, but merge sequence follows grant input order, not topo order
    // T: O(V + E*P + own) average, S: O(V*P)
    vector<vector<string>> resolveOrdered(int n, const vector<vector<int>>& grants, const vector<vector<string>>& own) {
        vector<vector<int>> parents(n); // role -> roles it inherits, in grant input order
        vector<int> order = topoOrder(n, grants, parents);
        if (static_cast<int>(order.size()) != n) {
            return {}; // cycle
        }

        vector<vector<string>> eff(n); // role -> effective privileges, first-occurrence order
        for (int v : order) {
            std::unordered_set<string> seen; // privileges already in eff[v]
            for (int p : parents[v]) { // grant input order, eff[p] is final
                for (const string& s : eff[p]) {
                    if (seen.insert(s).second) {
                        eff[v].push_back(s);
                    }
                }
            }
            for (const string& s : own[v]) {
                if (seen.insert(s).second) {
                    eff[v].push_back(s);
                }
            }
        }
        return eff;
    }

private:
    // Kahn; fills parents in grant input order; order.size() < n means a cycle
    vector<int> topoOrder(int n, const vector<vector<int>>& grants, vector<vector<int>>& parents) {
        vector<vector<int>> children(n); // role -> roles inheriting from it
        vector<int> indeg(n, 0); // role -> parents not yet resolved
        for (const auto& g : grants) {
            children[g[0]].push_back(g[1]);
            parents[g[1]].push_back(g[0]);
            indeg[g[1]]++;
        }

        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                order.push_back(i);
            }
        }
        for (int i = 0; i < static_cast<int>(order.size()); i++) {
            for (int c : children[order[i]]) {
                indeg[c]--;
                if (indeg[c] == 0) {
                    order.push_back(c);
                }
            }
        }
        return order;
    }
};

int main() {
    PrivilegeResolver pr;
    vector<vector<string>> own = {{"read", "write"}, {"exec", "read"}, {"audit"}, {"write", "deploy"}};
    vector<vector<string>> want;

    // 2 inherits 0 then 1, 3 inherits 2
    want = {{"read", "write"}, {"exec", "read"}, {"audit", "exec", "read", "write"},
            {"audit", "deploy", "exec", "read", "write"}};
    assert(pr.resolveUnion(4, {{0, 2}, {1, 2}, {2, 3}}, own) == want);

    want = {{"read", "write"}, {"exec", "read"}, {"read", "write", "exec", "audit"},
            {"read", "write", "exec", "audit", "deploy"}};
    assert(pr.resolveOrdered(4, {{0, 2}, {1, 2}, {2, 3}}, own) == want); // dup "read" / "write" keep first spot

    want = {{"read", "write"}, {"exec", "read"}, {"exec", "read", "write", "audit"},
            {"exec", "read", "write", "audit", "deploy"}};
    assert(pr.resolveOrdered(4, {{1, 2}, {0, 2}, {2, 3}}, own) == want); // grant order, not topo order

    want = {{"a"}};
    assert(pr.resolveOrdered(1, {}, {{"a", "a"}}) == want); // single role, duplicate own privilege
    assert(pr.resolveUnion(1, {}, {{"a", "a"}}) == want);

    assert(pr.resolveOrdered(2, {{0, 1}, {1, 0}}, {{"a"}, {"b"}}).empty()); // cycle
    assert(pr.resolveUnion(2, {{0, 1}, {1, 0}}, {{"a"}, {"b"}}).empty());

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p5"></a>

## 5. Possible Bipartition / Is Graph Bipartite

**Source:** LC886, LC785; Q40 contrast · **File:** `bipartition.cpp`

**Key idea:** BFS 2-colouring from every uncoloured node; a neighbour with the same colour = odd cycle. 3-colouring (Q40) is NP-hard: backtracking only.

**Watch:** Add dislike edges both ways. Outer loop over all components.

```cpp
// LC886: n people 1..n, dislikes [a, b]: can everyone be split into two groups with no dislike inside a group?
// LC785: is an undirected graph (adjacency list, 0-indexed) bipartite? same check

// people 1-indexed? dislikes symmetric?
// duplicate pairs? someone disliking themselves?
// graph may be disconnected?
// return only true/false, or the two groups?

#include <vector>
#include <queue>
#include <cassert>
#include <iostream>

using std::vector;

class GraphColoring {
public:
    // LC785: BFS 2-coloring, a neighbor must get the other color, T: O(V+E), S: O(V)
    bool isBipartite(const vector<vector<int>>& graph) {
        int n = static_cast<int>(graph.size());
        vector<int> color(n, -1); // node -> 0 / 1, -1 = not colored yet
        for (int s = 0; s < n; s++) { // every component, not just the one holding node 0
            if (color[s] != -1) {
                continue;
            }
            color[s] = 0;
            std::queue<int> q;
            q.push(s);
            while (!q.empty()) {
                int cur = q.front();
                q.pop();
                for (int nxt : graph[cur]) {
                    if (color[nxt] == -1) {
                        color[nxt] = 1 - color[cur];
                        q.push(nxt);
                    } else if (color[nxt] == color[cur]) {
                        return false; // odd cycle
                    }
                }
            }
        }
        return true;
    }

    // LC886: build the undirected graph, then the same check, T: O(V+E), S: O(V+E)
    bool possibleBipartition(int n, const vector<vector<int>>& dislikes) {
        vector<vector<int>> graph(n); // person -> people they can't share a group with
        for (const auto& d : dislikes) {
            graph[d[0] - 1].push_back(d[1] - 1); // 1-indexed input
            graph[d[1] - 1].push_back(d[0] - 1); // dislike works both ways
        }
        return isBipartite(graph);
    }
};

int main() {
    GraphColoring gc;

    assert(gc.possibleBipartition(4, {{1, 2}, {1, 3}, {2, 4}})); // target: {1, 4} vs {2, 3}
    assert(!gc.possibleBipartition(3, {{1, 2}, {1, 3}, {2, 3}})); // fault: odd cycle
    assert(!gc.possibleBipartition(5, {{1, 2}, {3, 4}, {4, 5}, {3, 5}})); // edge: odd cycle in a second component
    assert(gc.possibleBipartition(1, {})); // null: one person, no dislikes
    assert(!gc.isBipartite({{1, 2, 3}, {0, 2}, {0, 1, 3}, {0, 2}})); // LC785 example

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# BFS: grids and crawling

<a id="p6"></a>

## 6. Shortest Path in a Grid (any start/target)

**Source:** Q32 (C09 NG round 1), LC1091, J10351 · **File:** `grid-shortest-path.cpp`

**Key idea:** BFS: every move costs 1, so the first time the target is popped is the shortest. Why not DFS: it finds a path, not the shortest.

**Watch:** Mark visited on push. Ask 4 vs 8 directions and moves vs cells (LC1091 counts cells, start = 1).

```cpp
// m x n grid of 0 (open) / 1 (wall), shortest path from (sr, sc) to (tr, tc) without stepping on a 1
// return the number of moves, -1 if unreachable (Q32, C09 NG round 1)
// LC1091 variant: top-left to bottom-right, 8 directions, length counted in cells

// 4 or 8 directions?
// distance in moves (edges) or cells on the path?
// start or target on a wall? start == target?
// unreachable return value?
// grid can be empty? m, n bounds?

#include <vector>
#include <queue>
#include <utility>
#include <cassert>
#include <iostream>

using std::vector;

class GridPathFinder {
public:
    // Q32: any start -> target, 4 directions, counts moves
    // BFS, every move costs 1 so the first time a cell is reached is its shortest distance
    // T: O(m*n), S: O(m*n), grid not modified
    int shortestPath(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
        int m = static_cast<int>(grid.size());
        if (m == 0 || grid[sr][sc] == 1 || grid[tr][tc] == 1) {
            return -1;
        }
        int n = static_cast<int>(grid[0].size());
        vector<int> dr = {-1, 1, 0, 0}; // up, down, left, right
        vector<int> dc = {0, 0, -1, 1};

        vector<vector<int>> dist(m, vector<int>(n, -1)); // cell -> moves from start, -1 = not visited
        std::queue<std::pair<int, int>> q; // cells in BFS order, distance non-decreasing
        dist[sr][sc] = 0;
        q.push({sr, sc});
        while (!q.empty()) {
            int r = q.front().first;
            int c = q.front().second;
            q.pop();
            if (r == tr && c == tc) {
                return dist[r][c];
            }
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
                    continue;
                }
                if (grid[nr][nc] == 1 || dist[nr][nc] != -1) {
                    continue;
                }
                dist[nr][nc] = dist[r][c] + 1; // mark on push, else a cell is queued many times
                q.push({nr, nc});
            }
        }
        return -1;
    }

    // LC1091: (0, 0) -> (n-1, n-1) in an n x n grid, 8 directions, counts cells on the path
    // same BFS, start cell counts as 1, T: O(n^2), S: O(n^2)
    int shortestPathBinaryMatrix(const vector<vector<int>>& grid) {
        int n = static_cast<int>(grid.size());
        if (n == 0 || grid[0][0] == 1 || grid[n - 1][n - 1] == 1) {
            return -1;
        }
        vector<int> dr = {-1, -1, -1, 0, 0, 1, 1, 1}; // all 8 neighbours
        vector<int> dc = {-1, 0, 1, -1, 1, -1, 0, 1};

        vector<vector<int>> dist(n, vector<int>(n, -1)); // cell -> cells on the path so far, -1 = not visited
        std::queue<std::pair<int, int>> q;
        dist[0][0] = 1; // the start cell itself
        q.push({0, 0});
        while (!q.empty()) {
            int r = q.front().first;
            int c = q.front().second;
            q.pop();
            if (r == n - 1 && c == n - 1) {
                return dist[r][c];
            }
            for (int d = 0; d < 8; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
                    continue;
                }
                if (grid[nr][nc] == 1 || dist[nr][nc] != -1) {
                    continue;
                }
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
        return -1;
    }
};

int main() {
    GridPathFinder gp;
    vector<vector<int>> wall = {{0, 0, 0},
                                {1, 1, 0},
                                {0, 0, 0}};

    assert(gp.shortestPath(wall, 0, 0, 2, 0) == 6); // detour around the wall
    assert(gp.shortestPath(wall, 1, 2, 1, 2) == 0); // start == target
    assert(gp.shortestPath({{0, 1}, {1, 0}}, 0, 0, 1, 1) == -1); // sealed off in 4 directions
    assert(gp.shortestPath({{1, 0}}, 0, 0, 0, 1) == -1); // start on a wall

    assert(gp.shortestPathBinaryMatrix({{0, 1}, {1, 0}}) == 2); // LC example, diagonal, counts cells
    assert(gp.shortestPathBinaryMatrix({{0, 0, 0}, {1, 1, 0}, {1, 1, 0}}) == 4); // LC example
    assert(gp.shortestPathBinaryMatrix({{1, 0, 0}, {1, 1, 0}, {1, 1, 0}}) == -1); // blocked start
    assert(gp.shortestPathBinaryMatrix({{0}}) == 1); // n = 1, start is the target

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p7"></a>

## 7. Web Crawler / Fewest Clicks

**Source:** LC1236, Q06 (H01 IC1), J15972, J12933 · **File:** `web-crawler.cpp`

**Key idea:** BFS over pages discovered via getUrls; `parent` map is both visited set and path record; walk back from target for the path.

**Watch:** Same-host filter only if asked. `parent.count`, not `[]`. Follow-ups: API failure/retry, multithreaded BFS with atomic visited + in-flight counter.

```cpp
// pages and links are discovered only through getUrls(url)
// LC1236: return every page reachable from startUrl on the same host
// Q06 / J15972: fewest clicks from start to target, return the path, empty if unreachable

// same host only? (LC1236 yes, Q06 ask)
// URL normalization: trailing slash, #fragment, http vs https, case?
// start == target? unreachable return?
// getUrls can fail or be slow: retry, skip, or abort?
// return the click count, or the path too?

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

struct HtmlParser { // test double for the given links API
    std::unordered_map<string, vector<string>> links; // page -> links on that page

    vector<string> getUrls(const string& url) const {
        auto it = links.find(url);
        return it == links.end() ? vector<string>{} : it->second;
    }
};

class WebCrawler {
public:
    // BFS over pages on startUrl's host, T: O(V+E) getUrls calls and checks, S: O(V)
    vector<string> crawl(const string& startUrl, const HtmlParser& parser) {
        string host = hostOf(startUrl);
        std::unordered_set<string> seen = {startUrl}; // pages already queued
        std::queue<string> q;
        q.push(startUrl);
        vector<string> result;
        while (!q.empty()) {
            string cur = q.front();
            q.pop();
            result.push_back(cur);
            for (const string& nxt : parser.getUrls(cur)) {
                if (hostOf(nxt) == host && seen.insert(nxt).second) { // mark on push
                    q.push(nxt);
                }
            }
        }
        return result;
    }

    // BFS, each click costs 1 so the first time target is popped is the fewest clicks
    // T: O(V+E), S: O(V)
    vector<string> shortestPath(const string& start, const string& target, const HtmlParser& parser) {
        std::unordered_map<string, string> parent; // page -> page clicked from, doubles as visited
        parent[start] = start; // start points to itself, stops the walk back
        std::queue<string> q;
        q.push(start);
        while (!q.empty()) {
            string cur = q.front();
            q.pop();
            if (cur == target) {
                vector<string> path;
                for (string p = target; p != start; p = parent[p]) {
                    path.push_back(p);
                }
                path.push_back(start);
                std::reverse(path.begin(), path.end());
                return path;
            }
            for (const string& nxt : parser.getUrls(cur)) {
                if (parent.count(nxt) == 0) {
                    parent[nxt] = cur;
                    q.push(nxt);
                }
            }
        }
        return {};
    }

private:
    // "http://host/path" -> "host"
    string hostOf(const string& url) {
        size_t end = url.find('/', 7); // skip "http://"
        return url.substr(7, end - 7); // npos - 7 still clamps to the end
    }
};

int main() {
    WebCrawler wc;
    HtmlParser web;
    web.links = {{"http://a.com/", {"http://a.com/x", "http://b.com/"}},
                 {"http://a.com/x", {"http://a.com/", "http://a.com/y"}}};
    vector<string> want;

    want = {"http://a.com/", "http://a.com/x", "http://a.com/y"};
    vector<string> got = wc.crawl("http://a.com/", web);
    std::sort(got.begin(), got.end()); // any order is valid
    assert(got == want); // target: other host skipped, link back to root doesn't loop
    want = {"http://lonely.com/"};
    assert(wc.crawl("http://lonely.com/", web) == want); // null: page with no links

    want = {"http://a.com/", "http://a.com/x", "http://a.com/y"};
    assert(wc.shortestPath("http://a.com/", "http://a.com/y", web) == want); // target: 2 clicks
    want = {"http://a.com/"};
    assert(wc.shortestPath("http://a.com/", "http://a.com/", web) == want); // edge: start == target
    assert(wc.shortestPath("http://a.com/", "http://c.com/", web).empty()); // fault: unreachable

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Trees

<a id="p8"></a>

## 8. Tree Height after Deleting Nodes (promotion)

**Source:** LC559 + Q01 (C02) · **File:** `tree-height-deleted-nodes.cpp`

**Key idea:** Bottom-up: height = max child height + (deleted ? 0 : 1); a deleted node passes its child's height up, which is promotion. Explicit-stack version for 10k-deep chains.

**Watch:** Use `count`, not `[]` on the delete set. Q02 (min deletion set) is a separate DP problem: never present a greedy.

```cpp
// multi-way tree + delete list: a deleted node adds 0 to a path, its children move up to its parent
// return the height counted in nodes, root is never deleted, values are unique

#include <vector>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::unordered_set;

struct Node {
    int val;
    vector<Node> children;
};

class TreeHeightCalculator {
public:
    // bottom-up DFS, delete node adds 0, T: O(V+D), S: O(D+H), D: size of delete list, H: tree height
    int treeHeight_1(const Node& root, const vector<int>& deleted) {
        unordered_set<int> removed(deleted.begin(), deleted.end()); // ids to delete, O(1) lookup
        return dfs(root, removed);
    }

private:
    int dfs(const Node& cur, const unordered_set<int>& removed) {
        int maxH = 0; // tallest child path
        for (const Node& child : cur.children) {
            maxH = std::max(maxH, dfs(child, removed));
        }
        return maxH + (removed.count(cur.val) ? 0 : 1); // deleted adds 0
    }

public:
    // top-down explicit stack, T: (V+D), S: O(V+D)
    int treeHeight_2(const Node& root, const vector<int>& deleted) {
        unordered_set<int> removed(deleted.begin(), deleted.end());
        int maxH = 0;
        vector<std::pair<const Node*, int>> stk; // {node, height of root..node}
        stk.push_back({&root, 1}); // root is never deleted
        while (!stk.empty()) {
            auto [cur, h] = stk.back();
            stk.pop_back();
            maxH = std::max(maxH, h);
            for (const Node& child : cur->children) {
                stk.push_back({&child, h + (removed.count(child.val) ? 0 : 1)});
            }
        }
        return maxH;
    }
};

int main() {
    TreeHeightCalculator th;

    // 1 -> {2, 3}, 3 -> 4, 4 -> 5, longest path 1-3-4-5
    Node root{1, {{2, {}}, {3, {{4, {{5, {}}}}}}}};

    assert(th.treeHeight_1(root, {}) == 4); // no del
    assert(th.treeHeight_2(root, {}) == 4);

    assert(th.treeHeight_1(root, {3}) == 3); // del 3, make 4 promoted under 1, path 1-4-5
    assert(th.treeHeight_2(root, {3}) == 3);

    assert(th.treeHeight_1(root, {3,4}) == 2); // del 3,4, promote 5 under 1, path 1-5
    assert(th.treeHeight_2(root, {3,4}) == 2);

    assert(th.treeHeight_1(root, {2,3,4,5}) == 1); // del all except root
    assert(th.treeHeight_2(root, {2,3,4,5}) == 1);

    Node single{1, {}};
    assert(th.treeHeight_1(single, {}) == 1); // single root
    assert(th.treeHeight_2(single, {}) == 1); // single root

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p9"></a>

## 9. Min Whole-Subtree Deletions to Bound Depth

**Source:** LC559 + J12962 · **File:** `subtree-delete-depth.cpp`

**Key idea:** Each root child whose subtree goes deeper than k needs >= 1 deletion inside it, and deleting that child clears all of it: answer = count of such children (proven by disjointness).

**Watch:** Whole-subtree deletion, not promotion. k < 1 is impossible.

```cpp
// n-ary tree: maxDepth (LC559), then the fewest deletions so depth <= k (J12962)
// deleting a node removes its WHOLE subtree (not promotion); the root can't be deleted; depth counts nodes

// deletion removes the subtree, or promotes the children? (promotion = Q02, a different problem)
// minimize the number of deletions chosen, or the nodes removed?
// depth in nodes or edges? root alone = 1?
// k < 1: impossible, return -1?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

struct Node {
    int val;
    vector<Node> children;
};

class DepthLimiter {
public:
    // T: O(n), S: O(h)
    int maxDepth(const Node& node) {
        int best = 0; // tallest child
        for (const Node& c : node.children) {
            best = std::max(best, maxDepth(c));
        }
        return best + 1;
    }

    // each root child whose subtree goes deeper than k needs at least one deletion inside it,
    // and deleting that child itself is one deletion that clears all of it -> count those children
    // T: O(n), S: O(h)
    int minDeletions(const Node& root, int k) {
        if (k < 1) {
            return -1; // root alone already has depth 1 and can't be deleted
        }
        int deletions = 0;
        for (const Node& c : root.children) {
            if (1 + maxDepth(c) > k) {
                deletions++;
            }
        }
        return deletions;
    }
};

int main() {
    DepthLimiter dl;
    Node tree{1, {{2, {{4, {{5, {}}}}}}, {3, {}}}}; // 1 -> {2 -> 4 -> 5, 3}

    assert(dl.maxDepth(tree) == 4); // LC559 part
    assert(dl.minDeletions(tree, 2) == 1); // target: only child 2's subtree is too deep
    assert(dl.minDeletions(tree, 4) == 0); // edge: already within k
    assert(dl.minDeletions(tree, 1) == 2); // edge: k = 1 keeps only the root
    assert(dl.minDeletions(tree, 0) == -1); // fault: impossible bound
    assert(dl.minDeletions(Node{7, {}}, 1) == 0); // null: single node

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p10"></a>

## 10. New Tree of Subtree Sums

**Source:** Q45 (J10558 NG), LC1339 part · **File:** `subtree-sum-tree.cpp`

**Key idea:** Postorder: build children first; a new child's value already is its subtree sum, so a node = own value + new children's values. O(n) vs O(n^2) rescan baseline.

**Watch:** `long long` sums. Thread-pool follow-up: pending counters per parent, never block a worker on child futures.

```cpp
// build a new tree with the same shape, each new node's value = sum of the original node's subtree
// original tree must stay unchanged (Q45, J10558 NG); follow-up: huge tree, bounded thread pool

// binary or n-ary? node type given?
// sums can overflow int? negative values?
// empty tree return?
// who owns the new tree? (value-type children here, nothing to free)
// recursion depth OK, or very deep chains?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

struct Node {
    long long val; // wide so subtree sums don't overflow
    vector<Node> children;

    bool operator==(const Node& o) const {
        return val == o.val && children == o.children; // whole-tree compare for tests
    }
};

class SubtreeSumBuilder {
public:
    // postorder: build children first, a node's sum = its value + its new children's values
    // T: O(n), S: O(n) new tree + O(h) recursion, input not modified
    Node buildSums_1(const Node& cur) {
        Node copy{cur.val, {}};
        for (const Node& child : cur.children) {
            copy.children.push_back(buildSums_1(child));
            copy.val += copy.children.back().val; // child's new value is already its subtree sum
        }
        return copy;
    }

    // baseline: rescan each node's subtree, T: O(n*h), O(n^2) on a chain, S: O(n) + O(h)
    Node buildSums_2(const Node& cur) {
        Node copy{subtreeSum(cur), {}};
        for (const Node& child : cur.children) {
            copy.children.push_back(buildSums_2(child));
        }
        return copy;
    }

private:
    long long subtreeSum(const Node& cur) {
        long long sum = cur.val;
        for (const Node& child : cur.children) {
            sum += subtreeSum(child);
        }
        return sum;
    }
};

int main() {
    SubtreeSumBuilder sb;

    Node single{5, {}};
    assert(sb.buildSums_1(single) == single); // single node keeps its value
    assert(sb.buildSums_2(single) == single);

    // 1 -> {2 -> {4, 5}, 3}
    Node tree{1, {{2, {{4, {}}, {5, {}}}}, {3, {}}}};
    Node want{15, {{11, {{4, {}}, {5, {}}}}, {3, {}}}};
    Node before = tree;
    assert(sb.buildSums_1(tree) == want);
    assert(sb.buildSums_2(tree) == want);
    assert(tree == before); // original unchanged

    Node chain{1, {{2, {{3, {}}}}}}; // 1 -> 2 -> 3
    Node wantChain{6, {{5, {{3, {}}}}}};
    assert(sb.buildSums_1(chain) == wantChain);
    assert(sb.buildSums_2(chain) == wantChain);

    Node neg{0, {{-3, {}}, {3, {}}}}; // negatives cancel at the root
    assert(sb.buildSums_1(neg) == neg);
    assert(sb.buildSums_2(neg) == neg);

    Node big{2000000000, {{2000000000, {}}}}; // sum passes INT_MAX
    Node wantBig{4000000000LL, {{2000000000, {}}}};
    assert(sb.buildSums_1(big) == wantBig);
    assert(sb.buildSums_2(big) == wantBig);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p11"></a>

## 11. Subtree Averages

**Source:** LC2265 + Q46 (J9933) · **File:** `subtree-average.cpp`

**Key idea:** One postorder returning (sum, count). 2265 counts nodes equal to their floored subtree average; Q46 floors the average of all subtree averages and compares with root.

**Watch:** C++ `/` truncates toward zero: use floorDiv for negatives. Q46 differs from root == whole-tree average (chain 1->0->4).

```cpp
// LC2265: count nodes whose value equals the floor average of their own subtree
// Q46 (J9933): take every subtree's floor average, then the floor average of those n averages,
// and check whether it equals root's value (not the same as root == whole-tree average)

// both averages floored? (C++ / truncates toward zero, differs on negatives)
// values can be negative?
// empty tree: 0 / false?
// sums beyond int?

#include <utility>
#include <cassert>
#include <iostream>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class SubtreeAverage {
private:
    int matches = 0; // LC2265 answer
    long long avgSum = 0; // Q46: sum of every subtree's floor average
    long long nodes = 0; // Q46: n

public:
    // postorder returns (sum, count) of each subtree, T: O(n), S: O(h)
    int averageOfSubtree(TreeNode* root) {
        matches = 0;
        dfs(root);
        return matches;
    }

    // Q46, same postorder, T: O(n), S: O(h)
    bool rootEqualsAverageOfAverages(TreeNode* root) {
        if (root == nullptr) {
            return false;
        }
        avgSum = 0;
        nodes = 0;
        dfs(root);
        return root->val == floorDiv(avgSum, nodes);
    }

private:
    std::pair<long long, long long> dfs(TreeNode* node) {
        if (node == nullptr) {
            return {0, 0};
        }
        std::pair<long long, long long> l = dfs(node->left);
        std::pair<long long, long long> r = dfs(node->right);
        long long sum = l.first + r.first + node->val;
        long long count = l.second + r.second + 1;
        long long avg = floorDiv(sum, count);
        if (avg == node->val) {
            matches++;
        }
        avgSum += avg;
        nodes++;
        return {sum, count};
    }

    // floor division; plain / rounds toward zero, which is wrong for negative sums
    long long floorDiv(long long a, long long b) {
        long long q = a / b;
        if (a % b != 0 && (a < 0) != (b < 0)) {
            q--;
        }
        return q;
    }
};

int main() {
    SubtreeAverage sa;

    TreeNode a1(1), a0(0), a4(4); // chain 1 -> 0 -> 4
    a1.left = &a0;
    a0.left = &a4;
    assert(sa.averageOfSubtree(&a1) == 2); // target: 4 == 4 and 1 == 5/3, 0 != 2
    assert(!sa.rootEqualsAverageOfAverages(&a1)); // Q46 target: averages 1, 2, 4 -> 2, root is 1

    TreeNode n1(-1), n2(0); // -1 -> 0
    n1.left = &n2;
    assert(sa.averageOfSubtree(&n1) == 2); // edge: floor(-1/2) = -1, truncation would give 0
    assert(sa.rootEqualsAverageOfAverages(&n1)); // averages -1, 0 -> floor(-1/2) = -1 == root

    assert(sa.averageOfSubtree(nullptr) == 0); // null
    assert(!sa.rootEqualsAverageOfAverages(nullptr));

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p12"></a>

## 12. Boundary of Binary Tree (list / sum)

**Source:** LC545 + Q25 (C11, H08) · **File:** `tree-boundary.cpp`

**Key idea:** Root, left side top-down (stop before leaves), leaves left to right, right side bottom-up (stop before leaves). Each node appears once, so the sum can't double count.

**Watch:** Single node = root only. Comments ending in `\` are line continuations.

```cpp
// binary tree boundary, counter-clockwise from the root: root, left boundary top-down,
// leaves left to right, right boundary bottom-up; every node at most once (LC545)
// Q25 variant: return the sum of the boundary values instead of the list

// order counter-clockwise starting at root?
// root with no left child: no left boundary at all (LC rule)?
// a leaf that is also on a side: counted once, as a leaf?
// complete / triangle tree guaranteed, or any shape?
// return the list or the sum? values can overflow int?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class TreeBoundary {
public:
    // T: O(n), S: O(h) recursion for the leaves + O(n) output
    vector<int> boundary(TreeNode* root) {
        vector<int> out;
        if (root == nullptr) {
            return out;
        }
        out.push_back(root->val);
        if (isLeaf(root)) {
            return out; // single node: root is not also a leaf
        }

        // left side top-down, stop before the leaf so it isn't counted twice
        for (TreeNode* cur = root->left; cur != nullptr && !isLeaf(cur); cur = cur->left ? cur->left : cur->right) {
            out.push_back(cur->val);
        }
        addLeaves(root, out);
        vector<int> right; // right side top-down, appended reversed
        for (TreeNode* cur = root->right; cur != nullptr && !isLeaf(cur); cur = cur->right ? cur->right : cur->left) {
            right.push_back(cur->val);
        }
        out.insert(out.end(), right.rbegin(), right.rend());
        return out;
    }

    // Q25: each boundary node appears once in the list, so summing it can't double count
    long long boundarySum(TreeNode* root) {
        long long sum = 0;
        for (int v : boundary(root)) {
            sum += v;
        }
        return sum;
    }

private:
    bool isLeaf(TreeNode* node) {
        return node->left == nullptr && node->right == nullptr;
    }

    void addLeaves(TreeNode* node, vector<int>& out) { // preorder: leaves left to right
        if (node == nullptr) {
            return;
        }
        if (isLeaf(node)) {
            out.push_back(node->val);
            return;
        }
        addLeaves(node->left, out);
        addLeaves(node->right, out);
    }
};

int main() {
    TreeBoundary tb;
    vector<int> want;

    // 1 -> (2, 3), 2 -> (4, 5), 5 -> left 7, 3 -> right 6
    TreeNode n1(1), n2(2), n3(3), n4(4), n5(5), n6(6), n7(7); // stack nodes, nothing to free
    n1.left = &n2;
    n1.right = &n3;
    n2.left = &n4;
    n2.right = &n5;
    n5.left = &n7;
    n3.right = &n6;
    want = {1, 2, 4, 7, 6, 3};
    assert(tb.boundary(&n1) == want); // target: inner node 5 skipped, leaves 4 7 6
    assert(tb.boundarySum(&n1) == 23);

    TreeNode c1(1), c2(2), c3(3); // 1 -> right 2 -> right 3
    c1.right = &c2;
    c2.right = &c3;
    want = {1, 3, 2};
    assert(tb.boundary(&c1) == want); // edge: no left child, leaf 3 on the right side counted once

    TreeNode solo(5);
    want = {5};
    assert(tb.boundary(&solo) == want); // edge: single node is root only, not also a leaf

    assert(tb.boundary(nullptr).empty()); // null: empty tree
    assert(tb.boundarySum(nullptr) == 0);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p13"></a>

## 13. Directions between Two Tree Nodes

**Source:** LC2096 (company tag) · **File:** `tree-directions.cpp`

**Key idea:** Root->start and root->dest as L/R strings by backtracking; drop the common prefix (path to the LCA); leftover start moves become 'U'.

**Watch:** On a failed left search overwrite `path.back()` with 'R' before searching right.

```cpp
// binary tree with unique values; shortest directions from node startValue to node destValue
// 'L' = go to left child, 'R' = right child, 'U' = parent (LC2096)

// values unique? both start and dest always present?
// start == dest: empty string?
// any shortest path, or this exact letter format?
// how deep can the tree get?

#include <string>
#include <cassert>
#include <iostream>

using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class TreeNavigator {
public:
    // root->start and root->dest as L/R strings; drop the shared prefix (path down to the LCA)
    // what is left of the start path becomes 'U's, what is left of the dest path is walked as is
    // T: O(n), S: O(h) recursion + paths
    string getDirections(TreeNode* root, int startValue, int destValue) {
        string toStart;
        string toDest;
        findPath(root, startValue, toStart);
        findPath(root, destValue, toDest);

        int common = 0; // length of the shared prefix
        while (common < static_cast<int>(toStart.size()) && common < static_cast<int>(toDest.size()) &&
               toStart[common] == toDest[common]) {
            common++;
        }
        return string(toStart.size() - common, 'U') + toDest.substr(common);
    }

private:
    // backtracking: path holds the moves from root to node; false means target is not below node
    bool findPath(TreeNode* node, int target, string& path) {
        if (node == nullptr) {
            return false;
        }
        if (node->val == target) {
            return true;
        }
        path.push_back('L');
        if (findPath(node->left, target, path)) {
            return true;
        }
        path.back() = 'R';
        if (findPath(node->right, target, path)) {
            return true;
        }
        path.pop_back();
        return false;
    }
};

int main() {
    TreeNavigator tn;

    // 5 -> (1, 2), 1 -> left 3, 2 -> (6, 4)
    TreeNode n5(5), n1(1), n2(2), n3(3), n6(6), n4(4); // stack nodes, nothing to free
    n5.left = &n1;
    n5.right = &n2;
    n1.left = &n3;
    n2.left = &n6;
    n2.right = &n4;

    assert(tn.getDirections(&n5, 3, 6) == "UURL"); // target: LC example, up to root then down
    assert(tn.getDirections(&n5, 5, 4) == "RR"); // edge: start is an ancestor, no 'U'
    assert(tn.getDirections(&n5, 6, 2) == "U"); // edge: dest is an ancestor, only 'U'
    assert(tn.getDirections(&n5, 4, 4) == ""); // null: start == dest

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p14"></a>

## 14. Parent-Array Forest: Delete Node + Re-index

**Source:** Q41 (J11189 NG) · **File:** `forest-delete-node.cpp`

**Key idea:** Decide each survivor's parent in OLD indices (child of x -> parent[x], or itself if x was a root), then map old -> new with `p < x ? p : p - 1`.

**Watch:** Never fix parents while erasing; the promoted parent index also needs remapping.

```cpp
// forest as a parent array, parent[i] == i means i is a root; delete node x
// x's direct children move up to parent[x]; if x is a root, each child becomes its own root
// survivors are re-indexed in original order and every parent value is renumbered (Q41, J11189 NG)

// root encoded as parent[i] == i, or -1?
// x is a root: children become separate roots?
// keep survivors in original order?
// return only the new parent array, or payload / old->new map too?
// input always a valid forest (no cycles)? x in range?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

class ForestEditor {
public:
    // pick each survivor's parent in OLD indices, then shift every index past x down by one
    // T: O(n), S: O(n) for the output, input not modified
    vector<int> deleteNode(const vector<int>& parent, int x) {
        int n = static_cast<int>(parent.size());
        bool xIsRoot = parent[x] == x;
        vector<int> result; // new parent array, n - 1 entries
        for (int i = 0; i < n; i++) {
            if (i == x) {
                continue;
            }
            int p = parent[i]; // old index
            if (p == x) {
                p = xIsRoot ? i : parent[x]; // child of x: becomes a root, or moves up to x's parent
            }
            result.push_back(p < x ? p : p - 1); // old -> new index, p is never x here
        }
        return result;
    }
};

int main() {
    ForestEditor fe;
    vector<int> want;

    assert(fe.deleteNode({0}, 0).empty()); // only node

    want = {0, 1};
    assert(fe.deleteNode({0, 0, 0}, 0) == want); // root with two children, each becomes a root

    want = {0, 0, 1};
    assert(fe.deleteNode({0, 0, 1, 2}, 1) == want); // chain 0 <- 1 <- 2 <- 3, 2 moves up to 0, 3 keeps 2 renumbered

    want = {0, 0};
    assert(fe.deleteNode({0, 0, 1}, 2) == want); // x is the last index, nothing shifts

    want = {0, 1, 1, 2};
    assert(fe.deleteNode({0, 0, 2, 2, 3}, 1) == want); // two trees, every index after x shifts down

    want = {0, 0};
    assert(fe.deleteNode({1, 1, 0}, 0) == want); // x's parent has a larger index, must be renumbered too

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p15"></a>

## 15. Throne Inheritance without a King

**Source:** LC1600 / Q08 (H01 IC1) · **File:** `throne-inheritance.cpp`

**Key idea:** Hash-map adjacency (person -> children in birth order) + dead set; order = preorder skipping the dead. First birth's parent becomes the king (confirm the contract).

**Watch:** Death only marks, never removes. `children.find`, not `[]`.

```cpp
// birth(parent, child), death(name), getInheritanceOrder() (Q08, LC1600 variant, H01 IC1)
// no king given at construction: the parent in the first birth becomes the king
// order = preorder from the king, children in birth order, dead people skipped but their children kept

// how is the king created: first birth's parent, or a separate call?
// birth with an unknown parent: reject, or start a new tree?
// names unique and non-empty? a child born twice?
// dead person's children still inherit?
// how deep can the family tree get?

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

// T: birth / death O(1) average, getInheritanceOrder O(n), S: O(n)

class ThroneInheritance {
private:
    string king; // empty until the first birth names one
    std::unordered_set<string> people; // everyone known, king included
    std::unordered_map<string, vector<string>> children; // person -> children in birth order
    std::unordered_set<string> dead;

public:
    bool birth(const string& parent, const string& child) {
        if (king.empty()) {
            king = parent; // first birth creates the root
            people.insert(parent);
        }
        if (people.count(parent) == 0 || people.count(child) != 0) {
            return false; // unknown parent, or name already used
        }
        children[parent].push_back(child);
        people.insert(child);
        return true;
    }

    void death(const string& name) {
        dead.insert(name); // stays in the tree, only skipped in the order
    }

    vector<string> getInheritanceOrder() {
        vector<string> order;
        if (!king.empty()) {
            preorder(king, order);
        }
        return order;
    }

private:
    void preorder(const string& person, vector<string>& order) {
        if (dead.count(person) == 0) {
            order.push_back(person);
        }
        auto it = children.find(person); // find, not [], a leaf must not get an empty entry
        if (it == children.end()) {
            return;
        }
        for (const string& c : it->second) {
            preorder(c, order);
        }
    }
};

int main() {
    ThroneInheritance t;
    vector<string> want;

    assert(t.getInheritanceOrder().empty()); // null: no king yet

    t.birth("king", "andy");
    t.birth("king", "bob");
    t.birth("andy", "matt");
    want = {"king", "andy", "matt", "bob"};
    assert(t.getInheritanceOrder() == want); // target: andy's line before bob

    t.death("andy");
    want = {"king", "matt", "bob"};
    assert(t.getInheritanceOrder() == want); // edge: dead skipped, his child keeps the spot

    assert(!t.birth("ghost", "x")); // fault: unknown parent rejected
    assert(t.getInheritanceOrder() == want);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p16"></a>

## 16. Construct Quad Tree

**Source:** LC427 (company tag) · **File:** `quad-tree.cpp`

**Key idea:** 1: scan region, leaf if uniform else split (O(n^2 log n)). 2: build children first, merge 4 same-valued leaves (O(n^2)). Nodes in an index pool.

**Watch:** Merge only if all four are leaves AND equal. Store indices, not references, across pool growth.

```cpp
// n x n grid of 0/1, n a power of 2: build its quad tree (LC427)
// a region of all-equal cells is one leaf with that value; otherwise split into 4 quadrants

// n always a power of 2? n >= 1?
// node format given? (here a pool of nodes linked by index, nothing to free)
// value of a non-leaf node: anything?
// non-square / non-power-of-2 grids later?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

struct QuadNode {
    bool val = false;
    bool isLeaf = false;
    int topLeft = -1; // child indices into the pool, -1 for a leaf
    int topRight = -1;
    int bottomLeft = -1;
    int bottomRight = -1;
};

class QuadTreeBuilder {
private:
    vector<QuadNode> pool; // every node built, root returned by index

public:
    // 1: check whether the region is uniform by scanning it, split if not
    // T: O(n^2 logn), each level rescans its cells, S: O(n^2) nodes + O(logn) recursion
    int construct_1(const vector<vector<int>>& grid) {
        pool.clear();
        return build_1(grid, 0, 0, static_cast<int>(grid.size()));
    }

    // 2: build the 4 children first, merge them into one leaf if all are leaves with the same value
    // T: O(n^2), every cell is a leaf once and every merge is O(1), S: O(n^2) nodes + O(logn) recursion
    int construct_2(const vector<vector<int>>& grid) {
        pool.clear();
        return build_2(grid, 0, 0, static_cast<int>(grid.size()));
    }

    const QuadNode& node(int i) const {
        return pool[i];
    }

private:
    int makeLeaf(bool val) {
        QuadNode leaf;
        leaf.val = val;
        leaf.isLeaf = true;
        pool.push_back(leaf);
        return static_cast<int>(pool.size()) - 1;
    }

    int makeInner(int tl, int tr, int bl, int br) {
        QuadNode inner;
        inner.topLeft = tl;
        inner.topRight = tr;
        inner.bottomLeft = bl;
        inner.bottomRight = br;
        pool.push_back(inner); // indices, not references: safe across reallocation
        return static_cast<int>(pool.size()) - 1;
    }

    int build_1(const vector<vector<int>>& grid, int r, int c, int size) {
        bool uniform = true;
        for (int i = r; i < r + size && uniform; i++) {
            for (int j = c; j < c + size; j++) {
                if (grid[i][j] != grid[r][c]) {
                    uniform = false;
                    break;
                }
            }
        }
        if (uniform) {
            return makeLeaf(grid[r][c] == 1);
        }
        int half = size / 2;
        int tl = build_1(grid, r, c, half);
        int tr = build_1(grid, r, c + half, half);
        int bl = build_1(grid, r + half, c, half);
        int br = build_1(grid, r + half, c + half, half);
        return makeInner(tl, tr, bl, br);
    }

    int build_2(const vector<vector<int>>& grid, int r, int c, int size) {
        if (size == 1) {
            return makeLeaf(grid[r][c] == 1);
        }
        int half = size / 2;
        int tl = build_2(grid, r, c, half);
        int tr = build_2(grid, r, c + half, half);
        int bl = build_2(grid, r + half, c, half);
        int br = build_2(grid, r + half, c + half, half);
        bool same = pool[tl].isLeaf && pool[tr].isLeaf && pool[bl].isLeaf && pool[br].isLeaf &&
                    pool[tl].val == pool[tr].val && pool[tl].val == pool[bl].val && pool[tl].val == pool[br].val;
        if (same) {
            return makeLeaf(pool[tl].val); // the 4 children stay in the pool, just unreferenced
        }
        return makeInner(tl, tr, bl, br);
    }
};

// test helper: leaves reachable from node i
int countLeaves(const QuadTreeBuilder& qb, int i) {
    const QuadNode& n = qb.node(i);
    if (n.isLeaf) {
        return 1;
    }
    return countLeaves(qb, n.topLeft) + countLeaves(qb, n.topRight) +
           countLeaves(qb, n.bottomLeft) + countLeaves(qb, n.bottomRight);
}

int main() {
    QuadTreeBuilder qb;
    vector<vector<int>> mixed = {{1, 1, 0, 1},
                                 {1, 1, 0, 0},
                                 {1, 1, 1, 1},
                                 {1, 1, 1, 1}};
    int root;

    root = qb.construct_1(mixed);
    assert(countLeaves(qb, root) == 7); // target: only the top-right quadrant splits again
    root = qb.construct_2(mixed);
    assert(countLeaves(qb, root) == 7);

    root = qb.construct_1({{0, 0}, {0, 0}});
    assert(qb.node(root).isLeaf && !qb.node(root).val); // edge: uniform grid is one leaf
    root = qb.construct_2({{0, 0}, {0, 0}});
    assert(qb.node(root).isLeaf && !qb.node(root).val);

    root = qb.construct_1({{1, 0}, {1, 1}});
    assert(countLeaves(qb, root) == 4); // edge: one different cell forces a split
    root = qb.construct_2({{1, 0}, {1, 1}});
    assert(countLeaves(qb, root) == 4);

    root = qb.construct_1({{1}});
    assert(qb.node(root).isLeaf && qb.node(root).val); // null: 1 x 1 grid
    root = qb.construct_2({{1}});
    assert(qb.node(root).isLeaf && qb.node(root).val);

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Design: KV stores, caches, time windows

<a id="p17"></a>

## 17. Design In-Memory File System

**Source:** LC588; E01, C11 · **File:** `file-system.cpp`

**Key idea:** Trie of path components; `std::map` children keep `ls` sorted; one `walk(path, create)` serves all four operations.

**Watch:** `find`, not `[]`, so reads never create nodes. `ls` on a file returns its own name.

```cpp
// Design ls(path), mkdir(path), addContentToFile(path, content), readContentFromFile(path) over absolute paths
// ls on file returns filename, ls on dir return entries sorted, mkdir creates every missing parent

// repeated write append or overwrite?
// ls or read return on missing path?
// path rules: absolute or including ., ..?
// ls's display order, string order or file/dir group?
// concurrent access support?

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

// T: O(L*logC) per op, L: path components, C: max entries in one dir
// S: O(nodes + content bytes), nodes are never freed

struct Node {
    std::map<string, Node*> children; // name -> child, map keeps ls sorted
    string content; // file body, empty for a dir
    bool isFile = false;
};

class FileSystem {
public:
    vector<string> ls(const string& path) {
        Node* node = walk(path, false);
        if (node == nullptr) {
            return {};
        }
        if (node->isFile) {
            return {path.substr(path.rfind('/') + 1)}; // the filename
        }
        vector<string> names;
        for (const auto& entry : node->children) {
            names.push_back(entry.first);
        }
        return names;
    }

    void mkdir(const string& path) {
        walk(path, true);
    }

    void addContentToFile(const string& path,const string& content) {
        Node* node = walk(path, true);
        node->isFile = true;
        node->content += content; // append
    }

    string readContentFromFile(const string& path) {
        Node* node = walk(path, false);
        if (node == nullptr) {
            return "";
        }
        return node->content;
    }

private:
    Node root;

    // follow path from root; create missing nodes only when asked, else nullptr
    Node* walk(const string& path, bool create) {
        Node* cur = &root;
        size_t i = 1; // skip leading '/'
        while (i < path.size()) {
            size_t j = path.find('/', i);
            if (j == string::npos) {
                j = path.size();
            }
            string name = path.substr(i, j - i);

            auto it = cur->children.find(name); // use find, not [], to avoid read auto insertion
            if (it == cur->children.end()) {
                if (!create) {
                    return nullptr;
                }
                // emplace() returns std::pair<iterator, bool>
                it = cur->children.emplace(name, new Node()).first; // ~ insert(std::make_pair(name, new Node())) without temp copy
            }
            cur = it->second;
            i = j + 1;
        }
        return cur;
    }
};

int main() {
    FileSystem fs;
    vector<string> want;

    assert(fs.ls("/").empty()); // fresh root

    fs.mkdir("/a/b/c"); // creates missing parent
    fs.addContentToFile("/a/b/c/d", "hello");
    want = {"a"};
    assert(fs.ls("/") == want);
    want = {"d"};
    assert(fs.ls("/a/b/c") == want);
    assert(fs.ls("/a/b/c/d") == want); // ls on a file returns its own name
    assert(fs.readContentFromFile("/a/b/c/d") == "hello");

    fs.addContentToFile("/a/b/c/d", " world"); // repeated write appends
    assert(fs.readContentFromFile("/a/b/c/d") == "hello world");

    fs.mkdir("/a/b/e");
    fs.mkdir("/a/b/c"); // existing dir: no-op
    fs.addContentToFile("/a/b/aa", "x");
    want = {"aa", "c", "e"};
    assert(fs.ls("/a/b") == want); // sorted, mixed files and dirs
    assert(fs.readContentFromFile("/a/b/c/d") == "hello world");

    fs.addContentToFile("/x/y.txt", "z"); // missing parent is created
    want = {"y.txt"};
    assert(fs.ls("/x") == want);

    assert(fs.ls("/nope").empty()); // missing path, a read cannot create it
    want = {"a", "x"};
    assert(fs.ls("/") == want);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p18"></a>

## 18. KV Store with TTL + list valid entries

**Source:** Q27 (C12); LC706, LC1797 · **File:** `kv-store-ttl.cpp`

**Key idea:** Store + `set<(expireAt, key)>` index; every call purges expired entries from the front (amortized). Overwrite erases the old index pair so an old expiry can't delete the new value.

**Watch:** Erase the store entry before the set node that owns the key string. Ask: now == expireAt valid? time non-decreasing?

```cpp
// Build a KV store with get, set, update and delete. Add a TTL so entry expires ttl after last write then drop
// re-new only works on a live entry

// TTL relative to the write(now + ttl) or absolute expiry?
// update renews TTL?
// now==expiry, is entry still valid
// now non-decreasing?
// get return on a missing or expried key? delete return?

// T: O(logN) amortized per op, list adds O(KlogK), N: live entries, K: entires returned
// S: O(N)

#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <utility>
// #include <optional>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::string;
using std::vector;
using std::pair;

struct Entry {
    string value;
    long long expiredAt; // first time no longer valid
};

class TTLKVStore {
private:
    std::unordered_map<string, Entry> store; // key -> live-value, expiry
    std::set<pair<long long, string>> byExpiry; // (expiredAt, key), earliest expiry first

    // drop every entry with expireAt <= now, each entry leaves once -> amortized
    void purge(long long now) {
        while (!byExpiry.empty() && byExpiry.begin()->first <= now) {
            store.erase(byExpiry.begin()->second); // erasing the Entry first before set
            byExpiry.erase(byExpiry.begin());
        }
    }

public:
    void set(const string& key, const string& value, long long ttl, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it != store.end()) {
            byExpiry.erase({it->second.expiredAt, key});
        }
        store[key] = {value, now + ttl};
        byExpiry.insert({now + ttl, key});
    }

    // std::optional<string> get(const string& key, long long now) {
    string get(const string& key, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it == store.end()) {
            // return std::nullopt;
            return {}; // values are non-empty by contract
        }
        return it->second.value;
    }

    // live-key onlye, renews its ttl, exclude expired keys
    bool update(const string& key, const string& value, long long ttl, long long now) {
        purge(now);
        if (store.count(key) == 0) {
            return false;
        }
        set(key, value, ttl, now);
        return true;
    }

    bool remove(const string& key, long long now) {
        purge(now);
        auto it = store.find(key);
        if (it == store.end()) {
            return false;
        }
        byExpiry.erase({it->second.expiredAt, key});
        store.erase(it);
        return true;
    }

    int countValid(long long now) {
        purge(now);
        return static_cast<int>(store.size());
    }

    vector<pair<string, string>> listValid(long long now) {
        purge(now);
        vector<pair<string, string>> result;
        for (const auto& kv : store) {
            result.push_back({kv.first, kv.second.value});
        }
        std::sort(result.begin(), result.end());
        return result;
    }
};

int main() {
    TTLKVStore kv;

    kv.set("a", "1", 5, 0); // expires at 5
    assert(kv.get("a", 4) == "1");
    // assert(!kv.get("a", 5)); // now == expireAt is expired
    assert(kv.get("a", 5).empty());

    kv.set("b", "2", 5, 10); // expired at 15
    kv.set("b", "3", 5, 13); // overwrite, expires at 18
    assert(kv.get("b", 16) == "3");

    assert(!kv.update("zz", "x", 5, 16)); // missing key
    assert(kv.update("b", "4", 10, 17)); // renew, expired at 27
    kv.set("c", "5", 100, 17);
    assert(kv.countValid(20) == 2);
    vector<pair<string, string>> want = {{"b", "4"}, {"c", "5"}};
    assert(kv.listValid(20) == want);

    assert(kv.remove("c", 21));
    assert(!kv.remove("c", 21)); // already removed
    assert(!kv.update("b", "5", 5, 27)); // expired at 27, can't renew
    assert(kv.countValid(27) == 0);
    assert(kv.listValid(27).empty());

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p19"></a>

## 19. Transaction KV: begin / commit / rollback

**Source:** Q28 (C18) · **File:** `transaction-kv.cpp`

**Key idea:** 1: snapshot copy per begin. 2: undo log per transaction (value before its FIRST write there); rollback restores, commit merges into the parent keeping the parent's original.

**Watch:** Record 'did not exist' so rollback erases new keys. `emplace` in commit so the parent's older original wins. Don't claim O(1) rollback.

```cpp
// KV store: get, set, remove; then begin, commit, rollback with nested transactions (Q28, C18)
// rollback undoes every change since the matching begin; commit keeps the changes and closes
// the innermost transaction (its changes become part of the parent, so a parent rollback still undoes them)

// nested transactions allowed? commit closes the innermost or all?
// commit / rollback with no open transaction: error or no-op?
// "constant time": for which operations? worst case or amortized?
// get on a missing key returns? (here "", values are non-empty)
// concurrency / isolation between clients? (single client here)

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;
using std::pair;

// 1: begin copies the whole store, T: get/set/remove O(1), begin O(N), rollback O(N), commit O(N) to free, S: O(N * depth)
class TransactionKV_1 {
private:
    std::unordered_map<string, string> store; // key -> value
    vector<std::unordered_map<string, string>> snapshots; // store as it was at each open begin

public:
    string get(const string& key) {
        auto it = store.find(key);
        return it == store.end() ? "" : it->second;
    }

    void set(const string& key, const string& value) {
        store[key] = value;
    }

    void remove(const string& key) {
        store.erase(key);
    }

    void begin() {
        snapshots.push_back(store);
    }

    bool rollback() {
        if (snapshots.empty()) {
            return false;
        }
        store = std::move(snapshots.back());
        snapshots.pop_back();
        return true;
    }

    bool commit() {
        if (snapshots.empty()) {
            return false;
        }
        snapshots.pop_back(); // current store already holds the changes
        return true;
    }
};

// 2: undo log per transaction, value before its first write there
// T: get/set/remove O(1), begin O(1), rollback / commit O(K), K: keys touched in that transaction, S: O(N + total K)
class TransactionKV_2 {
private:
    std::unordered_map<string, string> store; // key -> value
    vector<std::unordered_map<string, pair<bool, string>>> undo; // per open transaction: key -> (existed, old value)

public:
    string get(const string& key) {
        auto it = store.find(key);
        return it == store.end() ? "" : it->second;
    }

    void set(const string& key, const string& value) {
        remember(key);
        store[key] = value;
    }

    void remove(const string& key) {
        remember(key);
        store.erase(key);
    }

    void begin() {
        undo.push_back({});
    }

    bool rollback() {
        if (undo.empty()) {
            return false;
        }
        for (const auto& kv : undo.back()) {
            if (kv.second.first) {
                store[kv.first] = kv.second.second; // restore old value
            } else {
                store.erase(kv.first); // key did not exist before
            }
        }
        undo.pop_back();
        return true;
    }

    bool commit() {
        if (undo.empty()) {
            return false;
        }
        if (undo.size() > 1) {
            auto& parent = undo[undo.size() - 2];
            for (const auto& kv : undo.back()) {
                parent.emplace(kv.first, kv.second); // emplace keeps the parent's older original
            }
        }
        undo.pop_back();
        return true;
    }

private:
    void remember(const string& key) {
        if (undo.empty() || undo.back().count(key) != 0) {
            return; // no transaction, or this transaction already saved the original
        }
        auto it = store.find(key);
        undo.back()[key] = (it == store.end()) ? pair<bool, string>(false, "") : pair<bool, string>(true, it->second);
    }
};

int main() {
    TransactionKV_1 a;
    assert(a.get("x") == ""); // null: missing key
    assert(!a.commit()); // fault: no open transaction
    assert(!a.rollback());
    a.set("x", "1");
    a.begin();
    a.set("x", "2");
    a.set("x", "3"); // same key twice in one transaction
    assert(a.rollback());
    assert(a.get("x") == "1"); // target: rollback restores the value before begin
    a.begin();
    a.remove("x");
    a.set("y", "5");
    a.begin();
    a.set("x", "9"); // re-create a key deleted by the outer transaction
    assert(a.commit()); // inner commit
    assert(a.get("x") == "9");
    assert(a.rollback()); // outer rollback undoes the committed inner changes too
    assert(a.get("x") == "1"); // edge: nested + delete then re-create
    assert(a.get("y") == ""); // key created inside the transaction is gone

    TransactionKV_2 b;
    assert(b.get("x") == ""); // null
    assert(!b.commit()); // fault
    assert(!b.rollback());
    b.set("x", "1");
    b.begin();
    b.set("x", "2");
    b.set("x", "3");
    assert(b.rollback());
    assert(b.get("x") == "1"); // target: only the first write in a transaction is remembered
    b.begin();
    b.remove("x");
    b.set("y", "5");
    b.begin();
    b.set("x", "9");
    assert(b.commit());
    assert(b.get("x") == "9");
    assert(b.rollback());
    assert(b.get("x") == "1"); // edge: parent kept its own original of x, not the inner one
    assert(b.get("y") == "");

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p20"></a>

## 20. Time Based KV Store (+ out-of-order)

**Source:** LC981, J9934 · **File:** `time-map.cpp`

**Key idea:** 1: per key append + `upper_bound` then `prev` (in-order timestamps). 2: per key `std::map<ts, value>` for out-of-order sets.

**Watch:** `prev` on `begin()` is UB: check first. Comparator argument order for `std::upper_bound` is (value, element).

```cpp
// set(key, value, timestamp); get(key, timestamp) returns the value set at the largest timestamp <= the given one,
// "" if none (LC981)
// J9934 follow-up: set timestamps can arrive out of order

// set timestamps increasing per key, or any order?
// same key and same timestamp twice: overwrite?
// get before the first set of a key, or an unknown key: ""?
// values non-empty, so "" means none?

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;
using std::pair;

// 1: timestamps increasing per key, append + binary search, T: set O(1), get O(log v), S: O(total sets)
class TimeMap_1 {
private:
    std::unordered_map<string, vector<pair<int, string>>> store; // key -> (timestamp, value), sorted by append order

public:
    void set(const string& key, const string& value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    string get(const string& key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) {
            return "";
        }
        const auto& versions = it->second;
        auto pos = std::upper_bound(versions.begin(), versions.end(), timestamp,
                                    [](int t, const pair<int, string>& p) { return t < p.first; }); // first ts > t
        if (pos == versions.begin()) {
            return ""; // every version is newer than timestamp
        }
        return std::prev(pos)->second;
    }
};

// 2: any order, per-key ordered map, T: set O(log v), get O(log v), S: O(total sets)
class TimeMap_2 {
private:
    std::unordered_map<string, std::map<int, string>> store; // key -> timestamp -> value

public:
    void set(const string& key, const string& value, int timestamp) {
        store[key][timestamp] = value; // same timestamp overwrites
    }

    string get(const string& key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) {
            return "";
        }
        const auto& versions = it->second;
        auto pos = versions.upper_bound(timestamp); // first ts > timestamp
        if (pos == versions.begin()) {
            return "";
        }
        return std::prev(pos)->second;
    }
};

int main() {
    TimeMap_1 a;
    assert(a.get("foo", 1) == ""); // null: unknown key
    a.set("foo", "bar", 1);
    a.set("foo", "bar2", 4);
    assert(a.get("foo", 3) == "bar"); // target: between versions, take the earlier one
    assert(a.get("foo", 4) == "bar2"); // edge: exact timestamp
    assert(a.get("foo", 0) == ""); // fault: before the first version

    TimeMap_2 b;
    assert(b.get("foo", 1) == ""); // null
    b.set("foo", "late", 10);
    b.set("foo", "early", 5); // out of order
    assert(b.get("foo", 7) == "early"); // target: order by timestamp, not by arrival
    assert(b.get("foo", 12) == "late");
    b.set("foo", "fixed", 5);
    assert(b.get("foo", 5) == "fixed"); // edge: same timestamp overwrites
    assert(b.get("foo", 4) == ""); // fault: before the first version

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p21"></a>

## 21. LRU Cache with resize

**Source:** Q33; LC146 · **File:** `lru-cache-resize.cpp`

**Key idea:** `std::list` (front = most recent) + map key -> list iterator; `splice` moves a node in O(1) without invalidating iterators; resize = set cap + same evict loop, O(evicted).

**Watch:** Erase the map entry before `pop_back`. Manual doubly-linked list with sentinels only if STL list is banned.

```cpp
// LRU cache: get(key), put(key, value), resize(newCapacity) (Q33, LC146 + resize)
// a hit or a write makes the key most recent; over capacity evicts the least recent
// shrinking evicts repeatedly until it fits, growing keeps everything

// get miss returns -1? does a miss touch the order?
// put on an existing key: update value and recency, no extra space?
// capacity >= 1? if 0 allowed, what do get/put do?
// resize to the same capacity: no-op?
// thread-safe?

#include <list>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <iostream>

using std::pair;

// T: get/put O(1) average, resize O(e), e: entries evicted
// S: O(capacity)

class LRUCache {
private:
    int cap;
    std::list<pair<int, int>> order; // (key, value), front = most recent, back = least recent
    std::unordered_map<int, std::list<pair<int, int>>::iterator> pos; // key -> its node in order

public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        auto it = pos.find(key); // find, not [], a miss must not insert
        if (it == pos.end()) {
            return -1;
        }
        order.splice(order.begin(), order, it->second); // move node to front, iterator stays valid
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = pos.find(key);
        if (it != pos.end()) {
            it->second->second = value;
            order.splice(order.begin(), order, it->second);
            return;
        }
        order.push_front({key, value});
        pos[key] = order.begin();
        evict();
    }

    void resize(int newCapacity) {
        cap = newCapacity;
        evict();
    }

private:
    void evict() {
        while (static_cast<int>(order.size()) > cap) {
            pos.erase(order.back().first); // erase the map entry before its node dies
            order.pop_back();
        }
    }
};

int main() {
    LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    assert(c.get(1) == 1); // target: hit, 1 becomes most recent
    c.put(3, 3); // evicts 2
    assert(c.get(2) == -1); // fault: miss
    c.put(1, 10); // edge: update refreshes 1, takes no extra slot
    c.put(4, 4); // evicts 3, not 1
    assert(c.get(3) == -1);
    assert(c.get(1) == 10);

    LRUCache r(2);
    r.put(1, 1);
    r.put(2, 2);
    r.get(1);
    r.resize(1); // shrink keeps the most recent
    assert(r.get(2) == -1);
    assert(r.get(1) == 1);
    r.resize(3); // grow keeps content
    r.put(5, 5);
    r.put(6, 6);
    assert(r.get(1) == 1);
    r.resize(1); // edge: shrink by two in one call
    assert(r.get(1) == 1);
    assert(r.get(6) == -1);
    assert(r.get(5) == -1);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p22"></a>

## 22. Design Hit Counter

**Source:** LC362; E01 IC1/IC2 · **File:** `hit-counter.cpp`

**Key idea:** 1: bare queue of timestamps, pop `<= t - 300`. 2: fixed 300 slots at `t % 300` with a timestamp per slot to detect staleness.

**Watch:** Window is (t - 300, t]. Overwrite a stale slot, never add to it.

```cpp
// hit(timestamp) records a hit; getHits(timestamp) returns hits in the past 300 seconds, window (t - 300, t]
// timestamps in seconds, calls arrive in non-decreasing time order (LC362)
// follow-up: huge number of hits per second -> fixed-size array indexed by timestamp % 300

// window (t - 300, t] or [t - 300, t]?
// timestamps non-decreasing across hit and getHits?
// many hits in the same second?
// counts beyond int?
// concurrent callers?

#include <queue>
#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

// 1: queue of every hit, T: hit O(1), getHits O(1) amortized, S: O(hits in window)
class HitCounter_1 {
private:
    std::queue<long long> hits; // timestamp of each hit, oldest at front

public:
    void hit(long long timestamp) {
        hits.push(timestamp);
    }

    long long getHits(long long timestamp) {
        while (!hits.empty() && hits.front() <= timestamp - 300) { // a hit at s counts iff s > t - 300
            hits.pop();
        }
        return static_cast<long long>(hits.size());
    }
};

// 2: fixed 300 slots, slot = timestamp % 300, T: hit O(1), getHits O(300), S: O(300)
class HitCounter_2 {
private:
    vector<long long> times = vector<long long>(300, -1); // slot -> second it holds, -1 = empty
    vector<long long> counts = vector<long long>(300, 0); // slot -> hits in that second

public:
    void hit(long long timestamp) {
        int i = static_cast<int>(timestamp % 300);
        if (times[i] != timestamp) { // slot still holds an older second: overwrite
            times[i] = timestamp;
            counts[i] = 0;
        }
        counts[i]++;
    }

    long long getHits(long long timestamp) {
        long long total = 0;
        for (int i = 0; i < 300; i++) {
            if (times[i] != -1 && timestamp - times[i] < 300) { // inside (t - 300, t]
                total += counts[i];
            }
        }
        return total;
    }
};

int main() {
    HitCounter_1 a;
    assert(a.getHits(1) == 0); // null: no hits yet
    a.hit(1);
    a.hit(2);
    a.hit(3);
    assert(a.getHits(4) == 3); // target: LC example
    a.hit(300);
    assert(a.getHits(300) == 4); // edge: hit at 1 still inside (0, 300]
    assert(a.getHits(301) == 3); // edge: hit at 1 drops out of (1, 301]

    HitCounter_2 b;
    assert(b.getHits(1) == 0); // null
    b.hit(1);
    b.hit(2);
    b.hit(3);
    assert(b.getHits(4) == 3); // target
    b.hit(300);
    assert(b.getHits(300) == 4); // edge: boundary
    assert(b.getHits(301) == 3);
    b.hit(301); // same slot as second 1: overwritten, not added
    assert(b.getHits(301) == 4); // fault: stale slot reuse

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p23"></a>

## 23. Design Log Storage System

**Source:** LC635; E01 IC1/IC2 · **File:** `log-storage.cpp`

**Key idea:** Granularity = prefix length of zero-padded timestamps. 1: scan + compare prefixes. 2: `multimap` + padded bounds ('0...' low, '9...' high) for one range scan.

**Watch:** `multimap`: two logs may share a timestamp. The end bound is inclusive at the granularity.

```cpp
// put(id, timestamp) with timestamp "YYYY:MM:DD:HH:MM:SS"
// retrieve(start, end, granularity): ids whose timestamp is in [start, end], compared only up to granularity (LC635)
// granularity: Year, Month, Day, Hour, Minute, Second

// timestamps always fixed-width and zero-padded? (string compare == time compare)
// range inclusive on both ends?
// output order?
// same id or same timestamp more than once?
// start after end: empty?

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

// characters of "YYYY:MM:DD:HH:MM:SS" that matter at each granularity
int prefixLen(const string& granularity) {
    if (granularity == "Year") {
        return 4;
    }
    if (granularity == "Month") {
        return 7;
    }
    if (granularity == "Day") {
        return 10;
    }
    if (granularity == "Hour") {
        return 13;
    }
    if (granularity == "Minute") {
        return 16;
    }
    return 19;
}

// 1: scan every log, compare truncated strings, T: put O(1), retrieve O(N*L), S: O(N)
class LogSystem_1 {
private:
    vector<std::pair<string, int>> logs; // (timestamp, id)

public:
    void put(int id, const string& timestamp) {
        logs.push_back({timestamp, id});
    }

    vector<int> retrieve(const string& start, const string& end, const string& granularity) {
        int len = prefixLen(granularity);
        string lo = start.substr(0, len);
        string hi = end.substr(0, len);
        vector<int> ids;
        for (const auto& log : logs) {
            string t = log.first.substr(0, len);
            if (lo <= t && t <= hi) { // zero-padded, so string order is time order
                ids.push_back(log.second);
            }
        }
        return ids;
    }
};

// 2: logs ordered by timestamp, pad the bounds and range-scan, T: put O(logN), retrieve O(logN + K), S: O(N)
class LogSystem_2 {
private:
    std::multimap<string, int> logs; // timestamp -> id, sorted by time

public:
    void put(int id, const string& timestamp) {
        logs.insert({timestamp, id});
    }

    vector<int> retrieve(const string& start, const string& end, const string& granularity) {
        int len = prefixLen(granularity);
        string lo = start.substr(0, len) + string("0000:00:00:00:00:00").substr(len); // below any real suffix
        string hi = end.substr(0, len) + string("9999:99:99:99:99:99").substr(len); // above any real suffix
        vector<int> ids;
        for (auto it = logs.lower_bound(lo); it != logs.end() && it->first <= hi; ++it) {
            ids.push_back(it->second);
        }
        return ids;
    }
};

int main() {
    vector<int> want;
    vector<int> got;

    LogSystem_1 a;
    assert(a.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year").empty()); // null: no logs
    a.put(1, "2017:01:01:23:59:59");
    a.put(2, "2017:01:01:22:59:59");
    a.put(3, "2016:01:01:00:00:00");
    want = {1, 2, 3};
    got = a.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year");
    std::sort(got.begin(), got.end());
    assert(got == want); // target: only the year is compared
    want = {1, 2};
    got = a.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Hour");
    std::sort(got.begin(), got.end());
    assert(got == want); // edge: 23:59:59 still inside end hour 23, 3 is before start hour 01
    assert(a.retrieve("2018:01:01:00:00:00", "2017:01:01:00:00:00", "Year").empty()); // fault: start after end

    LogSystem_2 b;
    assert(b.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year").empty()); // null
    b.put(1, "2017:01:01:23:59:59");
    b.put(2, "2017:01:01:22:59:59");
    b.put(3, "2016:01:01:00:00:00");
    want = {1, 2, 3};
    got = b.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Year");
    std::sort(got.begin(), got.end());
    assert(got == want); // target
    want = {1, 2};
    got = b.retrieve("2016:01:01:01:01:01", "2017:01:01:23:00:00", "Hour");
    std::sort(got.begin(), got.end());
    assert(got == want); // edge
    assert(b.retrieve("2018:01:01:00:00:00", "2017:01:01:00:00:00", "Year").empty()); // fault

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p24"></a>

## 24. Event Store: keep latest m, distinct keys before t

**Source:** Q10 (C05), J14594 · **File:** `event-store.cpp`

**Key idea:** `multiset<(ts, key)>` keeps the m latest by time (late records evicted at once); distinct count scans while ts < t into a hash set. topKey follow-up: freq + ranking set.

**Watch:** `multiset` because identical events are separate records; erase by iterator. Strict `< t`.

```cpp
// events (timestamp, key); keep only the m records with the latest timestamps (Q10, J14594)
// countDistinctBefore(t): distinct keys among kept records with timestamp strictly < t
// follow-up topKey(): most frequent key among kept records

// m = number of records, or number of distinct timestamps?
// "before": strictly < t, or <= t?
// records can arrive out of order?
// same timestamp with several keys? which one is evicted on a timestamp tie?
// top key: ties broken how? single key or top K? empty store returns?

#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>
#include <iostream>

using std::string;
using std::pair;

// T: record O(log m), countDistinctBefore O(W), W: kept records before t, topKey O(1)
// S: O(m)

class EventStore {
private:
    int m;
    std::multiset<pair<long long, string>> kept; // (timestamp, key), oldest first, at most m
    std::unordered_map<string, int> freq; // key -> kept records with that key
    std::set<pair<int, string>> byFreq; // (-count, key): most frequent first, ties by key

public:
    EventStore(int capacity) : m(capacity) {}

    void record(long long timestamp, const string& key) {
        kept.insert({timestamp, key});
        bump(key, +1);
        if (static_cast<int>(kept.size()) > m) {
            auto oldest = kept.begin(); // smallest timestamp, may be the record just added
            bump(oldest->second, -1); // use its key before the node is erased
            kept.erase(oldest);
        }
    }

    int countDistinctBefore(long long t) {
        std::unordered_set<string> keys; // distinct keys seen with timestamp < t
        for (auto it = kept.begin(); it != kept.end() && it->first < t; ++it) {
            keys.insert(it->second);
        }
        return static_cast<int>(keys.size());
    }

    string topKey() {
        return byFreq.empty() ? "" : byFreq.begin()->second;
    }

private:
    void bump(const string& key, int delta) {
        auto it = freq.find(key);
        int old = (it == freq.end()) ? 0 : it->second;
        if (old > 0) {
            byFreq.erase({-old, key}); // remove the stale ranking entry first
        }
        int now = old + delta;
        if (now == 0) {
            freq.erase(key);
        } else {
            freq[key] = now;
            byFreq.insert({-now, key});
        }
    }
};

int main() {
    EventStore s(3);
    s.record(1, "a");
    s.record(2, "b");
    s.record(3, "a");
    assert(s.countDistinctBefore(3) == 2); // target: a@1, b@2
    assert(s.countDistinctBefore(2) == 1); // edge: strictly < 2 excludes b@2
    assert(s.topKey() == "a"); // follow-up target

    s.record(4, "b"); // evicts a@1
    assert(s.countDistinctBefore(5) == 2);
    assert(s.topKey() == "b"); // b: 2, a: 1

    s.record(0, "c"); // fault: late record older than all kept, evicted at once
    assert(s.countDistinctBefore(5) == 2);
    assert(s.topKey() == "b");

    EventStore empty(2);
    assert(empty.countDistinctBefore(5) == 0); // null: nothing recorded
    assert(empty.topKey() == "");

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p25"></a>

## 25. Integer Stream with a Dynamic Blacklist

**Source:** Q42 (J10688) · **File:** `blacklist-stream.cpp`

**Key idea:** Hash sets `blacklist` and `seen`; visible = seen && !banned. ban/unban emit an event only if the set changed AND the value was seen.

**Watch:** `insert(...).second` and `erase(...) > 0` detect no-op repeats. Don't call callbacks under a lock.

```cpp
// integer stream with a dynamic blacklist (Q42, J10688)
// process(x): emit x unless it is blacklisted
// follow-up: ban / unban report a state-change event when an already-seen value flips visible <-> hidden

// repeated input: emit every time, or only the first time? (material contradicts itself)
// ban / unban of a value never seen: event or silent?
// repeated ban / unban: no new event?
// concurrent producers and blacklist updates?

#include <unordered_set>
#include <cassert>
#include <iostream>

// T: every operation O(1) average, S: O(B + S), B: blacklisted values, S: values seen

class StreamFilter {
private:
    std::unordered_set<int> blacklist; // values currently banned
    std::unordered_set<int> seen; // values that have appeared in the stream
    // visible(x) = seen(x) && !blacklist(x), no third set needed

public:
    // emits every non-banned occurrence
    bool process(int x) {
        seen.insert(x);
        return blacklist.count(x) == 0;
    }

    // true = event "x is now hidden"; only for a seen value that was visible
    bool ban(int x) {
        bool added = blacklist.insert(x).second; // false if already banned: no change
        return added && seen.count(x) != 0;
    }

    // true = event "x is visible again"; only for a seen value that was hidden
    bool unban(int x) {
        bool removed = blacklist.erase(x) > 0;
        return removed && seen.count(x) != 0;
    }
};

int main() {
    StreamFilter f;

    assert(f.process(1)); // null: empty blacklist emits
    assert(f.ban(1)); // target: seen value flips to hidden -> event
    assert(!f.process(1)); // banned value not emitted
    assert(!f.ban(1)); // edge: repeated ban, no new event

    assert(!f.ban(2)); // edge: ban before 2 is ever seen, silent
    assert(!f.process(2));
    assert(f.unban(2)); // 2 was seen while banned, now visible -> event
    assert(f.process(2)); // re-input after unban is emitted

    assert(!f.unban(3)); // fault: unban a value that was never banned or seen

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p26"></a>

## 26. Min Stack

**Source:** LC155 (company tag) · **File:** `min-stack.cpp`

**Key idea:** 1: each entry stores (value, min so far). 2: a separate min stack pushed on `<=`.

**Watch:** `<=` for duplicate minimums in version 2.

```cpp
// stack with push, pop, top, getMin, every operation O(1) (LC155)

// pop/top/getMin only called on a non-empty stack?
// duplicate minimum values?
// int range?

#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

// 1: every entry remembers the min at the time it was pushed, T: O(1) each, S: O(n) pairs
class MinStack_1 {
private:
    vector<std::pair<int, int>> st; // (value, min of this entry and everything below)

public:
    void push(int x) {
        int m = st.empty() ? x : std::min(x, st.back().second);
        st.push_back({x, m});
    }

    void pop() {
        st.pop_back();
    }

    int top() {
        return st.back().first;
    }

    int getMin() {
        return st.back().second;
    }
};

// 2: second stack only grows when a new min (or a tie) arrives, T: O(1) each, S: O(n) worst
class MinStack_2 {
private:
    vector<int> st; // all values
    vector<int> mins; // non-increasing, top = current min

public:
    void push(int x) {
        st.push_back(x);
        if (mins.empty() || x <= mins.back()) { // <= : a duplicate min must be pushed too
            mins.push_back(x);
        }
    }

    void pop() {
        if (st.back() == mins.back()) {
            mins.pop_back();
        }
        st.pop_back();
    }

    int top() {
        return st.back();
    }

    int getMin() {
        return mins.back();
    }
};

int main() {
    MinStack_1 a;
    a.push(-2);
    a.push(0);
    a.push(-3);
    assert(a.getMin() == -3); // target: LC example
    a.pop();
    assert(a.top() == 0);
    assert(a.getMin() == -2); // min restored after pop
    MinStack_1 a2;
    a2.push(1);
    a2.push(1);
    a2.pop();
    assert(a2.getMin() == 1); // edge: duplicate min survives one pop

    MinStack_2 b;
    b.push(-2);
    b.push(0);
    b.push(-3);
    assert(b.getMin() == -3); // target
    b.pop();
    assert(b.top() == 0);
    assert(b.getMin() == -2);
    MinStack_2 b2;
    b2.push(1);
    b2.push(1);
    b2.pop();
    assert(b2.getMin() == 1); // edge: fails if push used < instead of <=

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Heaps and ordered sets

<a id="p27"></a>

## 27. Design Task Manager

**Source:** LC3408 · **File:** `task-manager.cpp`

**Key idea:** Max-heap of (priority, taskId) + map taskId -> (user, current priority); edits/removals leave stale entries skipped on pop (lazy deletion).

**Watch:** Stale check needs both 'still active' and 'priority matches'. Heap grows with edits; ordered set or rebuild if it matters.

```cpp
// Design add(userId, taskId, priority), edit(taskId, newPriority), rmv(taskId), execTop()
// execTop runs the highest priority task (tie: larger taskId), removes it, returns its userId or -1

// active taskId unique? edit/rmv on a missing id?
// priority direction, larger runs first?
// tie-break on equal priority?
// execTop return when no task left?

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <cassert>
#include <iostream>

using std::vector;
using std::pair;

// T: add/edit O(logM), rmv O(1), execTop amortized O(logM), M: maxHeap entries incl. stale
// S: O(T + M), T: active tasks

class TaskManager {
private:
    std::unordered_map<int, pair<int, int>> tasks; // taskId -> (userId, current priority), active tasks
    std::priority_queue<pair<int, int>> maxHeap; // (priority, taskId), max-maxHeap, stale old task, all tasks

public:
    TaskManager(const vector<vector<int>>& tasks) {
        for (const auto& t : tasks) {
            add(t[0], t[1], t[2]);
        }
    }

    void add(int userId, int taskId, int priority) {
        tasks[taskId] = {userId, priority};
        maxHeap.push({priority, taskId});
    }

    void edit(int taskId, int newPriority) {
        auto it = tasks.find(taskId);
        if (it == tasks.end()) {
            return;
        }
        tasks[taskId].second = newPriority;
        maxHeap.push({newPriority, taskId}); // old entry goes stale, skipped on pop
    }

    void rmv(int taskId) {
        tasks.erase(taskId); // its maxHeap entries go stale
    }

    int execTop() {
        while (!maxHeap.empty()) {
            auto [priority, taskId] = maxHeap.top();
            maxHeap.pop();
            auto it = tasks.find(taskId);
            if (it != tasks.end() && it->second.second == priority) { // still active and current priority
                int userId = it->second.first;
                tasks.erase(it);
                return userId;
            }
        }
        return -1;
    }
};

int main() {
    TaskManager tm({{1, 101, 10}, {2, 102, 20}, {3, 103, 15}});
    tm.add(4, 104, 5);
    tm.edit(102, 8);
    assert(tm.execTop() == 3); // 103 at 15, stale (20, 102) skipped first
    tm.rmv(101);
    tm.add(5, 105, 15);
    assert(tm.execTop() == 5);

    assert(tm.execTop() == 2); // 102 at 8
    assert(tm.execTop() == 4); // 104 at 5
    assert(tm.execTop() == -1); // only stale entries left

    TaskManager tie({{1, 7, 3}, {2, 9, 3}});
    assert(tie.execTop() == 2); // same priority, larger taskId first

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p28"></a>

## 28. Priority Executor, duplicate IDs run once

**Source:** Q09 (C01), J13542 · **File:** `task-executor-dedup.cpp`

**Key idea:** Heap of (priority, -ts, -seq, id) for a total order; permanent `executed` set skips duplicate records.

**Watch:** Brace-init of std::tuple needs C++17; `emplace` or `make_tuple` for C++11.

```cpp
// Design addTask(taskId, priority, timestamp), executeTask()
// same taskId can be added many times, executeTask runs the highest priority record and returns its id
// an id runs at most once ever, later records of it are skipped, -1 if nothing runnable

// priority direction, larger runs first?
// tie-break on equal priority, earlier timestamp then earlier add?
// timestamp a sort key or just recorded?
// duplicate add a new record or overwrite the old priority?
// execution can fail? failure counts as done?
// re-add an done id, ignore or error?

#include <vector>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <cassert>
#include <iostream>

// T: addTask O(logR), executeTask amortized O(logR), R: records added
// S: O(R + X), X: done ids

class TaskExecutor {
private:
    std::priority_queue<std::tuple<int, long long, int, int>> maxHeap; // (priority, -ts, -seq, taskId)
    std::unordered_set<int> done; // ids that ran, permanent
    int seq = 0; // add order, last tie-break so the order is total

public:
    void addTask(int taskId, int priority, long long timestamp) {
        if (done.count(taskId) != 0) {
            return; // already ran, never runs again
        }
        maxHeap.push({priority, -timestamp, -seq, taskId}); // negate so max-maxHeap prefers earlier ts, earlier add
        seq++;
    }

    int executeTask() {
        while (!maxHeap.empty()) {
            int taskId = std::get<3>(maxHeap.top());
            maxHeap.pop();
            if (done.count(taskId) != 0) {
                continue; // duplicate record of an id that already ran
            }
            done.insert(taskId);
            return taskId;
        }
        return -1;
    }
};

int main() {
    TaskExecutor ex;
    assert(ex.executeTask() == -1); // empty

    ex.addTask(1, 5, 10);
    ex.addTask(2, 9, 11);
    ex.addTask(1, 9, 12); // duplicate id, higher priority record
    ex.addTask(3, 9, 11); // ties 2 on priority and ts, added later

    assert(ex.executeTask() == 2); // priority 9, ts 11, added first
    assert(ex.executeTask() == 3); // same priority and ts, added later
    assert(ex.executeTask() == 1); // priority 9 record of the duplicate id
    assert(ex.executeTask() == -1); // only (1, 5) left, id 1 already ran

    ex.addTask(1, 100, 20); // done id never runs again
    assert(ex.executeTask() == -1);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p29"></a>

## 29. Meeting Rooms II + buses with stations

**Source:** LC253, Q18 (C06) · **File:** `meeting-rooms.cpp`

**Key idea:** Rooms: heap of end times, two sorted arrays, or event sweep (peak of running count; -1 sorts before +1 at equal times). Buses with stations: per-station heap, or per-station sweep summing peak deficits.

**Watch:** `end == start` reuse lives in `<=` / the event sort order. Station versions need positive trip durations.

```cpp
// min rooms so overlapping meetings never share one, a room frees at end and can be reused at start == end
// follow-up: buses with depart/arrive stations, a bus is reusable only where it parked

// end == start reuses the room?
// closed or half-open intervals? zero-length?
// input sorted? empty?
// empty repositioning between stations allowed? turnaround time? every trip duration > 0?
// return count only, or the assignment?

#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <cassert>
#include <iostream>

using std::vector;

class IntervalScheduler {
public:
    // sort by start + min-heap of end times, T: O(NlogN), S: O(N), sorts its own copy
    int minMeetingRooms_1(vector<vector<int>> intervals) {
        std::sort(intervals.begin(), intervals.end());
        std::priority_queue<int, vector<int>, std::greater<int>> ends; // minHeap of end times
        for (const auto& intv : intervals) {
            int curStart = intv[0];
            int curEnd = intv[1];
            if (!ends.empty() && ends.top() <= curStart) { // <= : room freed at start time is reusable
                ends.pop();
            }
            ends.push(curEnd);
        }
        return static_cast<int>(ends.size()); // never shrinks, so final size is the peak
    }

    // two sorted arrays, T: O(NlogN), S: O(N)
    int minMeetingRooms_2(const vector<vector<int>>& intervals) {
        vector<int> starts;
        vector<int> ends;
        for (const auto& intv : intervals) {
            starts.push_back(intv[0]);
            ends.push_back(intv[1]);
        }
        std::sort(starts.begin(), starts.end());
        std::sort(ends.begin(), ends.end());

        int rooms = 0;
        int i = 0; // earliest end not yet reused
        for (int start : starts) {
            if (start >= ends[i]) {
                i++; // reuse the room that ended
            } else {
                rooms++;
            }
        }
        return rooms;
    }

    // sweep-line over events, T: O(NlogN), S: O(N)
    int minMeetingRooms_3(const vector<vector<int>>& intervals) {
        vector<std::pair<int, int>> events; // event: {start, +1} or {end, -1}
        for (const auto& intv : intervals) {
            events.push_back({intv[0], +1});
            events.push_back({intv[1], -1});
        }
        std::sort(events.begin(), events.end()); // same time: -1 before +1, free room before reuse

        int curRooms = 0;
        int maxRooms = 0;
        for (const auto& e : events) {
            curRooms += e.second;
            maxRooms = std::max(maxRooms, curRooms);
        }
        return maxRooms;
    }

    // follow-up: trip = {depStation, arrStation, depTime, arrTime}, needs arrTime > depTime
    // per-station min-heap of free times, T: O(NlogN), S: O(N), sorts its own copy
    int minBuses_1(vector<vector<int>> trips) {
        std::sort(trips.begin(), trips.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[2] < b[2]; // by departure time
        });
        std::unordered_map<int, std::priority_queue<int, vector<int>, std::greater<int>>> parked; // station -> times buses become free there
        int buses = 0;
        for (const auto& t : trips) {
            auto& here = parked[t[0]]; // [] inserts an empty heap, harmless here
            if (!here.empty() && here.top() <= t[2]) {
                here.pop(); // reuse a bus already at this station
            } else {
                buses++;
            }
            parked[t[1]].push(t[3]); // bus parks at the arrival station
        }
        return buses;
    }

    // sweep per station: buses = sum over stations of peak (departures - arrivals), T: O(NlogN), S: O(N)
    int minBuses_2(vector<vector<int>> trips) {
        std::unordered_map<int, vector<std::pair<int, int>>> events; // station -> event: {depart, +1} or {arrive, -1}
        for (const auto& t : trips) {
            events[t[0]].push_back({t[2], +1});
            events[t[1]].push_back({t[3], -1});
        }

        int buses = 0;
        for (auto& e : events) {
            auto& pairs = e.second;
            std::sort(pairs.begin(), pairs.end());
            int need = 0;
            int peak = 0; // buses must be parted here at start
            for (const auto& p : pairs) {
                need += p.second;
                peak = std::max(peak, need);
            }
            buses += peak;
        }
        return buses;
    }
};

int main() {
    IntervalScheduler is;

    assert(is.minMeetingRooms_1({{0, 30}, {5, 10}, {15, 20}}) == 2); // LC example
    assert(is.minMeetingRooms_2({{0, 30}, {5, 10}, {15, 20}}) == 2);
    assert(is.minMeetingRooms_3({{0, 30}, {5, 10}, {15, 20}}) == 2);
    assert(is.minMeetingRooms_1({{1, 5}, {5, 10}}) == 1); // end == start reuses
    assert(is.minMeetingRooms_2({{1, 5}, {5, 10}}) == 1);
    assert(is.minMeetingRooms_3({{1, 5}, {5, 10}}) == 1);
    assert(is.minMeetingRooms_1({{1, 10}, {2, 9}, {3, 8}}) == 3); // nested, all overlap
    assert(is.minMeetingRooms_2({{1, 10}, {2, 9}, {3, 8}}) == 3);
    assert(is.minMeetingRooms_3({{1, 10}, {2, 9}, {3, 8}}) == 3);
    assert(is.minMeetingRooms_1({}) == 0); // empty
    assert(is.minMeetingRooms_2({}) == 0);
    assert(is.minMeetingRooms_3({}) == 0);

    assert(is.minBuses_1({{0, 1, 0, 10}, {1, 0, 10, 20}}) == 1); // returns from station 1, arrive == depart reuses
    assert(is.minBuses_2({{0, 1, 0, 10}, {1, 0, 10, 20}}) == 1);
    assert(is.minBuses_1({{0, 1, 0, 10}, {0, 1, 10, 20}}) == 2); // bus parked at 1, can't serve station 0
    assert(is.minBuses_2({{0, 1, 0, 10}, {0, 1, 10, 20}}) == 2);
    assert(is.minBuses_1({{0, 1, 0, 10}, {1, 2, 5, 15}}) == 2); // not at station 1 yet at time 5
    assert(is.minBuses_2({{0, 1, 0, 10}, {1, 2, 5, 15}}) == 2);
    assert(is.minBuses_1({}) == 0); // empty
    assert(is.minBuses_2({}) == 0);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p30"></a>

## 30. Find Median from Data Stream

**Source:** LC295 · **File:** `median-stream.cpp`

**Key idea:** Max-heap low half + min-heap high half; low holds the extra; push through low, move its top up, rebalance.

**Watch:** Widen to long long before summing two tops; divide by 2.0.

```cpp
// numbers arrive one by one: addNum(num), findMedian() returns the median so far (LC295)
// even count: average of the two middle values

// findMedian never called on an empty stream?
// int range, two INT_MAX values averaged?
// return double?
// deletions or a sliding window later?

#include <vector>
#include <queue>
#include <functional>
#include <climits>
#include <cassert>
#include <iostream>

using std::vector;

// T: addNum O(logn), findMedian O(1), S: O(n)

class MedianFinder {
private:
    std::priority_queue<int> low; // max-heap, smaller half, holds the extra one on odd counts
    std::priority_queue<int, vector<int>, std::greater<int>> high; // min-heap, larger half

public:
    // invariants: every low <= every high, low.size() == high.size() or high.size() + 1
    void addNum(int num) {
        low.push(num);
        high.push(low.top()); // largest of the low half moves up, keeps the order invariant
        low.pop();
        if (high.size() > low.size()) {
            low.push(high.top()); // rebalance so low keeps the extra
            high.pop();
        }
    }

    double findMedian() const {
        if (low.size() > high.size()) {
            return low.top();
        }
        return (static_cast<long long>(low.top()) + high.top()) / 2.0; // widen first, INT_MAX + INT_MAX overflows
    }
};

int main() {
    MedianFinder mf;
    mf.addNum(1);
    mf.addNum(2);
    assert(mf.findMedian() == 1.5); // target: even count averages the middle two
    mf.addNum(3);
    assert(mf.findMedian() == 2); // target: odd count

    MedianFinder dup;
    dup.addNum(-1);
    dup.addNum(-1);
    assert(dup.findMedian() == -1); // edge: duplicates, negatives

    MedianFinder big;
    big.addNum(INT_MAX);
    big.addNum(INT_MAX);
    assert(big.findMedian() == INT_MAX); // fault: sum overflows int

    MedianFinder one;
    one.addNum(5);
    assert(one.findMedian() == 5); // null: single number

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p31"></a>

## 31. Top K Frequent Elements

**Source:** LC347 · **File:** `top-k-frequent.cpp`

**Key idea:** 1: count + min-heap capped at k (O(n log k)). 2: buckets by count (O(n)).

**Watch:** Min-heap of size k, not a max-heap of everything. `buckets(n + 1)`.

```cpp
// return the k most frequent values in nums, any order (LC347)

// ties at the k-th frequency: any of them, or guaranteed unique?
// output order?
// k <= number of distinct values?
// stream with a time window later? (needs eviction, see follow-ups)

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <functional>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::pair;

class FrequencyRanker {
public:
    // count + min-heap of size k, top is the weakest of the current best k
    // T: O(n logk), S: O(n)
    vector<int> topKFrequent_1(const vector<int>& nums, int k) {
        std::unordered_map<int, int> freq; // value -> count
        for (int x : nums) {
            freq[x]++;
        }
        std::priority_queue<pair<int, int>, vector<pair<int, int>>, std::greater<pair<int, int>>> minHeap; // (count, value)
        for (const auto& kv : freq) {
            minHeap.push({kv.second, kv.first});
            if (static_cast<int>(minHeap.size()) > k) {
                minHeap.pop(); // drop the least frequent, k best remain
            }
        }
        vector<int> result;
        while (!minHeap.empty()) {
            result.push_back(minHeap.top().second);
            minHeap.pop();
        }
        return result;
    }

    // bucket by count, a count is at most n, T: O(n), S: O(n)
    vector<int> topKFrequent_2(const vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        std::unordered_map<int, int> freq; // value -> count
        for (int x : nums) {
            freq[x]++;
        }
        vector<vector<int>> buckets(n + 1); // count -> values with that count
        for (const auto& kv : freq) {
            buckets[kv.second].push_back(kv.first);
        }
        vector<int> result;
        for (int c = n; c >= 1; c--) {
            for (int v : buckets[c]) {
                result.push_back(v);
                if (static_cast<int>(result.size()) == k) {
                    return result;
                }
            }
        }
        return result;
    }
};

int main() {
    FrequencyRanker fr;
    vector<int> want;
    vector<int> got;

    want = {1, 2};
    got = fr.topKFrequent_1({1, 1, 1, 2, 2, 3}, 2);
    std::sort(got.begin(), got.end()); // any order is valid
    assert(got == want); // target
    got = fr.topKFrequent_2({1, 1, 1, 2, 2, 3}, 2);
    std::sort(got.begin(), got.end());
    assert(got == want);

    want = {-1};
    assert(fr.topKFrequent_1({-1, -1, 4}, 1) == want); // edge: negative values, k = 1
    assert(fr.topKFrequent_2({-1, -1, 4}, 1) == want);

    assert(fr.topKFrequent_1({}, 0).empty()); // null: empty input
    assert(fr.topKFrequent_2({}, 0).empty());

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p32"></a>

## 32. Top K Book Sales (running totals)

**Source:** Q35 · **File:** `top-k-book-sales.cpp`

**Key idea:** Totals map + `set<(-total, name)>`: erase old entry, add, insert new; query reads the first k.

**Watch:** Never let a comparator read a mutable map; the set key must hold everything it sorts by.

```cpp
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
```

<a id="p33"></a>

## 33. Most Frequent IDs

**Source:** LC3092 (company tag) · **File:** `most-frequent-ids.cpp`

**Key idea:** 1: rescan counts per step. 2: multiset of positive counts, max = `*rbegin()`.

**Watch:** `erase(find(c))`, not `erase(c)`: two ids can share a count.

```cpp
// step i adds freq[i] copies of id nums[i] (negative freq removes copies)
// after every step, report the count of the most frequent id, 0 if the collection is empty (LC3092)

// freq can be negative? a count never drops below 0?
// return the count, not the id?
// empty collection -> 0?
// counts beyond int?

#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class FrequencyTracker {
public:
    // 1: update the count, rescan every id for the max, T: O(n*U), U: distinct ids, S: O(U)
    vector<long long> mostFrequentIDs_1(const vector<int>& nums, const vector<int>& freq) {
        std::unordered_map<int, long long> count; // id -> current copies
        vector<long long> ans;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            count[nums[i]] += freq[i];
            long long best = 0;
            for (const auto& kv : count) {
                best = std::max(best, kv.second);
            }
            ans.push_back(best);
        }
        return ans;
    }

    // 2: also keep every positive count in a multiset, max = last element, T: O(n logn), S: O(U)
    vector<long long> mostFrequentIDs_2(const vector<int>& nums, const vector<int>& freq) {
        std::unordered_map<int, long long> count; // id -> current copies
        std::multiset<long long> counts; // current count of each id with count > 0
        vector<long long> ans;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            long long& c = count[nums[i]];
            if (c > 0) {
                counts.erase(counts.find(c)); // one copy only; erase(c) would drop every id with that count
            }
            c += freq[i];
            if (c > 0) {
                counts.insert(c);
            }
            ans.push_back(counts.empty() ? 0 : *counts.rbegin());
        }
        return ans;
    }
};

int main() {
    FrequencyTracker ft;
    vector<long long> want;

    want = {3, 3, 2, 2};
    assert(ft.mostFrequentIDs_1({2, 3, 2, 1}, {3, 2, -3, 1}) == want); // target: LC example
    assert(ft.mostFrequentIDs_2({2, 3, 2, 1}, {3, 2, -3, 1}) == want);

    want = {2, 0};
    assert(ft.mostFrequentIDs_1({5, 5}, {2, -2}) == want); // edge: collection becomes empty
    assert(ft.mostFrequentIDs_2({5, 5}, {2, -2}) == want);

    want = {1, 1, 1};
    assert(ft.mostFrequentIDs_1({1, 2, 1}, {1, 1, -1}) == want); // fault: two ids share a count, remove one
    assert(ft.mostFrequentIDs_2({1, 2, 1}, {1, 1, -1}) == want);

    assert(ft.mostFrequentIDs_1({}, {}).empty()); // null
    assert(ft.mostFrequentIDs_2({}, {}).empty());

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Binary search

<a id="p34"></a>

## 34. Binary Search with Repeated Queries

**Source:** J10556 NG; LC704 / 34 / 35 · **File:** `binary-search-queries.cpp`

**Key idea:** One half-open `lowerBound` template answers search, range (`lowerBound(t+1) - 1` or upperBound - 1) and insert position; many static queries -> value -> first index map.

**Watch:** `1LL * target + 1` at INT_MAX. Inserting into a sorted array shifts every later index: the hash can't be patched.

```cpp
// static sorted array, answer many lookups (J10556 NG; LC704 / 34 / 35)
// search: index of target or -1; searchRange: first and last index; searchInsert: where target would go
// follow-up: many repeated queries -> prebuilt value -> first index map, O(1) average per query

// duplicates allowed? which index to return, any or leftmost?
// absent target: -1 or insert position?
// array static, or inserts/deletes between queries?
// how many queries vs n? repeated or clustered values?
// int range, target at INT_MAX?

#include <vector>
#include <unordered_map>
#include <climits>
#include <cassert>
#include <iostream>

using std::vector;

class SortedSearcher {
private:
    vector<int> a; // sorted values
    int n;
    std::unordered_map<int, int> firstIndex; // value -> leftmost index

public:
    // T: O(n) build, S: O(n) copy + map
    SortedSearcher(const vector<int>& sorted) : a(sorted), n(static_cast<int>(sorted.size())) {
        for (int i = 0; i < n; i++) {
            firstIndex.emplace(a[i], i); // emplace keeps the first index of a duplicate run
        }
    }

    // LC704, leftmost index on duplicates, T: O(logn)
    int search(int target) const {
        int i = lowerBound(target);
        return (i < n && a[i] == target) ? i : -1;
    }

    // LC34, T: O(logn)
    vector<int> searchRange(int target) const {
        int first = lowerBound(target);
        if (first == n || a[first] != target) {
            return {-1, -1};
        }
        int last = lowerBound(1LL * target + 1) - 1; // widen first, target + 1 overflows at INT_MAX
        return {first, last};
    }

    // LC35, T: O(logn)
    int searchInsert(int target) const {
        return lowerBound(target);
    }

    // follow-up: repeated queries on a static array, T: O(1) average
    int searchCached(int target) const {
        auto it = firstIndex.find(target); // find, not [], so a miss doesn't insert
        return it == firstIndex.end() ? -1 : it->second;
    }

private:
    // first index with a[i] >= target, n if none; answer always stays in [lo, hi]
    int lowerBound(long long target) const {
        int lo = 0;
        int hi = n;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] < target) {
                lo = mid + 1; // mid and everything left of it are too small
            } else {
                hi = mid; // mid could be the answer, keep it
            }
        }
        return lo;
    }
};

int main() {
    SortedSearcher s({-1, 0, 3, 5, 5, 5, 9, 12});
    vector<int> want;

    assert(s.search(9) == 6);
    assert(s.search(2) == -1); // absent, between two values
    assert(s.search(5) == 3); // duplicates: leftmost
    want = {3, 5};
    assert(s.searchRange(5) == want);
    want = {-1, -1};
    assert(s.searchRange(4) == want);
    assert(s.searchInsert(4) == 3);
    assert(s.searchInsert(-5) == 0); // before everything
    assert(s.searchInsert(13) == 8); // past the end
    assert(s.searchCached(5) == 3);
    assert(s.searchCached(2) == -1);

    SortedSearcher empty({});
    assert(empty.search(1) == -1); // empty array
    assert(empty.searchInsert(1) == 0);

    SortedSearcher top({INT_MAX, INT_MAX});
    want = {0, 1};
    assert(top.searchRange(INT_MAX) == want); // target + 1 would overflow as int

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p35"></a>

## 35. Find Minimum in Rotated Sorted Array

**Source:** LC153 (J13422), LC154 · **File:** `rotated-array-min.cpp`

**Key idea:** Compare `a[mid]` with `a[hi]`: greater -> lo = mid + 1, else hi = mid. Duplicates: equal -> hi-- (worst O(n)).

**Watch:** Compare with `a[hi]`, not `a[lo]`; `hi = mid`, not `mid - 1`.

```cpp
// sorted ascending array rotated at an unknown pivot, find the minimum (LC153, J13422)
// follow-up LC154: duplicates allowed

// distinct values? (153 yes, 154 no)
// array non-empty?
// maybe not rotated at all?
// return the value or its index?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class RotatedSearch {
public:
    // 1: baseline scan, T: O(n), S: O(1)
    int findMin_1(const vector<int>& a) {
        return *std::min_element(a.begin(), a.end());
    }

    // 2: [lo, hi] always holds the min; a[mid] > a[hi] means the drop is right of mid
    // T: O(logn), S: O(1)
    int findMin_2(const vector<int>& a) {
        int lo = 0;
        int hi = static_cast<int>(a.size()) - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] > a[hi]) {
                lo = mid + 1; // mid is in the left, larger part
            } else {
                hi = mid; // mid is in the right part, could be the min itself
            }
        }
        return a[lo];
    }

    // 3 (LC154): duplicates, a[mid] == a[hi] says nothing about the side, drop hi only
    // T: O(logn) typical, O(n) worst (all equal), S: O(1)
    int findMinDup(const vector<int>& a) {
        int lo = 0;
        int hi = static_cast<int>(a.size()) - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] > a[hi]) {
                lo = mid + 1;
            } else if (a[mid] < a[hi]) {
                hi = mid;
            } else {
                hi--; // a[hi] has a copy at mid, safe to drop
            }
        }
        return a[lo];
    }
};

int main() {
    RotatedSearch rs;

    assert(rs.findMin_1({4, 5, 6, 7, 0, 1, 2}) == 0); // target
    assert(rs.findMin_2({4, 5, 6, 7, 0, 1, 2}) == 0);
    assert(rs.findMin_2({1, 2, 3}) == 1); // edge: not rotated
    assert(rs.findMin_2({2, 1}) == 1); // edge: two elements, min last
    assert(rs.findMin_2({5}) == 5); // null: single element

    assert(rs.findMinDup({10, 1, 10, 10, 10}) == 1); // follow-up: a[mid] == a[hi] but the min is left of mid
    assert(rs.findMinDup({2, 2, 2}) == 2); // all equal, worst case

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p36"></a>

## 36. Maximum Throughput (binary search on the answer)

**Source:** Q34 · **File:** `max-throughput.cpp`

**Key idea:** For target T each service needs `(T-1)/t[i]` upgrades; feasibility is monotone in T, so binary search the last feasible T.

**Watch:** Round mid up for last-true search. Compare by division so cost never overflows. Don't claim the 'upgrade the slowest' greedy.

```cpp
// n services in series, service i has base throughput t[i], one upgrade costs c[i]
// after x upgrades its throughput is t[i] * (x + 1); total budget B
// maximize the system throughput = min over services (Q34)

// t, c positive integers? B >= 0?
// upgrades unlimited, any count per service?
// throughput t * (x + 1), linear in upgrades?
// return the max throughput only, or the upgrade plan?
// value bounds: does t * (1 + B / c) fit in 64 bits?

#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <iostream>

using std::vector;

class ThroughputPlanner {
public:
    // binary search the last feasible target T; cost to reach T is per service, so feasible(T) is monotone
    // T: O(n log U), U: hi - lo, S: O(1)
    long long maxThroughput(const vector<long long>& t, const vector<long long>& c, long long budget) {
        if (t.empty()) {
            return 0;
        }
        long long lo = *std::min_element(t.begin(), t.end()); // reachable with zero upgrades
        long long hi = LLONG_MAX; // tightest per-service bound: all budget on one service
        for (int i = 0; i < static_cast<int>(t.size()); i++) {
            long long maxUpgrades = budget / c[i];
            if (maxUpgrades + 1 <= LLONG_MAX / t[i]) { // check by division before multiplying
                hi = std::min(hi, t[i] * (maxUpgrades + 1));
            }
        }

        while (lo < hi) {
            long long mid = lo + (hi - lo + 1) / 2; // round up, else lo = mid loops forever
            if (feasible(t, c, budget, mid)) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        return lo;
    }

private:
    // can every service reach `target` within budget?
    bool feasible(const vector<long long>& t, const vector<long long>& c, long long budget, long long target) {
        long long cost = 0;
        for (int i = 0; i < static_cast<int>(t.size()); i++) {
            long long need = (target - 1) / t[i]; // = ceil(target / t) - 1 for target >= 1
            if (need > (budget - cost) / c[i]) {
                return false; // over budget; checked by division so cost never overflows
            }
            cost += need * c[i];
        }
        return true;
    }
};

int main() {
    ThroughputPlanner tp;

    assert(tp.maxThroughput({2, 3}, {1, 2}, 3) == 4); // target: upgrade both once, cost 3; 5 needs cost 4
    assert(tp.maxThroughput({2, 3}, {1, 2}, 0) == 2); // edge: no budget, slowest service decides
    assert(tp.maxThroughput({1000000000, 1000000000}, {1, 1}, 2000000000) == 1000000001000000000LL); // fault: needs 64-bit
    assert(tp.maxThroughput({}, {}, 5) == 0); // null: no services

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Strings and parsing

<a id="p37"></a>

## 37. Encode and Decode Strings

**Source:** LC271 (C04, J18280) · **File:** `encode-decode-strings.cpp`

**Key idea:** 1: length prefix `len#`, decode jumps over the content. 2: escape `\` and `,`, TERMINATE each string with `,` so {} and {""} differ.

**Watch:** Jump `i = hash + 1 + len`, never scan inside content. Escape the escape character.

```cpp
// encode a list of strings into one string and decode it back exactly (LC271, J18280)
// strings may be empty and may contain any character, including the ones the encoding uses

// any byte allowed in a string? '\0', non-ASCII?
// empty strings? empty list?
// decode only our own output, or must it reject malformed input?
// size limits? streaming decode?

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class Codec {
public:
    // length prefix "len#" before each string, T: O(L), S: O(L), L: total characters
    string encode_1(const vector<string>& strs) {
        string out;
        for (const string& s : strs) {
            out += std::to_string(s.size()) + "#" + s;
        }
        return out;
    }

    vector<string> decode_1(const string& data) {
        vector<string> strs;
        size_t i = 0;
        while (i < data.size()) {
            size_t hash = data.find('#', i); // first '#' after i ends the length; content may hold '#'
            size_t len = std::stoul(data.substr(i, hash - i));
            strs.push_back(data.substr(hash + 1, len));
            i = hash + 1 + len; // jump over the content, never scan inside it
        }
        return strs;
    }

    // escaping: '\' -> "\\", ',' -> "\,", every string ends with an unescaped ','
    // T: O(L), S: O(L)
    string encode_2(const vector<string>& strs) {
        string out;
        for (const string& s : strs) {
            for (char c : s) {
                if (c == '\\' || c == ',') {
                    out += '\\';
                }
                out += c;
            }
            out += ','; // terminator, not separator, so {} and {""} encode differently
        }
        return out;
    }

    vector<string> decode_2(const string& data) {
        vector<string> strs;
        string cur; // string being rebuilt
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i] == '\\') {
                cur += data[i + 1]; // escaped char taken literally
                i++;
            } else if (data[i] == ',') {
                strs.push_back(cur);
                cur.clear();
            } else {
                cur += data[i];
            }
        }
        return strs;
    }
};

int main() {
    Codec c;
    vector<string> want;

    want = {"hello", "world"}; // target
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    want = {"3#ab", "a,\\b"}; // edge: encoding chars and digits inside the content
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    want = {"", ""}; // edge: empty strings
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    want = {}; // null: empty list must not come back as {""}
    assert(c.decode_1(c.encode_1(want)) == want);
    assert(c.decode_2(c.encode_2(want)) == want);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p38"></a>

## 38. Basic Calculator I / II

**Source:** LC224, LC227 · **File:** `basic-calculator.cpp`

**Key idea:** 224: each number adds sign*num; '(' saves (result, sign) on a stack. 227: keep the open term `last`; * and / change it, + and - fold it into result.

**Watch:** 227 applies at operators AND at the last character. C++ `/` truncates toward zero. isdigit needs unsigned char.

```cpp
// evaluate an integer expression string, no eval()
// LC224: + - ( ) and unary minus, no * /
// LC227: + - * / with precedence, no parentheses, / truncates toward zero

// which operators? parentheses? unary minus (or plus)?
// spaces anywhere? only non-negative integer literals?
// division: truncate toward zero?
// input always valid? result and intermediates fit in int?

#include <string>
#include <stack>
#include <cctype>
#include <cassert>
#include <iostream>

using std::string;

class Calculator {
public:
    // LC224: each number adds sign*num; '(' saves (result, sign) and starts fresh
    // T: O(n), S: O(depth of parentheses)
    long long calculateParens(const string& s) {
        std::stack<long long> saved; // result before '(' then the sign in front of '('
        long long result = 0; // value of the current parenthesis level so far
        long long sign = 1; // sign for the number being read
        long long num = 0;
        for (char c : s) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                num = num * 10 + (c - '0');
            } else if (c == '+' || c == '-') {
                result += sign * num;
                num = 0;
                sign = (c == '+') ? 1 : -1; // also covers unary minus: result += 0 first
            } else if (c == '(') {
                saved.push(result);
                saved.push(sign);
                result = 0;
                sign = 1;
            } else if (c == ')') {
                result += sign * num;
                num = 0;
                result *= saved.top(); // sign in front of '(' pops first
                saved.pop();
                result += saved.top();
                saved.pop();
            }
        }
        return result + sign * num;
    }

    // LC227: keep the pending term `last`; * and / change it, + and - fold it into result
    // T: O(n), S: O(1)
    long long calculatePrecedence(const string& s) {
        long long result = 0; // sum of finished terms
        long long last = 0; // current term, signed, still open to * and /
        long long num = 0;
        char op = '+'; // operator in front of num
        int n = static_cast<int>(s.size());
        for (int i = 0; i < n; i++) {
            char c = s[i];
            if (std::isdigit(static_cast<unsigned char>(c))) {
                num = num * 10 + (c - '0');
            }
            if ((!std::isdigit(static_cast<unsigned char>(c)) && c != ' ') || i == n - 1) { // operator or end
                if (op == '+') {
                    result += last;
                    last = num;
                } else if (op == '-') {
                    result += last;
                    last = -num;
                } else if (op == '*') {
                    last *= num;
                } else {
                    last /= num; // C++ truncates toward zero, matches LC
                }
                op = c;
                num = 0;
            }
        }
        return result + last;
    }
};

int main() {
    Calculator calc;

    assert(calc.calculateParens("2-(5-6)") == 3); // target: minus in front of parentheses
    assert(calc.calculateParens("-(2+3)") == -5); // edge: unary minus at the start
    assert(calc.calculateParens("1-(-(2))") == 3); // edge: nested unary minus
    assert(calc.calculateParens("7") == 7); // null: single number

    assert(calc.calculatePrecedence("3+2*2") == 7); // target: * before +
    assert(calc.calculatePrecedence("1-7/2") == -2); // edge: division on a negative term truncates toward zero
    assert(calc.calculatePrecedence(" 3/2 ") == 1); // edge: spaces, last char is a space
    assert(calc.calculatePrecedence("42") == 42); // null: single number

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p39"></a>

## 39. Simplify Path

**Source:** LC71 (company tag) · **File:** `simplify-path.cpp`

**Key idea:** Split on '/', stack of names: '..' pops (not above root), '' and '.' skipped.

**Watch:** Empty result prints '/'. '...' is a plain name.

```cpp
// absolute Unix path -> canonical path (LC71)
// "." stays, ".." goes up one level (never above root), repeated '/' count as one, no trailing '/'

// input always absolute (starts with '/')?
// ".." at the root: stay at root?
// names like "..." are plain names?
// symlinks? (no, purely textual)

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class PathSimplifier {
public:
    // split on '/', keep a stack of names, T: O(n), S: O(n)
    string simplifyPath(const string& path) {
        vector<string> parts; // directory names from root to current
        int n = static_cast<int>(path.size());
        int i = 0;
        while (i < n) {
            while (i < n && path[i] == '/') {
                i++; // skip any run of slashes
            }
            int j = i;
            while (j < n && path[j] != '/') {
                j++;
            }
            string name = path.substr(i, j - i);
            if (name == "..") {
                if (!parts.empty()) {
                    parts.pop_back(); // at root, ".." stays at root
                }
            } else if (!name.empty() && name != ".") {
                parts.push_back(name);
            }
            i = j;
        }

        string out;
        for (const string& p : parts) {
            out += "/" + p;
        }
        return out.empty() ? "/" : out;
    }
};

int main() {
    PathSimplifier ps;

    assert(ps.simplifyPath("/a/./b/../../c/") == "/c"); // target: . and .. together
    assert(ps.simplifyPath("/home//foo/") == "/home/foo"); // edge: double slash, trailing slash
    assert(ps.simplifyPath("/../") == "/"); // fault: .. above root
    assert(ps.simplifyPath("/...") == "/..."); // edge: "..." is a plain name
    assert(ps.simplifyPath("/") == "/"); // null: root only

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p40"></a>

## 40. Reverse Each Alphanumeric Run

**Source:** Q43 (J10690) · **File:** `reverse-alnum-runs.cpp`

**Key idea:** 1: build a new string, flush each run reversed at separators. 2: in place, find [i, j) and std::reverse.

**Watch:** Flush the last run after the loop. ASCII check by hand, or isalnum with unsigned char.

```cpp
// reverse every maximal run of ASCII letters/digits; every other character stays where it is (Q43, J10690)
// "ab12-cd!" -> "21ba-dc!"

// alphanumeric = ASCII A-Z a-z 0-9 only? (std::isalnum depends on locale)
// modify in place, or return a new string?
// empty string, no separators, only separators?

#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::string;

class RunReverser {
public:
    // 1: build a new string, flush each run reversed when a separator shows up, T: O(n), S: O(n)
    string reverseRuns_1(const string& s) {
        string out;
        string run; // current run of alphanumerics, not yet written
        for (char c : s) {
            if (isAlnum(c)) {
                run += c;
            } else {
                out.append(run.rbegin(), run.rend());
                run.clear();
                out += c; // separator keeps its position
            }
        }
        out.append(run.rbegin(), run.rend()); // last run has no separator after it
        return out;
    }

    // 2: in place, find each run [i, j) and reverse it, T: O(n), S: O(1) extra
    void reverseRuns_2(string& s) {
        int n = static_cast<int>(s.size());
        int i = 0;
        while (i < n) {
            if (!isAlnum(s[i])) {
                i++;
                continue;
            }
            int j = i;
            while (j < n && isAlnum(s[j])) {
                j++;
            }
            std::reverse(s.begin() + i, s.begin() + j); // two-pointer swap inside [i, j)
            i = j;
        }
    }

private:
    bool isAlnum(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    }
};

int main() {
    RunReverser rr;
    string s;

    assert(rr.reverseRuns_1("ab12-cd!") == "21ba-dc!"); // target
    s = "ab12-cd!";
    rr.reverseRuns_2(s);
    assert(s == "21ba-dc!");

    assert(rr.reverseRuns_1("!!ab..c") == "!!ba..c"); // edge: separators at the start, back to back
    s = "!!ab..c";
    rr.reverseRuns_2(s);
    assert(s == "!!ba..c");

    assert(rr.reverseRuns_1("abc") == "cba"); // edge: no separator, last run flushed
    s = "abc";
    rr.reverseRuns_2(s);
    assert(s == "cba");

    assert(rr.reverseRuns_1("") == ""); // null
    s = "";
    rr.reverseRuns_2(s);
    assert(s == "");

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p41"></a>

## 41. Count Vowel Substrings

**Source:** LC2062 (company tag) · **File:** `vowel-substrings.cpp`

**Key idea:** 1: every start, extend until a consonant (O(n^2)). 2: for each end, valid starts lie in (lastConsonant, earliest of the five latest vowel positions].

**Watch:** Add `minLast - lastConsonant` only when positive.

```cpp
// count substrings made only of vowels (a e i o u) that contain all five (LC2062)

// lowercase letters only?
// substrings counted by position (same text at two places counts twice)?

#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::string;
using std::vector;

class VowelSubstrings {
public:
    // 1: every start, extend until a consonant, T: O(n^2), S: O(1) (set holds at most 5)
    int count_1(const string& s) {
        int n = static_cast<int>(s.size());
        int total = 0;
        for (int i = 0; i < n; i++) {
            std::unordered_set<char> vowels; // distinct vowels in s[i..j]
            for (int j = i; j < n && isVowel(s[j]); j++) {
                vowels.insert(s[j]);
                if (vowels.size() == 5) {
                    total++;
                }
            }
        }
        return total;
    }

    // 2: for each end r, valid starts are (lastConsonant, earliest of the five latest vowel spots]
    // T: O(n), S: O(1)
    int count_2(const string& s) {
        string order = "aeiou";
        vector<int> last(5, -1); // vowel -> latest index seen
        int lastConsonant = -1;
        int total = 0;
        for (int r = 0; r < static_cast<int>(s.size()); r++) {
            if (!isVowel(s[r])) {
                lastConsonant = r;
                continue;
            }
            last[order.find(s[r])] = r;
            int minLast = *std::min_element(last.begin(), last.end()); // start must reach every vowel
            if (minLast > lastConsonant) {
                total += minLast - lastConsonant; // starts lastConsonant+1 .. minLast
            }
        }
        return total;
    }

private:
    bool isVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }
};

int main() {
    VowelSubstrings vs;

    assert(vs.count_1("aeiouu") == 2); // target: "aeiou", "aeiouu"
    assert(vs.count_2("aeiouu") == 2);
    assert(vs.count_1("cuaieuouac") == 7); // edge: consonants on both sides
    assert(vs.count_2("cuaieuouac") == 7);
    assert(vs.count_1("aeio") == 0); // fault: one vowel missing
    assert(vs.count_2("aeio") == 0);
    assert(vs.count_1("") == 0); // null
    assert(vs.count_2("") == 0);

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Arrays and two pointers

<a id="p42"></a>

## 42. 0/1/2 Array: distance from 1s to 2s

**Source:** Q19 / A11 (C09 NG round 2), J10352; LC821, LC243 · **File:** `array-nearest-distance.cpp`

**Key idea:** Each 1's nearest 2: two passes carrying the last 2 seen from each side. Closest pair overall: one pass tracking the latest 1 and latest 2.

**Watch:** Clarify which distance first (nearest per 1, closest pair, or all pairs). -1 sentinels need a check before `<`.

```cpp
// array of 0 (empty) / 1 / 2, distance = index difference (Q19 A11, C09 NG round 2)
// version A: for each 1, in order, distance to its nearest 2, -1 if no 2 (J10352, LC821 pattern)
// version B: shortest distance over any (1, 2) pair, -1 if none (LC243 pattern)

// each 1's nearest 2, the global closest pair, or all pairs?
// distance = index difference? 0s don't block?
// output order: by position of the 1s?
// no 2 at all: -1 per 1? no 1: empty?
// 1D or 2D? (2D -> multi-source BFS from every 2)

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class ArrayDistance {
public:
    // version A: two passes, nearest 2 is either the last 2 on the left or the first 2 on the right
    // T: O(n), S: O(n)
    vector<int> nearestTwoPerOne(const vector<int>& a) {
        int n = static_cast<int>(a.size());
        vector<int> dist(n, -1); // index of a 1 -> distance to nearest 2 found so far, -1 = none yet
        int last = -1; // index of the closest 2 on the side already scanned
        for (int i = 0; i < n; i++) {
            if (a[i] == 2) {
                last = i;
            } else if (a[i] == 1 && last != -1) {
                dist[i] = i - last;
            }
        }

        last = -1;
        for (int i = n - 1; i >= 0; i--) {
            if (a[i] == 2) {
                last = i;
            } else if (a[i] == 1 && last != -1) {
                if (dist[i] == -1 || last - i < dist[i]) {
                    dist[i] = last - i;
                }
            }
        }

        vector<int> result; // one entry per 1, in order of appearance
        for (int i = 0; i < n; i++) {
            if (a[i] == 1) {
                result.push_back(dist[i]);
            }
        }
        return result;
    }

    // version B: one pass, the closest pair is adjacent in the 1/2 subsequence, so only the latest of each matters
    // T: O(n), S: O(1)
    int closestPair(const vector<int>& a) {
        int last1 = -1; // index of the latest 1 seen
        int last2 = -1; // index of the latest 2 seen
        int best = -1;
        for (int i = 0; i < static_cast<int>(a.size()); i++) {
            if (a[i] == 1) {
                last1 = i;
                if (last2 != -1 && (best == -1 || i - last2 < best)) {
                    best = i - last2;
                }
            } else if (a[i] == 2) {
                last2 = i;
                if (last1 != -1 && (best == -1 || i - last1 < best)) {
                    best = i - last1;
                }
            }
        }
        return best;
    }
};

int main() {
    ArrayDistance ad;
    vector<int> want;

    want = {2, 2, 3};
    assert(ad.nearestTwoPerOne({1, 0, 2, 0, 1, 1}) == want); // 2 on the right, then on the left
    want = {1};
    assert(ad.nearestTwoPerOne({2, 0, 0, 1, 2}) == want); // 2 on both sides, right one is nearer
    want = {-1, -1};
    assert(ad.nearestTwoPerOne({1, 0, 1}) == want); // no 2 at all
    assert(ad.nearestTwoPerOne({2, 0}).empty()); // no 1
    assert(ad.nearestTwoPerOne({}).empty()); // empty

    assert(ad.closestPair({1, 0, 2, 0, 1, 1}) == 2);
    assert(ad.closestPair({2, 0, 0, 1, 2}) == 1); // pair found when the 2 comes after the 1
    assert(ad.closestPair({1, 0, 1}) == -1); // no 2
    assert(ad.closestPair({}) == -1);

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p43"></a>

## 43. Trapping Rain Water

**Source:** LC42 (company tag) · **File:** `trapping-rain-water.cpp`

**Key idea:** 1: prefix and suffix max arrays. 2: two pointers, always move the lower side (its max is final).

**Watch:** `long long` total. Guard n == 0 in the array version.

```cpp
// bar heights, width 1 each: how much rain water is trapped between the bars (LC42)

// heights non-negative?
// empty input or fewer than 3 bars?
// total can exceed int? (n * max height)

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class RainWater {
public:
    // 1: water above bar i = min(tallest on its left, tallest on its right) - height[i]
    // precompute both maxima, T: O(n), S: O(n)
    long long trap_1(const vector<int>& height) {
        int n = static_cast<int>(height.size());
        if (n == 0) {
            return 0;
        }
        vector<int> leftMax(n); // i -> tallest bar in [0, i]
        vector<int> rightMax(n); // i -> tallest bar in [i, n-1]
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++) {
            leftMax[i] = std::max(leftMax[i - 1], height[i]);
        }
        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; i--) {
            rightMax[i] = std::max(rightMax[i + 1], height[i]);
        }
        long long water = 0;
        for (int i = 0; i < n; i++) {
            water += std::min(leftMax[i], rightMax[i]) - height[i];
        }
        return water;
    }

    // 2: two pointers; the lower side is the limit, the other side surely has a wall at least as tall
    // T: O(n), S: O(1)
    long long trap_2(const vector<int>& height) {
        int l = 0;
        int r = static_cast<int>(height.size()) - 1;
        int leftMax = 0; // tallest in [0, l]
        int rightMax = 0; // tallest in [r, n-1]
        long long water = 0;
        while (l < r) {
            if (height[l] < height[r]) {
                leftMax = std::max(leftMax, height[l]);
                water += leftMax - height[l]; // right side has height[r] > height[l], so leftMax decides
                l++;
            } else {
                rightMax = std::max(rightMax, height[r]);
                water += rightMax - height[r];
                r--;
            }
        }
        return water;
    }
};

int main() {
    RainWater rw;

    assert(rw.trap_1({3, 0, 2, 0, 4}) == 7); // target: 3 + 1 + 3
    assert(rw.trap_2({3, 0, 2, 0, 4}) == 7);
    assert(rw.trap_1({2, 0, 2}) == 2); // edge: equal walls
    assert(rw.trap_2({2, 0, 2}) == 2);
    assert(rw.trap_1({1, 2, 3}) == 0); // fault: rising bars, no right wall
    assert(rw.trap_2({1, 2, 3}) == 0);
    assert(rw.trap_1({}) == 0); // null
    assert(rw.trap_2({}) == 0);

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Backtracking

<a id="p44"></a>

## 44. Word Search II

**Source:** LC212; E01, E02, C12 · **File:** `word-search-trie.cpp`

**Key idea:** Trie of all words (TrieNode pool) + one DFS per cell walking the trie; prune when no child; clear `wordIdx` after reporting.

**Watch:** Push the new node before writing its index (vector reallocation). Mark '#' and restore.

```cpp
// board of letters, list of words: return every word that can be traced through adjacent cells
// 4 directions, a cell is used at most once per word (LC212)

// lowercase a-z only?
// 4 directions? a cell reused within one word?
// duplicate words in the list? output order?
// may the board be modified during the search (restored after)?
// board size, word length bounds?

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class WordFinder {
private:
    struct TrieNode {
        vector<int> children = vector<int>(26, -1); // letter -> child index in pool, -1 = none
        int wordIdx = -1; // index into words if a word ends here, -1 = none or already reported
    };
    vector<TrieNode> pool; // pool[0] is the root

public:
    // trie of all words + one DFS per cell that walks the trie alongside the board
    // T: O(m*n * 4*3^(L-1)) worst, L: longest word; S: O(26 * total chars) trie + O(L) recursion
    // board taken by value: cells are marked during the search
    vector<string> findWords(vector<vector<char>> board, const vector<string>& words) {
        buildTrie(words);
        vector<string> found;
        if (board.empty()) {
            return found;
        }
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                dfs(board, r, c, 0, words, found);
            }
        }
        return found;
    }

private:
    void buildTrie(const vector<string>& words) {
        pool.assign(1, TrieNode());
        for (int wi = 0; wi < static_cast<int>(words.size()); wi++) {
            int cur = 0;
            for (char ch : words[wi]) {
                int k = ch - 'a';
                if (pool[cur].children[k] == -1) {
                    int child = static_cast<int>(pool.size());
                    pool.push_back(TrieNode()); // may reallocate pool
                    pool[cur].children[k] = child; // so index into pool only after the push
                }
                cur = pool[cur].children[k];
            }
            pool[cur].wordIdx = wi;
        }
    }

    // invariant: the path so far spells the prefix that leads to trie node `node`
    void dfs(vector<vector<char>>& board, int r, int c, int node,
             const vector<string>& words, vector<string>& found) {
        if (r < 0 || r >= static_cast<int>(board.size()) || c < 0 || c >= static_cast<int>(board[0].size())) {
            return;
        }
        char ch = board[r][c];
        if (ch == '#') {
            return; // already on this path
        }
        int child = pool[node].children[ch - 'a'];
        if (child == -1) {
            return; // no word continues with this letter: prune
        }
        if (pool[child].wordIdx != -1) {
            found.push_back(words[pool[child].wordIdx]);
            pool[child].wordIdx = -1; // report each word once
        }

        board[r][c] = '#';
        dfs(board, r + 1, c, child, words, found);
        dfs(board, r - 1, c, child, words, found);
        dfs(board, r, c + 1, child, words, found);
        dfs(board, r, c - 1, child, words, found);
        board[r][c] = ch; // restore for other paths
    }
};

int main() {
    WordFinder wf;
    vector<vector<char>> board = {{'a', 'b'},
                                  {'c', 'd'}};
    vector<string> want;

    want = {"abdc", "acdb"};
    vector<string> got = wf.findWords(board, {"abdc", "acdb", "abc"});
    std::sort(got.begin(), got.end()); // any order is valid
    assert(got == want); // target: two paths found, "abc" needs a diagonal

    want = {"ab"};
    assert(wf.findWords(board, {"ab", "ab", "aba"}) == want); // edge: duplicate word once, no cell reuse
    assert(wf.findWords(board, {"z"}).empty()); // fault: letter not on the board
    assert(wf.findWords({}, {"a"}).empty()); // null: empty board

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p45"></a>

## 45. Work Schedule: all valid schedules

**Source:** Q36 · **File:** `work-schedule.cpp`

**Key idea:** Subtract fixed days; fill '?' slots left to right with 0..D ascending (lexicographic output), pruning when remaining is outside [0, slotsLeft*D].

**Watch:** Fixed digits may exceed dayHours. Complexity includes output size.

```cpp
// pattern of 7 chars, digits are fixed hours, '?' takes 0..dayHours
// return every filled pattern whose 7 days sum to exactly workHours, in lexicographic order (Q36)

// pattern always 7 chars of '0'-'9' and '?'?
// dayHours <= 9 so each day stays one digit?
// fixed digit larger than dayHours: allowed as is?
// no valid schedule: empty list?

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class ScheduleBuilder {
public:
    // DFS over '?' slots left to right, digits 0..D ascending -> output already sorted
    // T: O((D+1)^m + 7A), m: '?' count, A: answers; S: O(m) recursion + O(7A) output
    vector<string> findSchedules(const string& pattern, int workHours, int dayHours) {
        vector<int> slots; // indices of '?'
        int remaining = workHours; // hours still to place on '?' days
        for (int i = 0; i < static_cast<int>(pattern.size()); i++) {
            if (pattern[i] == '?') {
                slots.push_back(i);
            } else {
                remaining -= pattern[i] - '0';
            }
        }
        string cur = pattern;
        vector<string> result;
        fill(cur, slots, 0, remaining, dayHours, result);
        return result;
    }

private:
    void fill(string& cur, const vector<int>& slots, int idx, int remaining, int dayHours, vector<string>& result) {
        int left = static_cast<int>(slots.size()) - idx; // '?' days not yet filled
        if (remaining < 0 || remaining > left * dayHours) {
            return; // prune: total can no longer be hit exactly
        }
        if (left == 0) {
            result.push_back(cur); // remaining == 0 here, the prune rejected everything else
            return;
        }
        for (int h = 0; h <= dayHours && h <= remaining; h++) {
            cur[slots[idx]] = static_cast<char>('0' + h); // overwritten on every try, no restore needed
            fill(cur, slots, idx + 1, remaining - h, dayHours, result);
        }
    }
};

int main() {
    ScheduleBuilder sb;
    vector<string> want;

    want = {"0020100", "0021000", "0120000", "1020000"};
    assert(sb.findSchedules("??2??00", 3, 2) == want); // target: doc example, already sorted
    want = {"1111111"};
    assert(sb.findSchedules("1111111", 7, 8) == want); // null: no '?', fixed days already match
    assert(sb.findSchedules("8??????", 5, 2).empty()); // fault: fixed hours already exceed the total
    want = {"0000000"};
    assert(sb.findSchedules("?000000", 0, 0) == want); // edge: dayHours = 0

    std::cout << "all tests passed\n";
    return 0;
}
```


---

# Warm-ups

<a id="p46"></a>

## 46. Happy Number

**Source:** LC202 (J10271) · **File:** `happy-number.cpp`

**Key idea:** 1: seen set. 2: Floyd slow/fast.

**Watch:** Start fast at next(n).

```cpp
// replace n by the sum of the squares of its digits; happy if this reaches 1, else it loops forever (LC202, J10271)

// n positive?
// return only true/false?

#include <unordered_set>
#include <cassert>
#include <iostream>

class HappyNumber {
public:
    // 1: remember every value seen, a repeat means a cycle without 1, T: O(k * digits), S: O(k), k: steps
    bool isHappy_1(int n) {
        std::unordered_set<int> seen; // values already produced
        while (n != 1 && seen.insert(n).second) {
            n = next(n);
        }
        return n == 1;
    }

    // 2: Floyd, slow moves 1 step, fast moves 2; they meet inside the cycle, T: O(k * digits), S: O(1)
    bool isHappy_2(int n) {
        int slow = n;
        int fast = next(n);
        while (fast != 1 && slow != fast) {
            slow = next(slow);
            fast = next(next(fast));
        }
        return fast == 1;
    }

private:
    int next(int n) {
        int sum = 0;
        while (n > 0) {
            int d = n % 10;
            sum += d * d;
            n /= 10;
        }
        return sum;
    }
};

int main() {
    HappyNumber h;

    assert(h.isHappy_1(19)); // target: 19 -> 82 -> 68 -> 100 -> 1
    assert(h.isHappy_2(19));
    assert(!h.isHappy_1(2)); // fault: 2 enters the 4 -> 16 -> ... -> 4 cycle
    assert(!h.isHappy_2(2));
    assert(h.isHappy_1(1)); // null: already 1
    assert(h.isHappy_2(1));

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p47"></a>

## 47. Calculate Amount Paid in Taxes

**Source:** LC2303 (E01) · **File:** `tax-brackets.cpp`

**Key idea:** Tax each income slice inside its bracket; keep hundredths in long long, divide once.

**Watch:** Ask: income above the last bracket untaxed or last rate?

```cpp
// brackets[i] = {upper_i, percent_i}, sorted by upper; income in (upper_{i-1}, upper_i] taxed at percent_i
// return total tax (LC2303, E01 warm-up)

// brackets sorted, uppers strictly increasing?
// income above the last upper: untaxed, or keep the last rate? (J10689 vs J9367 differ)
// return a double, or exact cents?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class TaxCalculator {
public:
    // walk brackets, tax the slice of income inside each, T: O(B), S: O(1)
    double calculateTax(const vector<vector<int>>& brackets, int income) {
        long long hundredths = 0; // tax * 100, kept exact until the end
        int prevUpper = 0;
        for (const auto& b : brackets) {
            if (income <= prevUpper) {
                break; // nothing left to tax
            }
            int slice = std::min(income, b[0]) - prevUpper;
            hundredths += 1LL * slice * b[1];
            prevUpper = b[0];
        }
        return hundredths / 100.0; // one division, no accumulated rounding
    }
};

int main() {
    TaxCalculator tc;

    assert(tc.calculateTax({{3, 50}, {7, 10}, {12, 25}}, 10) == 2.65); // target: 1.5 + 0.4 + 0.75
    assert(tc.calculateTax({{3, 50}, {7, 10}, {12, 25}}, 3) == 1.5); // edge: income exactly on an upper
    assert(tc.calculateTax({{3, 50}}, 0) == 0.0); // null: no income

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p48"></a>

## 48. Maximum Profit Query Selection

**Source:** Q37 · **File:** `query-profit.cpp`

**Key idea:** One pass: total = floor(K / time) * profit; strict > keeps the smallest index on ties.

**Watch:** Use bestIdx == -1, not best == 0, to mean 'nothing fits'. Not a profit/time ratio greedy.

```cpp
// query type i takes time[i] and earns profit[i]; time budget K; pick ONE type and run it as often as fits
// total = floor(K / time[i]) * profit[i]; return {best total, its index}, ties -> smallest index,
// no type fits at all -> {0, -1} (Q37)

// time positive? profit can be 0 or negative?
// only one type, repeated? (not a knapsack)
// tie on total: smallest index?
// K and totals beyond int?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

class QueryPlanner {
public:
    // one pass, T: O(n), S: O(1)
    vector<long long> maxProfit(const vector<int>& time, const vector<int>& profit, long long K) {
        long long best = 0;
        int bestIdx = -1; // -1 = nothing fits yet; best == 0 alone can't tell "fits but earns 0"
        for (int i = 0; i < static_cast<int>(time.size()); i++) {
            if (time[i] > K) {
                continue; // can't run even once
            }
            long long total = (K / time[i]) * profit[i]; // whole runs only
            if (bestIdx == -1 || total > best) { // strict >: a tie keeps the smaller index
                best = total;
                bestIdx = i;
            }
        }
        return {best, static_cast<long long>(bestIdx)};
    }
};

int main() {
    QueryPlanner qp;
    vector<long long> want;

    want = {15, 1};
    assert(qp.maxProfit({3, 5}, {10, 15}, 5) == want); // target: better ratio 10/3 only earns 10
    want = {4, 0};
    assert(qp.maxProfit({2, 4}, {2, 4}, 4) == want); // edge: tie on total, smallest index
    want = {0, 0};
    assert(qp.maxProfit({1}, {0}, 3) == want); // edge: fits but earns 0, still a real answer
    want = {0, -1};
    assert(qp.maxProfit({6}, {9}, 5) == want); // fault: nothing fits in the budget
    assert(qp.maxProfit({}, {}, 5) == want); // null: no query types

    std::cout << "all tests passed\n";
    return 0;
}
```

<a id="p49"></a>

## 49. Any Path with +2, x2, floor /2

**Source:** Q44 (J10691) · **File:** `integer-ops-path.cpp`

**Key idea:** Split to 1, dub to 2 if b is even, then add to b; every phase is monotone, every value positive.

**Watch:** Output length is O(log a + b): don't claim a bound independent of b. Tests replay the path.

```cpp
// positive integers a, b; operations: add (+2), dub (x2), split (floor /2)
// return ANY sequence turning a into b with every intermediate value positive, not the shortest (Q44, J10691)

// any valid sequence, or shortest?
// a == b: empty sequence?
// every intermediate must stay positive?
// very large b: is a long output acceptable?

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class OpsPath {
public:
    // split down to 1, then dub to 2 if b is even, then add up to b; each phase only moves one way
    // T: O(log a + b), the output length, S: O(log a + b)
    vector<string> buildPath(long long a, long long b) {
        vector<string> ops;
        if (a == b) {
            return ops;
        }
        while (a > 1) {
            ops.push_back("split"); // floor(a / 2) >= 1 while a > 1, stays positive
            a /= 2;
        }
        if (b % 2 == 0) {
            ops.push_back("dub"); // 1 -> 2, now adds keep the parity of b
            a = 2;
        }
        while (a < b) {
            ops.push_back("add");
            a += 2;
        }
        return ops;
    }
};

// test helper: replay ops from a, -1 if any value goes non-positive
long long runOps(long long a, const vector<string>& ops) {
    for (const string& op : ops) {
        if (op == "add") {
            a += 2;
        } else if (op == "dub") {
            a *= 2;
        } else {
            a /= 2;
        }
        if (a <= 0) {
            return -1;
        }
    }
    return a;
}

int main() {
    OpsPath op;

    assert(runOps(5, op.buildPath(5, 6)) == 6); // target: odd a, even b
    assert(runOps(8, op.buildPath(8, 7)) == 7); // edge: even a, odd b, no dub
    assert(runOps(10, op.buildPath(10, 1)) == 1); // edge: b = 1, splits only
    assert(op.buildPath(4, 4).empty()); // null: a == b

    std::cout << "all tests passed\n";
    return 0;
}
```


## C++ cheat sheet (pitfalls that came up)

| Topic | Rule |
|---|---|
| `priority_queue` | Max-heap by default; min-heap = `priority_queue<T, vector<T>, std::greater<T>>`. Comparator returns true when `a` should sink below `b`. Class members: comparator struct, not a lambda. |
| `pair` / `tuple` keys | Lexicographic comparison for free; negate a field to flip it. Brace-init of `tuple` in `push({...})` needs C++17: use `emplace(...)` or `make_tuple` in C++11. |
| `set` / `map` | Red-black tree in libstdc++/libc++/MSVC: O(log n), sorted, iterators stable; erase invalidates only the erased element. |
| `multiset` / `multimap` | Duplicates kept, new equal keys go after old ones. `erase(it)` removes one copy, `erase(value)` removes all. `count` is O(log n + copies). |
| `unordered_map` | `operator[]` inserts on read: use `find` / `count` for lookups. A rehash invalidates iterators, not references. |
| `vector` | Reallocation invalidates references and iterators: push first, then index. Index loops over a growing vector re-read `size()` each iteration. |
| `std::list` | `splice` moves a node in O(1) and keeps iterators valid (LRU). |
| `emplace` vs `push` | `emplace` takes constructor arguments (works with explicit constructors); aggregates without a constructor use `push_back({...})`. |
| Integer width | `long long`; widen before the operation: `1LL * a * b`, `static_cast<long long>(x) + y`. Compare by division to avoid overflow (`need > (budget - cost) / c`). |
| Division | `/` and `%` truncate toward zero; floor division needs a correction for negative results. |
| Binary search | Half-open `[lo, hi)`: `hi = n`, `while (lo < hi)`, `hi = mid`. Last-true search: `mid = lo + (hi - lo + 1) / 2`. |
| `upper_bound` / `prev` | Last element <= t is `prev(upper_bound(t))`, only if the result is not `begin()`. |
| `isdigit` / `isalnum` | Pass `static_cast<unsigned char>(c)`. |
| Recursion depth | Deep chains (10^4+): offer an explicit stack. |
| Version check | `std::cout << __cplusplus;` prints 201103 / 201402 / 201703 / 202002. |

