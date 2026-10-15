## 22. 可调容量的 LRU Cache（B10 · LC 146 + resize）
**证据：** 职级／环节未确认；Hack2Hire 导出（A26-C01）。P0。
**本文件合同：**
- `get` miss 返回 -1，且 miss 不改变任何顺序、不插入。
- `get` 命中与 `put`（新键或已有键）都把该键变为最近使用；`put` 已有键只改值，不占新位置。
- `resize(newCap)`：缩小时从最久未使用端连续淘汰直到 `size <= newCap`；扩容保留全部数据；相同容量为 no-op。
- 容量允许 0：`put` 后立即被淘汰，`get` 永远 -1；`resize(0)` 清空。假设容量不为负。
- 单线程；键、值为 `int`。

**先问面试官：**
- miss 返回什么？miss 是否影响顺序？
- 容量能否为 0 或负数？`resize` 到 0 是清空还是报错？
- `put` 已有键是否算一次“使用”？
- 需要线程安全吗？允许用 `std::list` 吗，还是要手写双向链表？

**思路：** `std::list` 保存 (key, value)，前端最新、后端最旧；`unordered_map` 存 key → list iterator。`splice` 把节点移到前端是 O(1) 且不使迭代器失效。`put` 与 `resize` 共用同一个 `evict` 循环：只要 size > cap 就删除后端，先删 map 再 `pop_back`。

**复杂度：** n 为当前条目数，e 为一次 `resize` 淘汰的条目数。
- `get` / `put`：T 平均 O(1)；S 总体 O(n)（n ≤ cap）。
- `resize`：T O(e)，最坏 O(n)，所以不能说所有操作严格 O(1)（摊还看，每个条目最多被淘汰一次，总淘汰量 ≤ 总插入量）。

**Follow-up：**
- 本文件主体即 follow-up：`resize`（已实现，含一次淘汰多项、扩容、cap 0）。
- 禁用 `std::list`（未实现）：手写双向链表 + head/tail 哨兵节点，`remove(node)` + `addFront(node)`；记得析构时释放所有节点。
- 线程安全（未实现）：最简单是一把 `std::mutex` 包住每个操作；`get` 也改顺序，所以读写锁帮助不大。
- 返回被淘汰项 / 淘汰回调：在 `evict` 里收集 `order.back()`。

**易错点：**
- 先 `pop_back` 再用 `order.back().first` 删 map，删错键或访问已销毁节点。
- `get` 用 `pos[key]` 判断存在，miss 时插入一个悬空空迭代器。
- `put` 已有键时重新 `push_front` 而不删旧节点，造成重复条目与 size 错误。
- `resize` 只淘汰一个（用 `if` 而不是 `while`）。
