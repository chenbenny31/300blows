## 16. 优先级任务执行器：重复 ID 只执行一次（A19 · 无直接 LC；LC 3408 部分原型）
**证据：** 作者评论 IC2；平台不标具体职级；Onsite coding；P0（C01 / J13542）

**本文件合同：**
- 每次 `addTask` 都是一条独立记录；同一 ID 可有多条待执行记录，执行时取该 ID 最优的那条。
- 取任务顺序：priority 大者先；同 priority 时 timestamp 小者先；再相同时 taskId 小者先（全序，结果确定）。
- 一个 ID 一旦执行，永久不再执行：它剩下的旧记录在出堆时跳过，之后再 `addTask` 也直接忽略。
- 没有可执行任务时 `executeTask()` 返回 -1。
- taskId、timestamp 为 `int` 且不为 `INT_MIN`（堆里存相反数）。

**先问面试官：**
- priority 越大越先，还是越小越先？timestamp 是排序键，还是只是记录？
- 同 priority 同 timestamp 的 tie-break：ID 小者先，还是先加入者先？
- 重复 `addTask` 是新增一条记录，还是覆盖该 ID 的旧 priority？（覆盖时降级也要生效，就不能只保留最优）
- 已执行的 ID 再加入：忽略、报错，还是允许再跑一次？
- 执行失败怎么算：算执行过，还是允许重试？

**思路：** 大顶堆存 `(priority, -timestamp, -taskId)`，取反让「更早、更小」在大顶堆里更大，避免写自定义比较器。`done` 集合记录已执行的 ID；出堆时如果 ID 已在 `done` 里就是过期重复记录，丢弃继续弹（lazy deletion）。每条记录最多入堆、出堆各一次，所以均摊对数。

**复杂度：** R = 累计 `addTask` 记录数，X = 已执行 ID 数，P = 待执行的不同 ID 数。
- `TaskExecutor`：`addTask` T O(log R)；`executeTask` 均摊 T O(log R)（单次最坏要弹掉多条过期记录）；S O(R + X)。
- `CompactTaskExecutor`：`addTask` / `executeTask` T O(log P)，S O(P + X)。

**Follow-up：**
- 已实现 `CompactTaskExecutor`：重复 ID 大量涌入时堆会膨胀，改用有序 set + 「ID -> 当前键」，每个待执行 ID 只保留最优记录，新记录更差就忽略，更好就删旧插新；在本合同下与 lazy 堆输出完全相同。
- 覆盖语义（后加入的记录替换旧 priority，即使更低）：用 compact 版本，去掉 `it->second <= key` 的比较，直接删旧插新。
- `done` 无限增长：如果 ID 单调递增，可只存水位线；否则需要和面试官约定保留期限。
- 多线程执行：`executeTask` 的「弹出 + 标记 done」必须在同一把锁内，否则两个 worker 会拿到同一 ID 的两条记录。

**易错点：**
- 只在 `addTask` 时检查 `done`，出堆时不检查：同 ID 在执行前加入的旧记录会让它跑第二次。
- tie-break 依赖插入顺序（或 `priority_queue` 的内部顺序）：堆不稳定，同分顺序不确定；把 ID 放进键里。
- `-timestamp` 写成 `timestamp`：大顶堆会让更晚的任务先执行。
- `executeTask` 用 `if` 只弹一次：遇到过期记录就错误地返回 -1，必须 `while` 弹到有效记录。
