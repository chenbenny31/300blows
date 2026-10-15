# Specialty — one-trick problems

Problems whose solution hinges on a single non-obvious insight that is hard to derive cold
in an interview. The goal is recognition: read the trick column until each one is automatic,
then type the file once from memory.

Every file is a runnable C++17 interview-style solution (`g++ -std=c++17 -Wall -Wextra`,
prints `label PASS` per test). Line 1 states the problem, the `// Trick:` line is the insight,
`// Recognize:` says when to reach for it. Follow-ups are separate functions without tests.

## Monotonic stack / deque

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `84.cpp` | Largest Rectangle in Histogram | increasing stack; a popped bar spans (new top, i); sentinel 0 flushes the end | "nearest smaller bar on both sides" for every element. |
| `739.cpp` | Daily Temperatures | decreasing stack of unresolved days; a warmer day resolves every colder top | "next greater element" for every index. |
| `402.cpp` | Remove K Digits | increasing stack; pop a larger digit before a smaller one while k > 0 | smallest number/subsequence after removals -> greedy monotonic stack. |
| `316.cpp` | Remove Duplicate Letters | increasing stack; pop a larger top only if it appears again later (last index) | smallest subsequence using each distinct char once (same as LC 1081). |
| `32.cpp` | Longest Valid Parentheses | index stack seeded with -1 as the run's base; on ')' pop, then len = i - top | longest balanced substring (not subsequence) of brackets. |
| `239.cpp` | Sliding Window Maximum | decreasing deque of indices; front = window max, drop expired index from front | max/min of every fixed-size window. |
| `42.cpp` | Trapping Rain Water | two pointers; the lower side's running max bounds its water | water at i = min(max on left, max on right) - height[i]. |

## Index as hash / cycle detection

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `41.cpp` | First Missing Positive | cyclic sort; index as hash: put value v in slot v-1, then find the first misfit | O(1) space and only values 1..n matter -> use the array itself as the hash. |
| `448.cpp` | Find All Numbers Disappeared in an Array | sign marking; value v negates nums[v-1]; slots still positive are missing | values in 1..n + O(1) space -> the sign of nums[v-1] is a "seen v" flag. |
| `287.cpp` | Find the Duplicate Number | i -> nums[i] is a linked list; the duplicate is its cycle entry (Floyd) | values in 1..n usable as indices + read-only array + O(1) space. |
| `142.cpp` | Linked List Cycle II | Floyd; after meeting, step one from head and one from the meet: they hit entry | find a cycle's start in O(1) space (also LC 287). |
| `160.cpp` | Intersection of Two Linked Lists | two pointers jump to the other list's head at null; both walk a+c+b and meet | align two paths of different lengths without measuring them. |

## One-pass array tricks

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `31.cpp` | Next Permutation | rightmost dip a[i] < a[i+1]; swap a[i] with rightmost bigger; reverse suffix | "next greater arrangement" of a sequence, in place. |
| `169.cpp` | Majority Element | Boyer-Moore voting: a different element cancels one vote of the candidate | "more than n/k times" in O(1) space -> keep k-1 candidates. |
| `134.cpp` | Gas Station | if the tank goes negative at i, no start in [start, i] works; restart at i+1 | circular route with gains/costs and one valid start. |
| `238.cpp` | Product of Array Except Self | answer[i] = (product left of i) * (product right of i); two passes | "all except self" without division -> prefix * suffix. |
| `75.cpp` | Sort Colors | Dutch flag lo/i/hi; after swapping with hi, don't advance i (unchecked value) | 3-way partition around a pivot value in one pass. |
| `189.cpp` | Rotate Array | reverse all, then reverse the first k and the remaining n-k separately | in-place rotation/cyclic shift with O(1) extra space. |
| `48.cpp` | Rotate Image | clockwise = transpose, then reverse each row | in-place matrix rotation -> compose two reflections. |
| `11.cpp` | Container With Most Water | two pointers from both ends; always move the shorter side inward | maximize width * min(pair) over all pairs in O(n). |
| `135.cpp` | Candy | two passes: left-to-right for rising slopes, right-to-left taking the max | each element constrained by both neighbors -> one pass per direction. |

## Prefix sums and binary search

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `528.cpp` | Random Pick with Weight | prefix sums tile [1, total]; draw r, binary search the first prefix >= r | weighted random choice -> prefix sums + binary search. |
| `560.cpp` | Subarray Sum Equals K | sum(j+1..i) = prefix[i] - prefix[j]; count earlier prefixes == prefix - k | subarray sum target with negatives (sliding window breaks). |
| `974.cpp` | Subarray Sums Divisible by K | equal prefix remainders bound a divisible subarray; normalize negative % | "subarray sum divisible by / multiple of k" -> count prefix remainders. |
| `128.cpp` | Longest Consecutive Sequence | hash set; only start counting at x with no x-1, so each run is walked once | O(n) on unsorted data where sorting would be the obvious answer. |
| `410.cpp` | Split Array Largest Sum | binary search the answer in [max, sum]; greedily count pieces with sum <= mid | "minimize the maximum" + a monotone feasibility check |
| `4.cpp` | Median of Two Sorted Arrays | binary search how many of the shorter array go left so maxLeft <= minRight | k-th / median of two sorted arrays in log time -> binary search a partition |

