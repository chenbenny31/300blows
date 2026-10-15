# Snowflake NG interview preparation — session handoff

Updated: 2026-10-09, America/Los_Angeles.
Workspace: `/home/benny/Code/300blows/company/snowflake`.

This file preserves the preparation plan, user preferences, teaching content, review history, and next action from this conversation. It is a handoff, not a claim that all references or proposed snippets have been tested. Existing source files remain the code source of truth.

## Goal and source material

Prepare minimal C++ solutions that the user can write during an interview, based on current `ref/*.cpp`. Present problems one at a time in an evidence-informed NG preparation order, with tests, complexity, and important follow-ups. The user writes their own implementations in the workspace root; the assistant reviews them.

- Original prep guide: `/home/benny/Downloads/Snowflake_Algo_Prep_20261006_86LC.md` (outside this repository; not copied here).
- Actual handout: [ref/snowflake-handout.md](ref/snowflake-handout.md). The initially requested `ref/handout.md` does not exist.
- Reference implementation collection: 49 `.cpp` files in `ref/`, corresponding to 49 handout problem groups. Some contain multiple algorithms or variants.
- Prep guide: 86 distinct LC IDs, consisting of 21 main, 42 foundational/prototype, and 23 extension IDs. These are not 86 independently implemented interview questions, and 49/86 is not a meaningful coverage percentage.
- The original large guide and handout reads were output-truncated. Focused sections and the handout's complete source/key-idea/watch summaries were subsequently inspected. Do not claim exhaustive line-by-line review or correctness auditing of all references.

Interview scope reported by the supplied materials: two back-to-back technical rounds, C++, medium–hard algorithms, no DP expected for the screens, no AI during the actual interview, clarify requirements before coding, explain the approach, write and run tests. Later onsite includes coding/BQ/SD/HM. These are supplied-context claims, not newly verified scheduling facts.

Rankings are preparation judgments based on the supplied evidence, not measured probabilities. C09 explicitly describes NG grid BFS in round 1 and array distances in round 2. Several Jobright entries have NG labels, but labels/aggregated descriptions are weaker evidence than complete candidate reports. Do not treat duplicate aggregations as independent interview occurrences.

## User preferences — apply on every continuation

1. Show one problem at a time. `next` advances to the next group.
2. Main problem: complete minimal C++17 code, runnable `main`, target assertion, and a compact set of important tests with comments explaining their purpose.
3. Include brief problem semantics, material clarifications, an interview explanation, and time/space complexity.
4. Use `V` and `E` for graphs. Explain extra symbols such as privilege counts or obstacle budget.
5. Use separate `dr[]` and `dc[]` arrays with the same `d` index, `0 <= d < 4`. Do not use the compact five-element direction trick.
6. Important follow-ups: show only the crucial code changes, where they go, changed semantics, and complexity. The earlier full-code follow-up format was superseded.
7. Minor follow-ups: one or two sentences of reasoning. Prefer evidence-supported follow-ups; explicitly label additional practice extensions.
8. `check` / `check code` means inspect the user's solution in this directory, not an unspecified pasted snippet. Identify the currently relevant root source file.
9. For reviews, compile the MAIN solution plus its existing target test only. The user expressly permits a temporary copy and wants this to be fast. Follow-up snippets appended below `main` are intentional notes: review their logic separately, without repeatedly complaining that they are outside a function.
10. For reviews, report ONLY errors. Avoid routine praise, broad summaries, and unnecessary commentary. Still distinguish a compile failure from a test pass. A brief no-errors result is appropriate when needed.
11. Do not fix the user's C++ files automatically. The user owns implementation; reviews provide corrections for them to type.
12. An earlier assistant promise to use C++11 was superseded after checking CoderPad's C++17 support.

### C++ and CoderPad discussion

