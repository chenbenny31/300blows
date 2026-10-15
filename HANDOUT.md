# 300blows Handout

Everything needed to walk into a live coding round: the C++ style, the round
workflow, test templates, pitfalls that actually bit during prep, pattern skeletons,
and a map of this repo. Code style here is the **interview style** (fast to type,
one runnable file); the per-technique folders keep the LeetCode `class Solution`
style described in `README.md`.

---

## 1. Repo map — what to open when

| Folder | What it is | Use it for |
|---|---|---|
| `arrays-hashing/`, `two-pointers/`, `sliding-window/`, `stack/` | One file per LC number, `class Solution` style | Technique drills |
| `specialty/` | 50 one-trick problems; every file has `// Trick:` and `// Recognize:` | Read the README trick column until automatic |
| `company/snowflake/ng/` | Your own 12 core implementations (fixed) | Tier 1 — highest priority |
| `company/snowflake/ng/important/` | 13–20 (Tier 2) + 44–59 (gap fill from the handout) | Tier 2 |
| `company/snowflake/ng/optional/` | 21–43 | Tier 3/4 |
| `company/snowflake/ng/word-pair-within-k.cpp`, `command-interpreter.cpp` | Questions actually asked in your interviews | Re-do cold |
| `company/apple/{ng,important,optional}` | hack2hire Apple + LC Apple list (01–66) | Apple prep |
| `company/bytedance/{ng,important,optional}` | hack2hire ByteDance + LC ByteDance list (01–92) | ByteDance prep |
| `company/*/README.md` | Tier tables with sources | Pick what to do next |

Order of attack for any company: `ng/` cold (timer on) → `specialty/` tricks you
missed → `important/` → `optional/` only if time remains.

---

## 2. The round, minute by minute (45–60 min)

1. **Restate + clarify (2–4 min).** Say the input/output contract back. Ask about:
   empty input, duplicates, negatives/overflow, ties and ordering of output, sorted?,
   1-indexed?, invalid input behavior, size limits (n ≤ ? decides O(n²) vs O(n log n)).
   For spec-style questions (interpreters, KV stores) also ask: nesting? recursion?
   undefined names? what happens on repeated keys / redefinition?
2. **Approach out loud (3–5 min).** Brute force first in one sentence, then the plan.
   State the data structures *with what they store* and the complexity before coding.
3. **Skeleton + compile (2 min).** Includes, `using`, signature returning a dummy,
   `test` + `main` with the given examples. Compile now — typos surface while nothing
   else can be wrong.
4. **Implement (15–25 min).** Narrate the invariant, not the syntax.
5. **Run, fix one failure at a time (5 min).** Re-run between fixes.
6. **Add your own edge test** — the one that targets the code you trust least
   (SnowCal lesson: a multi-line function body would have exposed the off-by-one).
7. **Complexity + follow-ups (remaining).** Write follow-ups as a separate function.

Writing-order rule of thumb: if you can't write the main loop's invariant in one
comment line, you don't understand the solution yet — stop and think.

---

## 3. C++ interview style (the house rules)

### File skeleton
```cpp
// LC 207 Course Schedule: n courses, prereq [a, b] = b before a; can all finish?
// Follow-up: return one valid order (LC 210).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's BFS on indegrees; leftover nodes mean a cycle. T: O(V+E), S: O(V+E).
bool canFinish(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n);   // graph[b] = courses unlocked by b
    vector<int> indeg(n, 0);        // indeg[a] = prereqs of a not yet taken
    ...
}

void test(const string& label, int n, const vector<vector<int>>& prereqs, bool want) {
    cout << label << (canFinish(n, prereqs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);
    test("cycle", 2, {{1, 0}, {0, 1}}, false);
}
```

### Rules
- **`using std::xxx;` for everything** — types *and* functions (`using std::max;`,
  `using std::sort;`, `using std::priority_queue;`). **No `std::` in the body.**
  Never `using namespace std`, never `bits/stdc++.h`.
