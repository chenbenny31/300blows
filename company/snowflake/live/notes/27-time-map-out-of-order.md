## 27. TimeMap：乱序 timestamp（B24 · LC 981 乱序写入追问）
**证据：** 职级未确认；平台 Phone Screen（Jobright J9934）。P0。
**本文件合同：**
- `set(key, value, timestamp)` 的 timestamp 可以任意顺序到达；保留全部历史版本。
- `get(key, t)` 返回该 key 中 timestamp `<= t` 的最大版本的值；没有则返回 `""`（未知 key 同样返回 `""`）。
- 同一 key 同一 timestamp 再次 `set`：覆盖旧值。
- 假设 value 非空，所以 `""` 只表示“不存在”；timestamp 为非负 `int`。
- 不是事务 / rollback KV。

**先问面试官：**
- set 的 timestamp 是否单调？乱序时是否需要 get 立刻反映？
- 同 key 同 timestamp：覆盖、忽略还是保留多份？
- get 的边界是 `<= t` 还是 `< t`？t 小于所有版本时返回什么？
- 是否需要删除旧版本 / 限制历史长度？读多还是写多？

**思路：** 每个 key 一个按 timestamp 排序的结构。`map<int,string>` 直接 `store[key][ts] = value` 完成插入或覆盖；查询用 `upper_bound(t)` 找到第一个 `> t` 的版本，若它是 `begin()` 说明全部版本都比 t 新，否则 `prev` 即答案。方法二用排序 vector：`lower_bound` 定位，相等则覆盖，否则在该处插入（乱序时需要移动元素），查询同样 `upper_bound` + `prev`。

**复杂度：** v 为该 key 的版本数（忽略 key 的哈希成本）。
- `set` / `get`（map）：T O(logv) / O(logv)，S O(总 set 次数)。
- `set2` / `get2`（sorted vector）：T O(v)（插入时移动）/ O(logv)，S O(总 set 次数)；时间戳基本有序时 `set2` 接近 O(1) 追加，且查询缓存友好。

**Follow-up：**
- 已实现 `set2` / `get2`：排序 vector 版本，对比写入代价。
- 时间戳单调（LC 981 原题）：直接 `push_back` + `upper_bound`，`set` O(1)。
- 限制历史 / 过期清理：若 `p = upper_bound(cutoff)` 不是 `begin()`，执行 `erase(begin(), prev(p))`：删掉更旧的版本，但保留最后一个 `<= cutoff` 的版本，否则 `get(key, t >= cutoff)` 会错误地变成空。
- 并发：读多写少可用 `shared_mutex`，每个 key 一把锁可降低争用。

**易错点：**
- 对 `begin()` 调 `prev`（t 小于所有版本），UB。
- 用 `lower_bound(t)` 当成“<= t 的最后一个”，在没有精确命中时多走一位。
- `std::upper_bound` 自定义比较器参数顺序是 `(value, element)`，与 `lower_bound` 的 `(element, value)` 相反。
- `get` 用 `store[key]` 查找，未知 key 被插入空 map。
