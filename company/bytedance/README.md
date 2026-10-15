# ByteDance (TikTok) — NG coding prep

Sources: hack2hire ByteDance/TikTok NG reports (tag `ByteDance (hack2hire)`) and the LeetCode
ByteDance company list (tag `ByteDance / LC`). OA-only questions are excluded. Every file
compiles with `g++ -std=c++17 -Wall -Wextra` and prints `label PASS` per test. Follow-ups are
separate functions/classes without tests. Problems shared with Apple reuse the Apple version.

## ng/ — must do (top of the LC frequency list + most-reported phone/onsite questions)
| File | Source | Problem |
|---|---|---|
| `01-lru-cache.cpp` | ByteDance (hack2hire) / LC 146 | lru cache |
| `02-number-of-islands.cpp` | ByteDance (hack2hire) / LC 200 | number of islands |
| `03-merge-intervals.cpp` | ByteDance (hack2hire) / LC 56 | merge intervals |
| `04-course-schedule.cpp` | ByteDance (hack2hire) / LC 207 | course schedule |
| `05-kth-largest-quickselect.cpp` | ByteDance (hack2hire) / LC 215 | kth largest quickselect |
| `06-shortest-bridge.cpp` | ByteDance / LC 934 | shortest bridge |
| `07-word-ladder.cpp` | ByteDance / LC 127 | word ladder |
| `08-house-robber.cpp` | ByteDance (hack2hire) / LC 198 | house robber |
| `09-two-sum.cpp` | ByteDance / LC 1 | two sum |
| `10-add-two-numbers.cpp` | ByteDance / LC 2 | add two numbers |
| `11-search-rotated-array.cpp` | ByteDance (hack2hire) / LC 33 | search rotated array |
| `12-combination-sum.cpp` | ByteDance (hack2hire) / LC 39 | combination sum |
| `13-multiply-strings.cpp` | ByteDance / LC 43 | multiply strings |
| `14-max-subarray.cpp` | ByteDance (hack2hire) / LC 53 | max subarray |
| `15-best-time-stock.cpp` | ByteDance / LC 121 | best time stock |
| `16-maximal-square.cpp` | ByteDance / LC 221 | maximal square |
| `17-basic-calculator-ii.cpp` | ByteDance (hack2hire) / LC 227 | basic calculator ii |
| `18-basic-calculator.cpp` | ByteDance / LC 224 | basic calculator |
| `19-top-k-frequent.cpp` | ByteDance / LC 347 | top k frequent |
| `20-max-width-binary-tree.cpp` | ByteDance (hack2hire) / LC 662 | max width binary tree |
| `21-rotting-oranges.cpp` | ByteDance / LC 994 | rotting oranges |
| `22-longest-common-subsequence.cpp` | ByteDance / LC 1143 | longest common subsequence |
| `23-longest-repeating-char-replacement.cpp` | ByteDance (hack2hire) / LC 424 | longest repeating char replacement |
| `24-longest-substring-no-repeat.cpp` | ByteDance (hack2hire) / LC 3 | longest substring no repeat |
| `25-implement-queue.cpp` | ByteDance (hack2hire) | implement queue |
| `26-path-sum-ii.cpp` | ByteDance (hack2hire) / LC 113 | path sum ii |
| `27-all-paths-directed.cpp` | ByteDance (hack2hire) | all paths directed |
| `28-all-cycles-directed.cpp` | ByteDance (hack2hire) | all cycles directed |
| `29-design-hashmap.cpp` | ByteDance (hack2hire) / LC 706 | design hashmap |
| `30-reverse-words.cpp` | ByteDance (hack2hire) / LC 151 | reverse words |

