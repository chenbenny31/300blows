# Snowflake NG / IC1 / IC2 / Junior：live-coding 解答（按优先度）

来源清单：`Snowflake_NG_IC1_IC2_Junior_Coding_Final_20261012.md`。代码风格：`cpp-live-interview-style`（单文件、可运行、`test` / `check` 函数打印 FAIL、无 `assert`、无 `using namespace std`）。
算法与测试用例参考了 `../ref/*.cpp` 和 `../ref/snowflake-handout.md`；根目录下你自己的 `.cpp` 没有改动。

每题两份文件：`NN-slug.cpp`（代码 + 测试）和 `notes/NN-slug.md`（审阅卡：合同、先问什么、思路、复杂度、follow-up、易错点）。

本地运行：

```bash
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined NN-slug.cpp -o a && ./a
```

全部 43 个文件已经用 g++ 13（ASan + UBSan + `_GLIBCXX_DEBUG`）和 clang++ 编译，零警告，测试全部通过。

## 排序依据

优先度是准备建议，不是统计出来的出题频率（清单本身也不据此统计频率）。排序规则：
1. **Tier 1**：清单「先做这 12 个题族」，组内按证据强度排（明确 NG 原帖 > NG 平台标签 > 职级未确认的实际面经）。
2. **Tier 2**：清单「之后补」的 5 题 + 其余带 NG / IC1 / IC2 标签的 P0 + Intern 来源的 P0。
3. **Tier 3**：职级未确认、但有 Phone Screen 记录的 P0。
4. **Tier 4**：P1。

## Tier 1：先做这 12 个

| # | 文件 | 题族 | 证据 | LC |
|---|---|---|---|---|
| 01 | `01-grid-shortest-path` | A07 障碍网格最短路（+ 返回路径） | 明确 NG 原帖 + NG 标签 | 1091 原型 |
| 02 | `02-nearest-two-distance` | A08 每个 1 到最近 2（+ B25 全局最近一对） | 明确 NG 原帖 + NG 标签 | 821 / 243 |
| 03 | `03-forest-delete-reindex` | A02 parent 数组删除并重新编号 | NG 标签 | 1110 弱原型 |
| 04 | `04-dag-privilege` | A09 DAG 权限并集（+ 有序合并） | NG 标签 | 210 / 2192 |
| 05 | `05-course-schedule-ii` | A13 Course Schedule II（+ I04 只修目标课所需） | NG 标签 | 210 |
| 06 | `06-tree-boundary` | A14 二叉树逆时针边界（+ 求和） | NG 标签 | 545 |
| 07 | `07-tree-height-after-deletes` | B01 删除后 N 叉树高度 | 职级未确认，两轮 coding | 559 原型 |
| 08 | `08-connect-four` | B03 当前落子是否四连（+ 重力落子） | 职级未确认，两轮 coding | 348 原型 |
| 09 | `09-kv-store-ttl` | B04 KV CRUD → TTL → 按时间列出有效项 | 职级未确认，两轮店面 | 706 / 1797 |
| 10 | `10-encode-decode-strings` | B05 Encode / Decode Strings | 职级未确认，phone screen | 271 |
| 11 | `11-in-memory-file-system` | B07 内存文件系统 | 职级未确认，phone screen | 588 |
| 12 | `12-word-search-ii` | B16 Word Search II | 职级未确认，两轮 phone screen | 212 |

## Tier 2：之后补 + 其余有级别标签的 P0

| # | 文件 | 题族 | 证据 | LC |
|---|---|---|---|---|
| 13 | `13-parallel-courses` | A05 最少学期（+ 异时长 LC2050） | NG 标签 | 1136 / 2050 |
| 14 | `14-throne-inheritance` | A11 王位继承 | NG 标签 + IC1 二手变体 | 1600 |
| 15 | `15-top-k-unique-searches` | A12 按独立用户数 Top-K 搜索词 | NG 标签 | 347 / 692 原型 |
| 16 | `16-task-executor-dedup` | A19 优先级任务执行器，重复 ID 只执行一次 | 作者评论 IC2 | 3408 部分原型 |
| 17 | `17-dag-reachability` | A15 DAG 可达集合 | IC2 标签 | 2192 反向 |
| 18 | `18-subtree-sum-tree` | A06 子树和构建新树 | NG 标签 | 1339 原型 |
| 19 | `19-binary-search-queries` | A04 二分查找 + 重复查询 | NG 标签 | 704 / 34 |
| 20 | `20-web-crawler` | A01 Web Crawler 最少点击（+ 路径） | IC1 | 1236 / 127 原型 |
| 21 | `21-acl-allow-deny` | A18 多重继承 ACL allow / deny | IC1 二手摘编 | 210 / 2192 原型 |
| 22 | `22-lru-cache-resize` | B10 可 resize 的 LRU | 职级未确认（Hack2Hire） | 146 |
| 23 | `23-top-k-book-sales` | B12 累计销量 Top-K | 职级未确认（Hack2Hire） | 347 / 692 原型 |
| 24 | `24-rate-limiter` | B15 滑动窗口限流（+ 多规则） | 职级未确认 | 359 / 362 原型 |
| 25 | `25-meeting-rooms-bus` | I01 Meeting Rooms II（+ 巴士复用） | Intern 标签 | 253 / 252 |
| 26 | `26-copy-random-list` | I05 复制带随机指针的链表 | Intern 标签 | 138 |

