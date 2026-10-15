## 43. 支持嵌套事务的 KV（I02 · 无直接 LC；706 / 981 / 1146 部分原型）
**证据：** New Grad + Intern 平台标签；Phone Screen / OOD；P1 基础，P2 完整 ACID
**本文件合同：**
- 单客户端、单线程；key、value 都是 string，value 非空，`get` 缺失返回 `""`。
- 支持嵌套事务：`commit` / `rollback` 只作用于最内层；内层 commit 后的修改归入父事务，父事务 rollback 仍会撤销它们。
- 没有打开的事务时 `commit` / `rollback` 返回 false 且不改变任何状态；事务外的 `set` / `del` 直接生效。
- 每层一个 undo log：key -> 本层第一次写之前的值，`std::optional` 的 nullopt 表示"本来不存在"（rollback 时要 erase）。
- 内层 commit 把 undo log 合并到父层，父层已有的 key 保留父层的记录（更早的值）。
- 不做 durability（无 WAL / 落盘），所以不声称完整 ACID。

**先问面试官：**
- 需要嵌套事务吗？commit 是只关闭最内层还是全部提交？
- 没有事务时 commit / rollback：报错、返回 false，还是 no-op？
- get 不存在的 key 返回什么？value 可以是空串吗？
- 多客户端并发吗？要哪种隔离级别（snapshot isolation / serializable）？冲突时谁失败？
- 对复杂度有要求吗（如"rollback 必须 O(1)"在 undo log 下做不到，只能 O(本层修改的 key 数)）？

**思路：** 当前数据始终在 `store` 里，读是 O(1)。每次写之前，如果本层还没记录过这个 key，就把它的旧值（或"不存在"）存进本层 undo log；同一层后续再写不再记录，所以 log 里永远是"进入本层之前"的值。rollback 按 log 恢复或删除；commit 把本层 log 合并进父层，用 `emplace` 让父层的更早记录胜出，于是父层 rollback 能一直回到父层开始时的状态。最外层 commit 时直接丢弃 log 即可。

**复杂度：** `get` / `set` / `del` T O(1) 平均；`begin` O(1)；`commit` / `rollback` T O(K)，K 为最内层事务修改过的不同 key 数；S O(N + ΣK)，N 为当前 key 数。

**Follow-up：**
- 另一种实现：每次 `begin` 拷贝整个 store 作快照，rollback 直接换回；写起来更短，但 begin O(N)、空间 O(N · 深度)，key 多时不可接受。
- 多客户端隔离（只口述）：
  - Snapshot isolation：每个事务读开始时刻的快照，写入私有 write set，提交时若有 key 在此期间已被别人提交就 abort（first-committer-wins）；会出现 write skew，所以不等于 serializable。
  - Serializable：要么严格两阶段锁（读锁 / 写锁持有到 commit，需要死锁检测或超时），要么在 SI 上加读写冲突检测（SSI）。
  - MVCC 实现 SI：每个 key 存 `(commitTs, value)` 版本链，读按 startTs 找最新的 ≤ startTs 版本，读不阻塞写；需要后台回收旧版本。锁方案实现简单但读写互相阻塞。
  - 本单客户端版本可作为每个客户端的私有 write buffer，commit 时在全局锁或版本校验下一次性应用，保证原子性。
- O(1) 的 `count(value)` 一类查询：额外维护 value -> 计数，set / del / rollback 时同步更新。

**易错点：**
- 不记录"本来不存在"，rollback 后新建的 key 还残留（测试 y、z）。
- 同一层每次写都覆盖 undo 记录，rollback 只能回到上一次写之前（测试 x 应回到 "1" 而不是 "3"）。
- 内层 commit 合并时用 `parent[key] = old` 覆盖父层记录，父层 rollback 回不到父层开始时的值。
- 最外层 commit 时还去访问 `undo[size - 2]`（size 为 1 时越界）；以及在没有事务时 commit / rollback 不返回 false。