## Randomization and O(1) design

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `380.cpp` | Insert Delete GetRandom O(1) | vector + val->index map; remove = move the last element into the hole, pop back | O(1) delete AND O(1) uniform random -> dense array plus index hash map |
| `382.cpp` | Linked List Random Node | reservoir sampling - replace the pick with the i-th node with probability 1/i | uniform pick from a stream / list whose length is unknown, O(1) space |
| `384.cpp` | Shuffle an Array | Fisher-Yates - for i from n-1 down, swap a[i] with a[rand() % (i+1)] | uniform random permutation in O(n), each of the n! orders equally likely |
| `470.cpp` | Implement Rand10() Using Rand7() | (rand7()-1)*7 + rand7() is uniform on 1..49; reject > 40, fold 1..40 mod 10 | turn a small uniform RNG into a different-range one -> rejection sampling |
| `215.cpp` | Kth Largest Element in an Array | quickselect - partition, then keep only the side holding sorted index n-k | k-th order statistic in O(n) average without a full sort |

## Bit tricks

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `136.cpp` | Single Number | XOR everything - pairs cancel (x ^ x = 0) and only the single remains | "appears twice except one" + O(1) space -> XOR |
| `137.cpp` | Single Number II | count each of the 32 bits mod 3 - the leftover bits form the single number | "appears k times except one" -> per-bit counts mod k |
| `338.cpp` | Counting Bits | dp[i] = dp[i >> 1] + (i & 1) - i is i/2 shifted left plus its lowest bit | popcount for a whole range -> reuse the answer of a smaller number |

## Greedy and intervals

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `621.cpp` | Task Scheduler | answer = max(#tasks, (maxCnt-1)*(n+1) + #letters having maxCnt) | cooldown scheduling where only counts matter, not the actual order |
| `435.cpp` | Non-overlapping Intervals | sort by END and greedily keep the earliest-ending one; remove what overlaps it | max set of non-overlapping intervals (activity selection) |
| `763.cpp` | Partition Labels | record each letter's last index; extend end = max(last[c]); cut when i == end | partition so groups never straddle a cut -> last-occurrence sweep |

## Parsing and serialization

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `snowcal.cpp` | SnowCal interpreter (Snowflake interview) | store each body as lines in a map; INV re-runs it recursively on the value | END closes the most recent FUN, so count FUN/END depth to find the match |
| `394.cpp` | Decode String | at '[' push (count, text so far); at ']' pop and append cur repeated count times | nested brackets with an outer context to resume -> stack of saved states |
| `224.cpp` | Basic Calculator | at '(' push (result so far, sign before it); at ')' fold back as one signed term | only + - and parens -> running sum with a sign, no operator precedence |
| `271.cpp` | Encode and Decode Strings | frame each string as "len#str"; decode reads len, then takes len raw chars | no delimiter is safe for arbitrary strings -> length-prefix framing |
| `297.cpp` | Serialize and Deserialize Binary Tree | preorder with "#" for null fixes the shape; rebuild in that order via a cursor | one traversal alone is ambiguous until null markers are written out |

## Graph specials

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `399.cpp` | Evaluate Division | a / b = v is edge a->b (v) and b->a (1/v); x / y = product of weights on a path | ratios / exchange rates that chain transitively -> weighted graph search |
| `269.cpp` | Alien Dictionary | only the FIRST differing char of adjacent words gives an edge; then topo-sort | "sorted by unknown rules" -> derive pairwise constraints, then Kahn |
| `332.cpp` | Reconstruct Itinerary | Hierholzer - DFS smallest ticket first, add airport on the way out, reverse | "use every edge exactly once" -> Eulerian path, not backtracking |

## DP specials

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `300.cpp` | Longest Increasing Subsequence | tails[k] = smallest tail of a length-(k+1) run; binary-search replace in tails | LIS in O(n log n), or anything reducible to LIS (envelopes, stacking) |
| `312.cpp` | Burst Balloons | pick the LAST balloon k burst in (i, j); its neighbors are then fixed at i and j | removing an item changes neighbors -> interval DP over the last one removed |
| `416.cpp` | Partition Equal Subset Sum | odd total -> false; else 0/1 subset-sum to total/2, sums iterated backward | "split into two equal halves" = does some subset reach exactly half |

## String matching

| File | Problem | Trick | Recognize |
|---|---|---|---|
| `28.cpp` | Find the Index of the First Occurrence in a String | KMP - on a mismatch fall back to lps[j-1], never moving back in the haystack | substring search in O(n + m), or any "longest prefix that is also a suffix" |