## Tier 3：职级未确认的 P0

| # | 文件 | 题族 | LC |
|---|---|---|---|
| 27 | `27-time-map-out-of-order` | B24 TimeMap 乱序时间戳 | 981 |
| 28 | `28-event-store` | B06 事件流保留最近 m 条 | 635 / 347 部分原型 |
| 29 | `29-merge-intervals` | B08 合并区间 | 56 |
| 30 | `30-valid-parentheses` | B09 括号匹配 () → ()[]{} | 20 |
| 31 | `31-find-anagrams` | B17 找所有字母异位词 | 438 |
| 32 | `32-rotated-array-min` | B18 旋转数组最小值（+ 有重复） | 153 / 154 |
| 33 | `33-reverse-alnum-runs` | B20 反转连续字母数字段 | 344 原型 |
| 34 | `34-tax-brackets` | B22 分段税档 | 2303 |

## Tier 4：P1

| # | 文件 | 题族 | LC |
|---|---|---|---|
| 35 | `35-course-retry` | A17 课程发现与失败重试（IC2） | 210 原型 |
| 36 | `36-overpay-coins` | A10 允许多付找零的最少硬币（NG） | 322 仅原型 |
| 37 | `37-max-throughput` | B11 预算内最大化最小吞吐 | 875 / 2064 原型 |
| 38 | `38-query-profit` | B14 只选一种 query 的最大利润 | 无强映射 |
| 39 | `39-work-schedule` | B13 补齐七天工时 | 39 / 216 原型 |
| 40 | `40-blacklist-stream` | B19 动态黑名单过滤 | 706 基础 |
| 41 | `41-integer-ops-path` | B21 +2 / ×2 / ÷2 任意路径 | 397 弱原型 |
| 42 | `42-subtree-average` | B23 子树平均值的平均 | 2265 原型 |
| 43 | `43-transaction-kv` | I02 事务 KV（单客户端，嵌套 begin / commit / rollback） | 706 / 1146 部分原型 |

## 没有写代码的题

- **A03 异步消息统计分布式树、I03 SQL Operator Pipeline**：协议 / OOD 设计题，属于后续 onsite，不是传统 coding。
- **B02 最少删除使树高 ≤ H、A16 LC1235、D01、D02**：DP，与「screen 不考 DP」冲突，放在最后；B02 的思路写在 07 号审阅卡的 follow-up 里。
- **B26 恰好三染色**：P2。
- **I02 的多客户端并发 ACID**：只在 43 号审阅卡里讨论（隔离级别、加锁 vs MVCC），代码是单客户端版本。

## 需要你留意的合同选择

这些地方清单本身没有定死，代码选了一种并写在审阅卡里，面试时先问清楚：

- **09 TTL KV**：`update` 不续期，`ttl <= 0` 表示永不过期，`now < expireAt` 才算有效。
- **16 任务执行器**：优先级大的先执行，同优先级时间戳早的先，再按 ID 小的先。
- **21 ACL**：子节点只继承父节点的**最终**权限集合，自己的 allow 可以重新授予；「上游 deny 一律生效」的另一种合同写在卡里，菱形测试会给出不同结果。
- **28 事件流**：时间戳相同时先到的记录先被淘汰。
- **34 税档**：超过最高档的收入是否征税，由构造参数切换。
- **36 硬币**：只枚举找零 c ∈ [0, 200)；c ≥ 200 时付款和找零里各去掉一枚 200 会更优，卡里有证明。
- **42 子树平均**：除法向零截断；向下取整的版本是 follow-up，两者在负数上结果不同。