- `auto x = ...` is supported in C++11; `auto [r, c] = ...` is a structured binding requiring C++17.
- C++11 alternative: read `q.front().first` / `.second`, or use `auto cell = q.front()` and then its members.
- [CoderPad's official C++ page](https://coderpad.io/languages/cpp/) was searched during the conversation and specified GCC 8 with `-std=c++17`. This supports the structured-binding syntax in its documented Interview environment; it is not verification of an individual interview pad's configuration.

## Progress and upcoming order

Four of the 49 groups have been PRESENTED. Do not equate presentation with validated completion. Therefore 45 existing groups have not yet been presented, plus 4 planned additions.

| Position | Problem | Status / reference |
|---|---|---|
| 1 | Grid shortest path | Presented; user's base target passed; `ref/grid-shortest-path.cpp` |
| 2 | Nearest 2 for each 1 | Presented; user's base target passed; `ref/array-nearest-distance.cpp` |
| 3 | Parallel courses / semesters | Presented; latest observed typo fixed, not rerun; `ref/parallel-courses.cpp` |
| 4 | DAG privilege inheritance | Presented; latest observed compile typos fixed, not rerun; `ref/dag-privilege-inheritance.cpp` |
| 5 | Parent-array forest deletion and reindexing | NEXT; `ref/forest-delete-node.cpp` |
| 6 | Build a new tree of subtree sums | Pending; `ref/subtree-sum-tree.cpp` |
| 7 | Binary search with repeated queries | Pending; `ref/binary-search-queries.cpp` |
| 8 | Tree height after deletion with child promotion | Pending; `ref/tree-height-deleted-nodes.cpp` |
| 9 | Web crawler / shortest click path | Pending; `ref/web-crawler.cpp` |
| 10 | Priority executor with duplicate IDs | Pending; `ref/task-executor-dedup.cpp` |

The complete order after the first ten has not been fixed. Course Schedule II remains a separate reference group even though it overlaps with parallel courses; do not silently double-count it as already completed.

### Remaining reference groups beyond the first ten

All ancestors in a DAG; Course Schedule II; bipartition; whole-subtree deletion to bound depth; subtree averages; tree boundary; tree directions; throne inheritance; quad tree; file system; TTL KV; transaction KV; time-based KV; LRU with resize; hit counter; log storage; event store; dynamic blacklist; min stack; task manager; meeting rooms/buses; stream median; top-k frequent elements; running book sales; most frequent IDs; rotated-array minimum; maximum throughput; encode/decode strings; Basic Calculator I/II; simplify path; reverse alphanumeric runs; vowel substrings; trapping rain water; Word Search II; work schedule; happy number; tax brackets; query profit; integer-operation path.

### Planned additions and scope limits

After prioritizing the existing material, add:

1. Connect Four detection around the latest move.
2. Nested `ADD/MULT` calculator.
3. Exactly-three-color graph coloring, including requiring all three colors.
4. A clearly specified JSON parser subset.

Defer minimum-node-deletion DP with child promotion, IP longest-prefix routing, and advanced concurrency until the core is solid. Other historical OA, SQL, and DP questions are not automatically part of the main screen preparation.

Coverage distinctions: whole-subtree deletion is not child promotion; bipartition is not exactly-three-coloring; Basic Calculator is not JSON or the nested instruction grammar. Most major NG topics are covered by `ref/`, but it is not exhaustive or freshly audited.

## Problem 1 — grid shortest path

Sources: prep Q32/C09 NG round 1, J10351; [reference](ref/grid-shortest-path.cpp); [user code](shortest-path.cpp).

Contract: rectangular 0/1 grid, 0 open, 1 blocked; supplied in-bounds start/target; four directions; distance counts moves, not visited cells; return -1 for blocked endpoints/unreachable/empty grid. No requirement to validate arbitrary coordinates or ragged rows under this contract.

Algorithm: BFS with `dist` initialized to -1; start distance 0; mark visited when enqueuing; return distance when target is popped. First discovery is shortest because all edges cost one. Ordinary DFS does not guarantee shortest distance.

Target grid `{{0,0,0},{1,1,0},{0,0,0}}`, start `(0,0)`, target `(2,0)`, answer 6. Path: `(0,0),(0,1),(0,2),(1,2),(2,2),(2,1),(2,0)`.

Important tests:

```cpp
assert(shortestPath({{0,0,0},{1,1,0},{0,0,0}}, 0,0,2,0) == 6); // Detour.
assert(shortestPath({{0}}, 0,0,0,0) == 0); // Same endpoint.
assert(shortestPath({{0,1},{1,0}}, 0,0,1,1) == -1); // Diagonals forbidden.
assert(shortestPath({{1,0}}, 0,0,0,1) == -1); // Blocked start.
assert(shortestPath({{0,1}}, 0,0,0,1) == -1); // Blocked target.
assert(shortestPath({}, 0,0,0,0) == -1); // Empty grid.
assert(shortestPath({{}}, 0,0,0,0) == -1); // Empty row.
```

### Follow-up: return the actual path

Preferred: store parent coordinates on first enqueue; the parent array also serves as visited. Initialize each to `{-1,-1}` and `parent[sr][sc] = {sr,sc}`. On discovery assign `parent[nr][nc] = {r,c}`. Once the target is reached:

```cpp
vector<pair<int,int>> path;
for (auto p = target; p != start; p = parent[p.first][p.second]) {
    path.push_back(p);
}
path.push_back(start);
reverse(path.begin(), path.end());
return path;
```

Return type becomes `vector<pair<int,int>>`; failure returns `{}`. Same endpoint returns one cell. Nonempty path has `path.size()-1` moves. Do not overwrite a previously assigned parent. For multiple valid shortest paths, test endpoints, adjacency, open cells, and minimal length rather than one exact sequence. The detour test above has a unique path.

Alternative requested by the user: carry `vector<pair<int,int>> path` in every queue state.

```cpp
struct State {
    int r, c;
    vector<pair<int,int>> path;
};
// Start: q.push({sr, sc, {{sr, sc}}});
// Pop: State cur = std::move(q.front()); q.pop();
auto nextPath = cur.path;
nextPath.push_back({nr, nc});
q.push({nr, nc, std::move(nextPath)});
```

User asked whether updating `cur.path` then moving works. It cannot be moved into every neighbor because later neighbors still need the original path. Safe alternative:

```cpp
cur.path.push_back({nr, nc});
q.push({nr, nc, cur.path}); // Copy, not move.
cur.path.pop_back();       // Restore for the next neighbor.
```

Moving is possible for the last valid neighbor only, but bookkeeping is usually not worthwhile. Copies make this slower than parent pointers.

### Other grid extensions

- Eight directions: add diagonal dr/dc pairs; clarify corner-cutting. Counting cells instead of moves changes initial distance to 1.
- Multiple starts: enqueue all starts at distance 0.
- Nonuniform costs: 0–1 BFS for costs 0/1, Dijkstra for arbitrary nonnegative costs.
- Bidirectional BFS can reduce explored cells but does not improve the worst-case linear graph bound.
- Obstacle elimination was introduced as an EXTRA practice extension, not confirmed from the report. Track `(r,c,remaining)` and `seen[r][c][remaining]`; a cell alone is insufficient state. Assume open endpoints, binary cells, valid coordinates, k >= 0. Freeze queue layers to count moves. On entering a neighbor, compute `remaining = cur.remaining - grid[nr][nc]`; skip negative or already-seen states.
- Obstacle tests: `{{0,1,0},{0,1,0},{0,0,0}}` from `(0,0)` to `(0,2)` gives 2 with k=1 and 6 with k=0; `{{0,1,1,0}}` gives -1 with k=1 and 3 with k=2; single open endpoint gives 0.

## Problem 2 — array distances

Sources: Q19/A11/C09 NG round 2 (original report ambiguous about exact output), J10352 explicitly per-1 nearest distance; [reference](ref/array-nearest-distance.cpp); [user code](array-nearest-distance.cpp).

Contract: array of 0/1/2; return nearest-2 distance for each 1 in its original order; distance is absolute index difference; zeros do not block; one 2 can serve multiple 1s; no 2 means -1 per 1; no 1 means empty output. Clarify per-1 minimum versus global closest pair versus all pairs before coding.

Algorithm: left-to-right track latest 2 and store left distances; right-to-left track next 2 and improve distances; collect entries corresponding to 1s. The nearest 2 lies on one of these two sides.

```cpp
assert((nearestTwo({1,0,2,0,1,0,0,2,1}) == vector<int>{2,2,1})); // Main target.
assert((nearestTwo({1,0,2,0,1,1}) == vector<int>{2,2,3})); // Both sides represented.
assert((nearestTwo({2,0,0,1,2}) == vector<int>{1})); // Right improves left.
assert((nearestTwo({2,0,1,0,2}) == vector<int>{2})); // Tie.
assert((nearestTwo({1,1,2}) == vector<int>{2,1})); // Shared target.
assert((nearestTwo({1,0,1}) == vector<int>{-1,-1})); // Missing 2.
assert(nearestTwo({2,0,2}).empty()); // Missing 1.
assert(nearestTwo({}).empty()); // Empty input.
```

Extra parentheses protect `assert` from initializer-list commas.

Follow-up: global closest 1/2 distance. Maintain `last1`, `last2`, `best`, initialized -1. At each 1, compare against latest 2; at each 2, compare against latest 1. Keep the smallest valid difference. Missing either class returns -1.

```cpp
assert(closestPair({1,0,0,2,0,1}) == 2); // Later pair improves answer.
assert(closestPair({1,1,0,2}) == 2); // Latest 1 matters.
assert(closestPair({2,0,1}) == 2); // Reverse encounter order.
assert(closestPair({1,2}) == 1); // Adjacent.
assert(closestPair({1,0,1}) == -1); // Missing 2.
assert(closestPair({2,0,2}) == -1); // Missing 1.
assert(closestPair({}) == -1); // Empty.
```

One earlier chat response accidentally included `closestPair({}). == -1`; it immediately corrected that typo. Use only the valid assertion above.

Other variants: nearest index requires a tie policy; 2D four-direction unit-cost grids use multi-source BFS from all 2s; streaming cannot generally finalize an answer immediately because a future 2 may be closer.

## Problem 3 — parallel courses

Sources: J10557 NG, LC1136; duration extension LC2050; [reference](ref/parallel-courses.cpp); [user code](parallel-courses.cpp).

Contract: courses 1..n; `[a,b]` means a prerequisite of b; one semester each; unlimited concurrent courses; cycles return -1. Convert to zero-based graph indices.

Algorithm: Kahn BFS with indegrees; freeze queue size for each semester; newly released courses wait until the next layer. Count processed courses and reject if fewer than n. With unlimited parallelism, delaying an available course cannot improve the result.

```cpp
assert(minSemesters(3, {{1,3},{2,3}}) == 2); // Take 1,2 then 3.
assert(minSemesters(4, {{1,2},{2,3}}) == 3); // Disconnected course 4.
assert(minSemesters(3, {}) == 1); // All concurrent.
assert(minSemesters(3, {{1,2},{2,1}}) == -1); // Free node must not hide a cycle.
assert(minSemesters(1, {}) == 1); // Single course.
assert(minSemesters(0, {}) == 0); // If empty input allowed.
```

User questioned whether the disconnected-course test should return 2: initial queue indeed contains courses 1 and 4, but semesters are `{1,4}`, `{2}`, `{3}`, so answer is 3.

Follow-up: durations. Keep graph/indegrees/initial queue; use `vector<long long> start(n,0)`; when popping cur compute `finish = start[cur] + duration[cur]`, update global maximum, then `start[next] = max(start[next], finish)` for every child before decrementing indegree. Return total when taken==n, otherwise -1. Function returns `long long`; duration is zero-indexed. Target relations `{{1,3},{2,3}}`, durations `{3,2,5}`, answer 8. This is a DAG longest-path recurrence and is an extension after the no-DP main track.

Other variants: collect each frozen layer to return semester groups; append popped courses for a valid total order. At most k courses per semester is a different optimization problem: arbitrary greedy selection is not guaranteed optimal.

## Problem 4 — DAG privilege inheritance

Sources: J9549/J12349 NG labels, Q04/Q05 related variants; [reference](ref/dag-privilege-inheritance.cpp); [user code](dag-privilege.cpp).

Contract: roles 0..n-1, edge `[u,v]` means v inherits u; own privileges plus all ancestor privileges; deduplicate; base output lexicographically sorted; return empty on cycle; no deny semantics in base version.

Algorithm: construct children/parents and indegrees. A growing `vector<int> order` serves as queue and topo order; its loop condition must re-read `order.size()`. Reject if order.size()!=n. In topo order union each parent's effective set plus own into `set<string> effective[cur]`; convert sets to output vectors in role-ID order.

Main target: own `{{"read"},{"write"},{"read"},{"run"}}`, grants `{{0,1},{0,2},{1,3},{2,3}}`; output `{{"read"},{"read","write"},{"read"},{"read","run","write"}}`. Diamond paths must not duplicate `read`.

Other important tests: repeated local privileges collapse; disconnected roles keep their own privileges; cycle returns empty. `std::set` gives lexicographic order, not first-insertion order.

### Ordered inheritance follow-up

Merge parents in GRANT INPUT order, then local privileges in local input order; keep first occurrence. `parents[v]` must retain grant order; do not use topo-processing order as the merge order. After topo/cycle check, replace set processing and output conversion with:

```cpp
vector<vector<string>> effective(n);
for (int cur : order) {
    unordered_set<string> seen;
    for (int parent : parents[cur]) {
        for (const string& privilege : effective[parent]) {
            if (seen.insert(privilege).second)
                effective[cur].push_back(privilege);
        }
    }
    for (const string& privilege : own[cur]) {
        if (seen.insert(privilege).second)
            effective[cur].push_back(privilege);
    }
}
return effective;
```

Add `<unordered_set>`. Critical test: own `{{"read"},{"write"},{"read","run"}}`, grants `{{1,2},{0,2}}`, role 2 expected `{"write","read","run"}` even if topo order processes role 0 first.

Deny rules require clarification about propagation and whether descendants may re-grant. Effective sets alone may not preserve enough information for all possible deny policies.

## Complexity summary

| Solution | Time | Extra space |
|---|---|---|
| Grid shortest distance | O(V+E) | O(V) |
| Grid path via parent pointers | O(V+E) | O(V), including path |
| Grid full path per queue entry | O(V²) on this bounded-degree grid | O(V²), conservative upper bound |
| Grid with up to k eliminations | O((V+E)(k+1)) | O(V(k+1)) |
| Nearest 2 per 1 | O(n) | O(n) |
| Global closest 1/2 pair | O(n) | O(1) |
| Parallel courses, semesters | O(V+E) | O(V+E) |
| Parallel courses, durations | O(V+E) | O(V+E) |
| Privilege union, sorted sets | O(V+E+(A+EP)log(P+1)+VP) | O(V+E+VP) |
| Ordered privilege union, hash dedup | Expected O(V+E+A+EP) | O(V+E+VP) |

For grids, V=rows*columns and E=O(V) for fixed four-direction adjacency. For courses/roles, V counts nodes and E counts prerequisite/grant edges. P counts all distinct privileges, A counts total entries in own. Privilege bounds treat string comparisons/hashing as constant cost; real long-string costs are additional. Input storage is excluded; explicitly included output storage as above. General graph path-copy time can depend on E as well; the V² statement is for the grid.

## Review history and actual validation

Usual flags: `g++ -std=c++17 -Wall -Wextra`. Earlier syntax checks used `-fsyntax-only`; some limited diagnostics with `-fmax-errors`. Do not use `-DNDEBUG`, which removes assertions.

The assistant used Python's `tempfile.TemporaryDirectory(prefix='snowflake-review-')` to copy only source before the first `// follow-up:` marker into a temporary `.cpp`, compile it to a temporary executable, run it only if compilation succeeded, then clean up. This is the user-approved workflow. Do not compile appended note snippets as complete translation units and report their intentional missing wrapper repeatedly. Do not silently fix a temporary copy and claim the original passed.

| File | Last executed validation | Historical findings | Observation when saving this file |
|---|---|---|---|
| `shortest-path.cpp` | Extracted base compiled, target passed, exit 0 | dr/dc originally missing `[]`; later path snippet used `dr[c]` for column and missed struct semicolon | Base unchanged in logic; column expression now `dc[d]`; path snippet still lacks `;` after `struct State` |
| `array-nearest-distance.cpp` | Extracted base compiled, target passed, exit 0 | Base and closest-pair snippet logically correct; initial whole-file compile rejected loose snippet | No new algorithm error observed |
| `parallel-courses.cpp` | Extracted base compilation failed at `using namespace std:`; target not run | Colon instead of semicolon; algorithm and duration snippet looked correct | Now reads `using namespace std;`; not recompiled after this fix |
| `dag-privilege.cpp` | Base compilation failed; target not run | `chilren` declaration, undeclared `graph[cur]`, `count` instead of `cout` | All three now corrected to children/children[cur]/cout; not recompiled after fixes |

The misspelling `resolvePriviledges` is used consistently in declaration and call, so it is not a functional error. Do not flag it as a compile issue.

Only main target assertions execute in the user's current files; additional cases remain comments. Do not claim those extra cases passed. No follow-up snippet has been compiled as an integrated function during this session. Chat-generated teaching implementations and additional tests have generally not been compiled. Do not inherit the handout's historical sanitizer claims as validation of current user code; this session did not run sanitizers or disable leak checking.

## Working boundaries and persistence

The user supplied a detailed Agent Working Rules baseline in the conversation. Important continuing boundaries:

- No agent-directed Git access at all, including read-only commands, APIs, libraries, or inspection of `.git` internals. Only the baseline's narrow incidental read-only build-metadata exception exists. No Git operations were used for this work.
- C++ implementation is human-owned by default. This task is teaching/review; preserve user edits and do not apply source fixes without explicit scope authorization.
- Protect dependencies, contracts, secrets, environment, and external services. No downloads/toolchain changes or destructive cleanup outside authorization.
- Nontrivial repository implementation requires an accepted persisted design and task state; agreement with an algorithm explanation does not authorize editing C++ implementations. No repository implementation design or implementation stage was approved here.
- Preserve evidence and do not weaken tests or bypass the intended execution path. Stop affected speculative work after two materially different failed approaches or other baseline stop conditions.
- Future nontrivial implementation work should load the complete user baseline from its human-designated persisted location; this condensed handoff is not a replacement or permission to relax it. No local baseline/AGENTS.md was found in the initial ancestor inspection.

This document was explicitly requested by the user as a Markdown file in the current directory. It is the canonical preparation-session handoff; reuse it rather than creating parallel progress notes. It does not duplicate the full reference handout's implementation corpus.

Persistence checks: working-tree file listing found no `.gitignore` in this workspace; explicit checks for `.gitignore` in `/home/benny`, `Code`, `300blows`, `company`, and this workspace found none. No tracking, global exclusions, or Git metadata were inspected; cross-machine sharing is not verified. The external Downloads guide must be supplied separately on another machine. Relative source links assume the current workspace files are carried along.

Agent-authored repository changes for this preservation request: `PREP_SESSION.md` only. User-authored C++ and reference files were read, not edited. Scope of observation: listed reference inventory and the four root solution files; no claim of repository cleanliness or pre-session authorship beyond conversational evidence.

Experimental artifact disposition: review temporary directories were automatically removed after tests. Historical concrete paths visible in logs included `/tmp/snowflake-review-v5jmn6ie` and `/tmp/snowflake-review-gukdo6xi`; no pending temporary artifacts are known. No production experiments were created. Pending deletion: none.

## C++17 interview quick reference and caveats

Added at the user's request on 2026-10-09. These are small reference fragments, not one complete program; they assume the stated headers and `using namespace std;`. They have not been compiled as a combined test suite. Prefer familiar, explicit syntax over compact tricks.

### Heaps: max, min, pairs, custom priority

```cpp
#include <queue>
#include <vector>
#include <functional> // greater
#include <utility>    // pair

priority_queue<int> maxHeap; // Largest value first; default is less<int>.
maxHeap.push(3);
maxHeap.push(8);
int largest = maxHeap.top(); // 8; top() does not remove it.
maxHeap.pop();               // Removes 8; returns void.

priority_queue<int, vector<int>, greater<int>> minHeap;
minHeap.push(3);
minHeap.push(8);
int smallest = minHeap.top(); // 3.

// Pairs compare lexicographically: first field, then second field.
priority_queue<pair<int, int>> tasks; // Highest priority, then largest ID.
tasks.push({10, 3});
tasks.push({10, 7}); // (10,7) comes first.

using Entry = pair<long long, int>;
priority_queue<Entry, vector<Entry>, greater<Entry>> distances;
distances.push({5, 2}); // Lowest distance, then smallest node ID.

// Heapify existing values in O(n); repeated push is O(n log n).
vector<int> values = {3, 8, 1};
priority_queue<int> built(values.begin(), values.end());
```

Custom rule: highest priority first, then earliest timestamp, then smallest ID:

```cpp
struct Task {
    int priority;
    long long timestamp;
    int id;
}; // Remember the semicolon.

struct LowerPriority {
    bool operator()(const Task& a, const Task& b) const {
        if (a.priority != b.priority) return a.priority < b.priority;
        if (a.timestamp != b.timestamp) return a.timestamp > b.timestamp;
        return a.id > b.id;
    }
};

priority_queue<Task, vector<Task>, LowerPriority> heap;
heap.push({10, 100, 7});
heap.push({10, 90, 3}); // Earlier timestamp: this comes first.
```

- For `priority_queue`, `comp(a,b) == true` means a has LOWER priority than b. For `sort`, true means a should come BEFORE b. Remember this distinction.
- Comparators must define a strict weak ordering: never use `<=` or `>=`; equal objects must compare false in both directions.
- `top()` is O(1), push/pop O(log n), storage O(n). Check `empty()` before top/pop.
- Copy the top before popping: `auto item = heap.top(); heap.pop();`. A reference to top must not be used after pop. `top()` gives a const reference, so `std::move(heap.top())` generally copies ordinary value types rather than moving them.
- Equal-priority entries are not automatically FIFO. Add a sequence number if stable insertion order is required.
- `priority_queue` has no arbitrary erase or decrease-key. For mutable tasks, use a current-state map plus stale-entry checks (sometimes requiring a version), or an ordered set with erase/reinsert.
- Do not let a comparator read mutable external priority values. Changing those values does not reheapify the queue or reorder a set.
- Negating a key to reverse ordering can overflow at the minimum signed value. A comparator avoids that trap.

### Queue, stack, structured bindings, and copies

```cpp
queue<pair<int, int>> q;
q.push({0, 1});
auto [r, c] = q.front(); // C++17, copies coordinates.
q.pop();                // Returns void.

// C++11-compatible alternative:
// auto cell = q.front();
// q.pop();
// int r = cell.first, c = cell.second;

vector<int> stack;
stack.push_back(5);
int cur = stack.back();
stack.pop_back();
```

- Never bind `auto& [r,c] = q.front()` and then use r/c after `q.pop()`; they referred to the removed element.
- `for (auto x : items)` copies; `for (const auto& x : items)` reads without copying; `for (auto& x : items)` can modify the original.
- `const vector<int>&` parameters avoid copying read-only inputs. Passing a whole grid or adjacency list by value on each recursive call can dramatically increase time/space.
- `std::move` enables moving; it does not guarantee that a move occurs. After moving from an STL container, do not rely on its previous contents or assume it is empty. It remains valid for operations whose preconditions are satisfied.
- BFS: mark seen on enqueue, not dequeue. For layers, capture `int size = q.size()` before the inner loop.

### Hash maps, ordered maps, and sets

```cpp
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

unordered_map<string, int> count;
++count["read"]; // Intentionally inserts 0 if missing, then increments.

auto it = count.find("write");
if (it != count.end()) {
    int value = it->second; // Read without inserting.
    (void)value;
}

unordered_set<int> seen;
if (seen.insert(7).second) { /* First insertion only. */ }
bool existed = seen.erase(7) > 0;

multiset<int> endings = {5, 5, 9};
auto pos = endings.find(5);
if (pos != endings.end()) endings.erase(pos); // Removes ONE 5.
// endings.erase(5);                         // Removes ALL 5s.

for (auto it = count.begin(); it != count.end();) {
    if (it->second == 0) it = count.erase(it);
    else ++it;
}
```

- `map[key]` and `unordered_map[key]` INSERT missing keys with default values. Use `find` for non-mutating lookup; `at` throws on missing keys.
- `contains` is C++20; use `find` or `count` in C++17.
- `map`/`set` iterate in comparator order, not insertion order. Unordered containers provide no sorted or insertion order guarantee.
- Hash lookup/insert/erase is average O(1), worst-case O(n). Ordered map/set lookup and insertion are O(log n). String hashing/comparison adds string-length costs.
- `emplace(key,value)` does not overwrite an existing map value; assignment through `[]` does. This matters when preserving the earliest undo-log value.
- Inserting into an unordered container may rehash and invalidate iterators. Avoid holding an iterator across such insertions; references/pointers to existing elements survive rehash but not erasure of their element.
- There is no standard default `std::hash<pair<int,int>>` for C++17. For grids, use a 2D visited array; otherwise provide a pair hash or use `set<pair<int,int>>`.

### Vectors, bounds, and lifetime

```cpp
vector<int> a(5, -1);     // Five elements, all -1.
vector<int> b{5, -1};     // TWO elements: 5 and -1.
vector<vector<int>> dist(rows, vector<int>(cols, -1));

vector<int> output;
output.reserve(100);     // Capacity only; size remains 0.
output.push_back(7);     // Correct; output[0] before this would be invalid.

int n = static_cast<int>(a.size()); // Assuming size fits in int.
for (int i = n - 1; i >= 0; --i) { /* Safe when n == 0. */ }

int dr[] = {0, 0, -1, 1};
int dc[] = {1, -1, 0, 0};
// int nr = r + dr[d], nc = c + dc[d]; // d in [0,4).
```

- `size()` is unsigned. Avoid `int i = a.size()-1` for an empty vector; convert to an appropriate signed size before subtracting. Never reverse-loop with unsigned `i >= 0`.
- Check `grid.empty()` before `grid[0]`; rectangularity and coordinate validity remain separate preconditions.
- `operator[]` does not check bounds. `at()` does and throws on out-of-range access.
- Vector reallocation invalidates references, pointers, and iterators into it. In a trie/node pool, save indices across `push_back`, then reacquire the element by index.
- Erasing from a vector shifts later elements and invalidates iterators/references at or after the erase position. Do not erase nodes while still interpreting old parent indices.
- `vector<bool>` uses proxy elements, not ordinary `bool&`. It is fine for basic visited indexing; use `vector<char>` when ordinary element-reference behavior is useful.
- Recursive DFS uses O(h) call stack and may overflow for a deep chain. Consider an explicit stack if depth can be large; there is no portable fixed safe recursion depth.

### Sorting, binary search, and iterators

```cpp
#include <algorithm>
#include <iterator>

sort(a.begin(), a.end()); // Ascending, O(n log n).
sort(a.begin(), a.end(), greater<int>()); // Descending.

sort(a.begin(), a.end()); // Restore ascending order before these searches.
auto lo = lower_bound(a.begin(), a.end(), target); // First >= target.
auto hi = upper_bound(a.begin(), a.end(), target); // First > target.
bool found = lo != a.end() && *lo == target;
int occurrences = static_cast<int>(hi - lo);

map<int, string> versions;
auto after = versions.upper_bound(timestamp);
if (after != versions.begin()) {
    auto latest = prev(after); // Greatest timestamp <= query.
    // Use latest->second.
}
```

- Searches require the range to be ordered/partitioned consistently with their comparator. Do not binary-search an arbitrary unsorted vector.
- On a vector, lower/upper_bound are O(log n). Use `map.lower_bound` / `set.lower_bound` rather than the generic algorithm on tree iterators; generic iterator advancement can be linear.
- Do not dereference `end()`, use `prev(begin())`, or call `back()` on an empty container.
- `sort` is not stable. Use an explicit tie-breaker or `stable_sort` if equal-key input order must be preserved.
- For manual binary search, use `mid = lo + (hi-lo)/2` and maintain a clear closed or half-open interval invariant.
- Pair and tuple comparisons are lexicographic, which is convenient only when that matches the required tie-breaking contract.

### Integers, strings, and assertions

```cpp
#include <limits>
#include <cctype>

long long product = 1LL * x * y; // Widen BEFORE multiplication.
double median = (static_cast<long long>(left) + right) / 2.0;
long long inf = numeric_limits<long long>::max() / 4;

string s = "abc";
bool digit = isdigit(static_cast<unsigned char>(s[0]));
auto separator = s.find('#');
if (separator != string::npos) { /* Separator exists. */ }

vector<int> got = {1, 2};
assert((got == vector<int>{1, 2})); // Parentheses protect initializer commas.
```

- Signed overflow is undefined behavior. `long long result = x*y` does not prevent an int multiplication from overflowing first. Even long long needs bounds appropriate to the contract.
- C++ integer division truncates toward zero: `-3/2 == -1`, not -2. If floor division is required and denominator is positive, use `q=a/b; if (a%b<0) --q;`.
- Avoid `abs(INT_MIN)` in int and avoid overflowing subtraction before taking an absolute difference; widen first when needed.
- An infinity sentinel is not permission to add arbitrarily large values. Check reachability and ensure additions stay representable.
- `string::find` returns an unsigned position or `string::npos`; do not store it in int. `substr(start,length)` takes a length, not an ending index, and creates a copy.
- Character-classification functions require EOF or an unsigned-char value converted to int. For strictly ASCII alphanumeric rules, explicit character ranges avoid locale-dependent semantics.
- `assert` disappears with `-DNDEBUG`; do not use that flag for interview tests. Do not place production-required side effects only inside assertions.
- Use standard headers explicitly. `<bits/stdc++.h>` is a compiler-specific convenience, not a standard C++ header.

### Thirty-second compile checklist

1. `using namespace std;`, not a colon; `struct ... };`, not just a closing brace.
2. Exact identifier spelling: children/graph, cout/count, consistent function names.
3. Arrays need `[]`; row uses `dr[d]`, column uses `dc[d]`.
4. Check empty containers before front/back/top; copy values before removing elements.
5. Initialize counters/sentinels and mark visited at discovery.
6. Confirm index base, edge direction, tie rules, and interval endpoints.
7. Compile the base and run the target assertion early. Review loose follow-up notes separately as agreed.

## Next step

If the user says `check`, inspect their latest relevant source and run its base target test via the temporary-copy workflow; report errors only. The last reviewed problem is DAG privilege inheritance, whose previously reported compiler typos are now visibly fixed but untested.

If the user says `next`, present **problem 5/49: parent-array forest deletion and reindexing**, after reading `ref/forest-delete-node.cpp`. Maintain the accepted main-code / compact-follow-up / commented-tests / complexity format. Do not restart the prep sequence or claim all four presented groups are validated complete.
