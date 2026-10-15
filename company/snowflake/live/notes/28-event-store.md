## 28. 事件流保留最新 m 条 + 时间查询（B06 · 无直接 LC；635、347 为部分原型）
**证据：** 职级未确认；phone screen（地里 C05、Jobright J14594）。P0。
**本文件合同：**
- m 是 **记录条数**（平台版本明确）；`m = 0` 什么都不保留。
- 记录可以乱序到达；始终保留 timestamp 最大的 m 条。比现有全部记录都旧的新记录在已满时立即被丢弃。
- timestamp 相同的并列：先淘汰 **更早被 record 的那条**（FIFO），与 key 字典序无关。
- 完全相同的 (timestamp, key) 是两条独立记录，各占一个名额、各计一次频率。
- `countDistinctBefore(t)`：保留记录中 timestamp **严格小于** t 的不同 key 数。
- `mostFrequentKey()`：保留记录中出现次数最多的 key，并列取字典序最小，空时返回 `""`。

**先问面试官：**
- m 指记录数，还是不同 timestamp 数 / 时间跨度？
- 记录会乱序到达吗？同 timestamp 多条时淘汰哪条？
- “before t” 是 `< t` 还是 `<= t`？
- 最高频 key 的统计范围（仅保留记录还是全部历史）、并列规则、空时返回值？
- 查询与写入的比例（决定是否值得维护更复杂的索引）？

**思路：** `multimap<int,string>` 按 timestamp 排序保存每条保留记录；`insert` 会把新元素放在相同 key 区间的末尾（C++11 起保证），所以 `begin()` 恰好是“最旧、并列时最早记录”的那条。每次插入后若超过 m 就删除 `begin()`，同步减少 `freq`，减到 0 时 erase，使 `freq` 只含保留记录。计数查询从头顺序扫到 `timestamp >= t` 为止，用 hash set 去重；最高频直接扫 `freq`。

**复杂度：** m 为容量，W 为保留记录中 timestamp < t 的条数，U 为保留记录中不同 key 数。
- `record`：T O(logm)，S 总体 O(m)。
- `countDistinctBefore`：T O(W)，S O(W)。
- `mostFrequentKey`：T O(U)，S O(1)。

**Follow-up：**
- 已实现 `mostFrequentKey()`（独立测试 `testMostFrequent` 等）。
- 最高频 O(logU) 更新 + O(1) 查询（未实现）：再维护 `set<pair<int,string>> byFreq` 存 `(-count, key)`，每次计数变化先 erase 旧条目再 insert 新条目，查询取 `begin()`。
- 不同 key 计数更快（未实现）：查询多时可对每个 key 维护其最早保留 timestamp，答案 = 最早 timestamp < t 的 key 数，用有序结构 + 树状数组可做 O(log)；面试中先讲清 O(W) 版本。
- 若 m 指时间跨度：淘汰条件改为 `begin()->first <= newest - m`，循环删除。

**易错点：**
- 用 `multiset<pair<int,string>>`，并列时按 key 淘汰，与“先记录先淘汰”的合同不一致（测试 "tie evicted z"）。
- 用 `set` 而不是 multi 容器，丢掉完全相同的重复记录。
- 新记录比全部旧时先判断“满了就删 begin”再插入，删掉了本该保留的记录。
- 淘汰后 `freq` 没减或减到 0 不删除，`mostFrequentKey` 返回已不存在的 key。