- **Include only what you use.**
- **Free functions** for algorithm problems; a **class only for design problems**.
  In a class, **data members first**, then private helpers, then `public:`.
- **Recursion = a separate helper** with explicit parameters. No recursive lambdas,
  no `std::function`.
- **Follow-ups = separate function or class**, no tests for them.
- **Lines ≤ 90 columns.** A trailing comment that doesn't fit moves above the line.
- **Names:** adjacency list is `graph` (or `children`/`parents`) with a comment saying
  what an edge means; traversal variables `cur` / `nxt`. Short, descriptive locals.
  No macros, no `ll`/`vi`/`pii`.
- **Braces on every body**; one-liners are fine: `if (seen[v]) { continue; }`.
- Early `return` / `continue` over flag variables.
- `int n = nums.size();` once. Never `i < v.size() - 1` (wraps on empty).
- Bounded keys → `vector<int> cnt(26, 0)`; otherwise `unordered_map`, read with
  `find` / `count`.
- Stack = `vector` (`push_back/back/pop_back`). Queue = `queue`.
  Min-heap = `priority_queue<int, vector<int>, greater<int>>`. Multi-key → pair/tuple
  and negate a field instead of writing a comparator.
- `long long` once sums/products can pass ~2e9; widen *before* the op: `1LL * a * b`.
- Chars: `c >= '0' && c <= '9'`, `c - 'a'` (not `isdigit`, UB on negative `char`).
- **No `<sstream>`** — split / parse with a plain char loop (template in §6).
- Hand-written linked structures when the design needs one (LRU/LFU DLL with embedded
  sentinels, destructor, deleted copy constructor/assignment).
- Leave out: `reserve`, `noexcept`, `const` member functions, smart pointers,
  error handling beyond the spec.

### Comments — four kinds only
1. Top of file: 1–3 line problem statement (+ `// Follow-up:` lines).
2. Above each function: `// <technique>. T: O(...), S: O(...).` Rough upper bound,
   explainable in one sentence; graphs in **V+E**, grids in **R*C**.
3. Every important data structure: **what it stores**, not how it works
   (`// dist[r][c] = steps from start, -1 = unvisited`).
4. A few decision comments where an order or guard is load-bearing
   (`// push after the w2 check, so a word never pairs with itself`).
Nothing on test lines.

### Tests
- One `void test(const string& label, <inputs>, <want>)` printing only
  `label PASS` / `label FAIL`. Use `want`, not `expected`; no "got X".
- Design problems: `void check(bool ok, const string& label)` + a short call sequence.
- **2–4 cases**, each chosen for maximum coverage, simplest canonical shape,
  domain-meaningful values: diamond `0->1->3, 0->2->3`; permissions `"r"/"w"/"x"`;
  the LC examples; empty; the adversarial case for your mechanism.
- Several valid answers → compare a sorted/canonical form or a validity check.
- Doubles → `fabs(got - want) < 1e-9`.
- Trees/lists → nodes on the stack (`TreeNode a(5), b(1); a.left = &b;` or a
  `vector<ListNode>` linked afterwards). If the algorithm allocates, free in the test.

Compile locally:
```bash
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined f.cpp -o f && ./f
```

---

## 4. C++ pitfalls that actually came up