## important/ — rest of the LC list and the solid hack2hire reports
| File | Source | Problem |
|---|---|---|
| `31-subsets.cpp` | ByteDance (hack2hire) / LC 78 | subsets |
| `32-add-strings-char-arrays.cpp` | ByteDance (hack2hire) / LC 415 variant | add strings char arrays |
| `33-substructure-of-tree.cpp` | ByteDance (hack2hire) / Jianzhi Offer 26 | substructure of tree |
| `34-kth-node-from-end.cpp` | ByteDance (hack2hire) | kth node from end |
| `35-three-sum-closest.cpp` | ByteDance / LC 16 | three sum closest |
| `36-merge-k-sorted-lists.cpp` | ByteDance / LC 23 | merge k sorted lists |
| `37-trapping-rain-water.cpp` | ByteDance / LC 42 | trapping rain water |
| `38-jump-game.cpp` | ByteDance / LC 55 | jump game |
| `39-reverse-linked-list-ii.cpp` | ByteDance / LC 92 | reverse linked list ii |
| `40-restore-ip-addresses.cpp` | ByteDance / LC 93 | restore ip addresses |
| `41-zigzag-level-order.cpp` | ByteDance / LC 103 | zigzag level order |
| `42-clone-graph.cpp` | ByteDance / LC 133 | clone graph |
| `43-sort-list.cpp` | ByteDance / LC 148 | sort list |
| `44-find-peak-element.cpp` | ByteDance / LC 162 | find peak element |
| `45-sliding-window-max.cpp` | ByteDance / LC 239 | sliding window max |
| `46-decode-string.cpp` | ByteDance / LC 394 | decode string |
| `47-remove-k-digits.cpp` | ByteDance / LC 402 | remove k digits |
| `48-delete-node-bst.cpp` | ByteDance / LC 450 | delete node bst |
| `49-single-element-sorted.cpp` | ByteDance / LC 540 | single element sorted |
| `50-subarray-sum-k.cpp` | ByteDance / LC 560 | subarray sum k |
| `51-accounts-merge.cpp` | ByteDance / LC 721 | accounts merge |
| `52-infected-tree-time.cpp` | ByteDance / LC 2385 | infected tree time |
| `53-longest-common-prefix-length.cpp` | ByteDance / LC 3043 | longest common prefix length |
| `54-design-memory-allocator.cpp` | ByteDance / LC 2502 | design memory allocator |
| `55-product-last-k.cpp` | ByteDance / LC 1352 | product last k |
| `56-valid-parentheses.cpp` | ByteDance (hack2hire) / LC 20 | valid parentheses |
| `57-top-k-kols.cpp` | ByteDance (hack2hire) | top k kols |
| `58-add-search-word.cpp` | ByteDance (hack2hire) / LC 211 | add search word |
| `59-sqrt-precision.cpp` | ByteDance (hack2hire) | sqrt precision |
| `60-permutation-in-string.cpp` | ByteDance (hack2hire) / LC 567 | permutation in string |
| `61-lfu-cache.cpp` | ByteDance (hack2hire) / LC 460 | lfu cache |
| `62-max-path-sum.cpp` | ByteDance (hack2hire) / LC 124 | max path sum |
| `63-longest-palindromic-substring.cpp` | ByteDance (hack2hire) / LC 5 | longest palindromic substring |
| `64-group-anagrams.cpp` | ByteDance (hack2hire) / LC 49 | group anagrams |
| `65-regex-matching.cpp` | ByteDance (hack2hire) / LC 10 | regex matching |
| `66-longest-increasing-subsequence.cpp` | ByteDance (hack2hire) / LC 300 | longest increasing subsequence |

## optional/ — hards, niche, or intern / non-SWE-role reports
| File | Source | Problem |
|---|---|---|
| `67-text-justification.cpp` | ByteDance / LC 68 | text justification |
| `68-remove-duplicate-letters.cpp` | ByteDance / LC 316 | remove duplicate letters |
| `69-shortest-distance-buildings.cpp` | ByteDance / LC 317 | shortest distance buildings |
| `70-min-area-rectangle.cpp` | ByteDance / LC 939 | min area rectangle |
| `71-remove-zero-sum-nodes.cpp` | ByteDance / LC 1171 | remove zero sum nodes |
| `72-k-length-substrings-no-repeat.cpp` | ByteDance (hack2hire) / LC 1100 | k length substrings no repeat |
| `73-remove-duplicates-ii.cpp` | ByteDance (hack2hire) / LC 80 | remove duplicates ii |
| `74-missing-ranges.cpp` | ByteDance (hack2hire) / LC 163 | missing ranges |
| `75-swim-rising-water.cpp` | ByteDance (hack2hire) / LC 778 | swim rising water |
| `76-combinations.cpp` | ByteDance (hack2hire) / LC 77 | combinations |
| `77-photo-groups.cpp` | ByteDance (hack2hire) / LC 547 | photo groups |
| `78-valid-parenthesis-string.cpp` | ByteDance (hack2hire) / LC 678 | valid parenthesis string |
| `79-search-insert-position.cpp` | ByteDance (hack2hire) / LC 35 | search insert position |
| `80-circular-deque-index.cpp` | ByteDance (hack2hire) | circular deque index |
| `81-daily-login-counts.cpp` | ByteDance (hack2hire) | daily login counts |
| `82-min-cost-steps.cpp` | ByteDance (hack2hire) | min cost steps |
| `83-construct-pre-in.cpp` | ByteDance (hack2hire) / LC 105 | construct pre in |
| `84-populate-next-right.cpp` | ByteDance (hack2hire) / LC 116 | populate next right |
| `85-house-robber-iii.cpp` | ByteDance (hack2hire) / LC 337 | house robber iii |
| `86-n-queens.cpp` | ByteDance (hack2hire) / LC 51 | n queens |
| `87-largest-rectangle-histogram.cpp` | ByteDance (hack2hire) / LC 84 | largest rectangle histogram |
| `88-kth-smallest-subarray-sum.cpp` | ByteDance (hack2hire) / LC 1918 | kth smallest subarray sum |
| `89-camel-case-filter.cpp` | ByteDance (hack2hire) | camel case filter |
| `90-construct-pre-post.cpp` | ByteDance (hack2hire) / LC 889 | construct pre post |
| `91-max-candies.cpp` | ByteDance (hack2hire) / LC 2226 | max candies |
| `92-min-jumps-home.cpp` | ByteDance (hack2hire) / LC 1654 | min jumps home |