| Pitfall | Correct form |
|---|---|
| `s.substr(pos, len)` — 2nd arg is a **length** | `s.substr(i + 1, j - i - 1)` for the part between i and j |
| `m[key]` **inserts** a default on miss | read with `m.find(key)` / `m.count(key)` |
| `auto& [a, b] = stk.back(); stk.pop_back();` dangles | copy first: `auto [a, b] = stk.back();` |
| Structured bindings on `vector` don't compile | `int u = e[0], v = e[1];` |
| `vector` push_back invalidates references/iterators | `unordered_map`/`map` node references stay valid on insert |
| `priority_queue` default is a **max-heap** (`less`: `comp(a,b)` true ⇒ a sinks) | min-heap: `greater<T>`; `sort` reads the same comparator the other way |
| Comparator must be strict (`<`, never `<=`) | otherwise UB in `sort` |
| `size()` is unsigned: `v.size() - 1` on empty = huge | `int n = v.size();` then use `n - 1` |
| `int` overflow in `a * b`, prefix sums, mid = `(lo + hi) / 2` | `1LL * a * b`, `long long` sums, `lo + (hi - lo) / 2` |
| `1 << 31` is UB on int | `1u << i`, `1LL << i` |
| `x & -x` on `INT_MIN` overflows | do it on `unsigned` / `long long` |
| No built-in split in C++17 | char loop (§6); `istringstream` / C++20 `views::split` exist but skip in interviews |
| `stoi` throws on non-numeric / out of range | validate first if input is untrusted |
| `static_cast<vector<int>>(set)` doesn't convert | range constructor `vector<int>(s.begin(), s.end())` |
| `std::move` from a `set` element | can't (keys are const) — copy |
| Class owning raw `new` nodes | destructor + `Cls(const Cls&) = delete; Cls& operator=(const Cls&) = delete;` |
| Embedded sentinels `Node head{}, tail{};` | live inside the object (stack if the object is local) — no `new` needed |
| Deep recursion (chain of 1e5) | explicit stack |
| Two loops sharing one index (`i++` in both) | write the invariant: "i points at the next unread line" (SnowCal bug) |

---

## 5. Pattern skeletons

### BFS on a grid (mark on push)
```cpp
// BFS from (sr, sc); dist doubles as visited. T: O(R*C), S: O(R*C).
int shortestPath(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
    int rows = grid.size(), cols = grid[0].size();
    vector<vector<int>> dist(rows, vector<int>(cols, -1));   // -1 = unvisited
    int dr[4] = {1, -1, 0, 0}, dc[4] = {0, 0, 1, -1};
    queue<pair<int, int>> q;
    q.push({sr, sc});
    dist[sr][sc] = 0;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == tr && c == tc) { return dist[r][c]; }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) { continue; }
            if (grid[nr][nc] == 1 || dist[nr][nc] != -1) { continue; }
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return -1;
}
```

### Kahn topological order (cycle ⇔ order.size() < n)
```cpp
// T: O(V+E), S: O(V+E).
vector<int> topoOrder(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);   // graph[u] = nodes that must come after u
    vector<int> indeg(n, 0);        // indeg[v] = unprocessed predecessors of v
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        indeg[e[1]]++;
    }
    queue<int> q;
    for (int v = 0; v < n; v++) {
        if (indeg[v] == 0) { q.push(v); }
    }
    vector<int> order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    // nodes on or behind a cycle never reach indegree 0
    if ((int)order.size() < n) { return {}; }
    return order;
}
```

### DFS 3-color (cycle detection reachable from one node)
```cpp
// state: 0 = unvisited, 1 = on current path, 2 = done. Appends in postorder.
bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state,
         vector<int>& order) {
    if (state[cur] == 2) { return true; }
    if (state[cur] == 1) { return false; }   // back edge = cycle
    state[cur] = 1;
    for (int nxt : graph[cur]) {
        if (!dfs(nxt, graph, state, order)) { return false; }
    }
    state[cur] = 2;
    order.push_back(cur);
    return true;
}
```

### Tree postorder (return a value up, track a global best by reference)
```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// best path through any node; returns best downward chain from cur. T: O(n), S: O(h).
int gain(TreeNode* cur, int& best) {
    if (cur == nullptr) { return 0; }
    int l = max(0, gain(cur->left, best));
    int r = max(0, gain(cur->right, best));
    best = max(best, cur->val + l + r);
    return cur->val + max(l, r);
}
```

### Linear scan with "last seen" state (nearest-two / word pairs)
```cpp
// closest distance between any 1 and any 2; 0 = none yet (distance is never 0).
int closestPair(const vector<int>& nums) {
    int last1 = -1, last2 = -1, best = 0;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (nums[i] == 1) {
            last1 = i;
            if (last2 != -1 && (best == 0 || i - last2 < best)) { best = i - last2; }
        }
        if (nums[i] == 2) {
            last2 = i;
            if (last1 != -1 && (best == 0 || i - last1 < best)) { best = i - last1; }
        }
    }
    return best == 0 ? -1 : best;
}
```

### Binary search — first index where pred is true
```cpp
// half-open [lo, hi); returns n if pred is never true. T: O(log n).
int lowerBound(const vector<int>& a, int target) {
    int lo = 0, hi = a.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] >= target) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}
```
Binary search on the answer: same loop over a value range with a monotone
`feasible(mid)` (410, 875, 2226).

### Monotonic stack — next greater element
```cpp
// decreasing stack of unresolved indices. T: O(n), S: O(n).
vector<int> nextGreater(const vector<int>& a) {
    int n = a.size();
    vector<int> ans(n, -1);
    vector<int> stk;   // indices whose next greater is not found yet
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && a[stk.back()] < a[i]) {
            ans[stk.back()] = a[i];
            stk.pop_back();
        }
        stk.push_back(i);
    }
    return ans;
}
```

### Top-K with a size-k heap (top = worst kept)
```cpp
// T: O(n log k), S: O(k).
vector<int> topK(const vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> heap;   // k largest, min on top
    for (int x : nums) {
        heap.push(x);
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top());
        heap.pop();
    }
    return result;
}
```

### Backtracking (choose / explore / unchoose)
```cpp
void backtrack(const vector<int>& nums, int start, vector<int>& path,
               vector<vector<int>>& result) {
    result.push_back(path);
    for (int i = start; i < (int)nums.size(); i++) {
        // sorted input: skip equal siblings to avoid duplicate subsets
        if (i > start && nums[i] == nums[i - 1]) { continue; }
        path.push_back(nums[i]);
        backtrack(nums, i + 1, path, result);
        path.pop_back();
    }
}
```

### Interpreter / deferred execution (SnowCal family: 224, 394, 736)
```cpp
// funcs is passed down by reference (no globals); FUN stores, INV re-runs.
int exec(const vector<string>& cmds, int result,
         unordered_map<string, vector<string>>& funcs) {
    int n = cmds.size();
    for (int i = 0; i < n; i++) {
        int sp = cmds[i].find(' ');
        string op = cmds[i].substr(0, sp);
        string arg = cmds[i].substr(sp + 1);
        if (op == "FUN") {
            vector<string> body;
            // END closes the most recent FUN, so count depth; i ends on that END
            int depth = 1;
            for (i++; i < n; i++) {
                if (cmds[i].substr(0, 3) == "FUN") { depth++; }
                if (cmds[i] == "END" && --depth == 0) { break; }
                body.push_back(cmds[i]);
            }
            funcs[arg] = body;
        } else if (op == "INV" && funcs.count(arg)) {
            result = exec(funcs[arg], result, funcs);
        } else if (op == "ADD") {
            result += stoi(arg);
        } else if (op == "MUL") {
            result *= stoi(arg);
        }
    }
    return result;
}
```

### LRU with a hand-written DLL (design-class layout)
```cpp
class LRUCache {
    struct Node {
        int key, val;
        Node* prev;
        Node* next;
    };
    int cap;                          // max items kept
    Node head{};                      // sentinel; head.next = most recent
    Node tail{};                      // sentinel; tail.prev = least recent
    unordered_map<int, Node*> nodes;  // key -> its node

    void unlink(Node* x) {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }
    void pushFront(Node* x) {
        x->prev = &head;
        x->next = head.next;
        head.next->prev = x;
        head.next = x;
    }

public:
    LRUCache(int capacity) : cap(capacity) {
        head.next = &tail;
        tail.prev = &head;
    }
    ~LRUCache() {
        for (Node* cur = head.next; cur != &tail;) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    int get(int key) {
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        unlink(it->second);
        pushFront(it->second);
        return it->second->val;
    }
    void put(int key, int value) {
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->val = value;
            unlink(it->second);
            pushFront(it->second);
            return;
        }
        Node* x = new Node{key, value, nullptr, nullptr};
        nodes[key] = x;
        pushFront(x);
        if ((int)nodes.size() > cap) {
            Node* lru = tail.prev;
            unlink(lru);
            nodes.erase(lru->key);
            delete lru;
        }
    }
};
```

---

## 6. Snippets worth memorizing verbatim

### Split on spaces (no sstream)
```cpp
vector<string> splitWords(const string& text) {
    vector<string> words;
    string word;
    for (char c : text) {
        if (c != ' ') {
            word += c;
            continue;
        }
        if (!word.empty()) { words.push_back(word); }
        word.clear();
    }
    if (!word.empty()) { words.push_back(word); }
    return words;
}
```

### Parse an integer by hand
```cpp
// reads digits starting at i, leaves i on the first non-digit
long long readNumber(const string& s, int& i) {
    long long num = 0;
    while (i < (int)s.size() && s[i] >= '0' && s[i] <= '9') {
        num = num * 10 + (s[i] - '0');
        i++;
    }
    return num;
}
```

### Union-find (path compression)
```cpp
struct DSU {
    vector<int> parent;   // parent[x] = x's parent; root when parent[x] == x
    DSU(int n) : parent(n) {
        for (int i = 0; i < n; i++) { parent[i] = i; }
    }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) { return false; }
        parent[a] = b;
        return true;
    }
};
```

### Sweep line for max overlap (half-open intervals)
```cpp
// (t, -1) sorts before (t, +1): a room frees before the next meeting takes it
int maxOverlap(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> events;   // (time, +1 start / -1 end)
    for (const auto& m : intervals) {
        events.push_back({m[0], 1});
        events.push_back({m[1], -1});
    }
    sort(events.begin(), events.end());
    int cur = 0, best = 0;
    for (const auto& [time, delta] : events) {
        cur += delta;
        best = max(best, cur);
    }
    return best;
}
```

---

## 7. Complexity cheat sheet

| n | Fits |
|---|---|
| ≤ 12 | O(n!) permutations |
| ≤ 20–25 | O(2^n) subsets / bitmask DP |
| ≤ 500 | O(n³) |
| ≤ 5·10³ | O(n²) |
| ≤ 10⁵–10⁶ | O(n log n) or O(n) |
| ≥ 10⁹ (values) | binary search on the answer, math, O(log n) |

Say complexity as an upper bound with a one-line reason: "each node is pushed and
popped once, each edge relaxed once → O(V+E)".

---

## 8. Lessons from the real rounds

- **Word pairs within k (Snowflake, passed).** A `last1/last2` scan generalizes to a
  deque of pending indices pruned lazily. Push *after* the w2 check so a word never
  pairs with itself. Follow-up: many queries → precompute word → positions, then
  two pointers.
- **SnowCal interpreter (Snowflake).** Core idea: FUN stores the body, INV re-runs it
  recursively on the current value; the function table is passed by reference.
  What cost time: picking the representation (a lambda would work but is harder in
  C++; a `vector<string>` body is the simple version). What cost correctness: two
  loops advancing one index (extra `i++` skipped the first body line). Missed hint:
  "END closes the most recent function" = nesting → depth counter.
- **Interviewer silence while you're stuck is them deciding whether to hint** — not a
  verdict. Recovering fast after a hint reads as "needed a nudge, then executed well".
- **Before coding spec questions:** list 3–4 clarifications as comments, then write
  the chosen design in one line.
- **Always add one test the prompt didn't give**, aimed at your least-trusted branch.

---

## 9. AI-assisted rounds

When a round lets (or requires) you to use an AI assistant, the grade is judgment:
1. **Decompose first** — write the plan and the interfaces yourself, then delegate
   small pieces.
2. **Read every line it returns.** Look for the classic slips: off-by-one, missing
   empty/edge case, `operator[]` inserting, unsigned underflow, wrong comparator
   direction, global state leaking between calls.
3. **Verify with tests you wrote**, not tests it wrote for its own code.
4. **Say what you rejected and why** ("it used a global map; I pass it by
   reference so tests are independent").
5. Keep ownership of the design — the assistant writes code, you decide structure.
