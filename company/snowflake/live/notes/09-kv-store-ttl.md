## 09. 带 TTL 的 KV 存储 + 按时间枚举有效项（B04 · 无直接 LC；706 / 1797 / 981 部分原型）
**证据：** 职级未确认；地里两轮店面（C12）；Phone Screen；P0。

**本文件合同：**
- `set(key, value, now, ttl)`：`ttl > 0` 时 `expireAt = now + ttl`；`ttl <= 0` 表示永不过期（内部存 `LLONG_MAX`，不进索引）。set 总是覆盖 value 和 expiry（相当于 upsert + 续期）。
- 有效条件 `now < expireAt`；`now == expireAt` 已过期。
- `update(key, value, now)` 只对有效 key 生效、**保持原 expiry**，不创建新 key；要改 TTL 就调用 `set`。返回是否成功。
- `get` 缺失或过期返回 `""`（合同：value 非空）；遇到过期 key 顺手 erase（lazy）。`remove` 返回"是否删掉了一个有效 key"，过期 key 也会被物理删掉但返回 false。
- 所有调用的 `now` 非递减（被 purge 掉的 key 不能"回到过去"再读到）。
- `listValid(now)` 按 key 字典序返回 `(key, value)`；`std::map` 天然有序，不需要 sort。

**先问面试官：**
- TTL 是相对 `now + ttl` 还是绝对过期时间？`now == expireAt` 算不算有效？
- update 是否续期？对不存在 / 已过期的 key 是报错、返回 false 还是当作 set？
- 时间是否单调？是否会查询历史时刻（要求保留已过期数据 → 不能物理删除）？
- get 的缺失返回值：`""`、`optional`、还是抛异常？value 能否为空串？
- listValid 的频率和规模：是否需要比 O(n) 更好（决定要不要维护过期索引）？

**思路：** 主存 `map<key, Entry{value, expireAt}>`，过期判断永远以主存里的 `expireAt` 为准。基础版 lazy：get / update / remove 碰到过期就删，`listValid` 扫描过滤。Follow-up 加 `multimap<expireAt, key>` 索引，`purgeExpired(now)` 从最早的过期时间开始弹出；索引里允许有 stale pair（key 被重新 set / 删除后旧 pair 不去删），弹出时只有当主存里该 key 的 `expireAt <= now` 才 erase，所以旧 pair 永远删不掉新写入。每个 pair 只入、出各一次，摊还 O(log m)。

**复杂度：** n = 主存条目数（含未清理的过期项），m = 索引 pair 数（含 stale），x = 本次真正删掉的过期 key，s = 本次弹出的 stale pair，k = 输出条数；字符串比较长度视为常数。
- set / get / update / remove：T O(log n + log m)，S O(1)。
- `listValid`（扫描）：T O(n)，S O(k)。
- `purgeExpired`：T O((x + s) log m)，摊还到每次 set 为 O(log m)；S O(1)。
- `listValid2`（先 purge 再全量拷贝）：T O(k) + purge，S O(k)。总空间 O(n + m)。

**Follow-up：**
- **已实现：** `purgeExpired(now)` + `listValid2(now)`，过期 key 在 O(x log m) 内清除，枚举只付输出成本 O(k)。
- 索引膨胀：反复 set 同一个 key 会积累 stale pair。可改成 eager 删除：用 `set<pair<expireAt, key>>`，set 前 `erase({old.expireAt, key})`，索引大小恒等于有 TTL 的 key 数；代价是每个 set / remove 多一次 O(log m) 删除，且要记得 remove 也要删索引。
- 要查历史时刻（时间可回溯）：不能物理删除，改为每个 key 存版本列表 + 二分（LC981 思路）。
- 高并发 / 后台清理：purge 放后台线程按时间片执行，需加锁或分片；get 仍做 lazy 判断以保证正确性不依赖后台进度。
- 返回值区分"缺失"和"空值"：改成 `std::optional<string>`。

**易错点：**
- 重新 set 后旧索引 pair 到期，按 pair 直接 erase key，把新值删了（本文件靠"以主存 expireAt 为准"避免；测试 `testResetWithNewExpiry` 专门覆盖）。
- 边界写成 `now > expireAt`，导致 `now == expireAt` 时仍返回旧值。
- `store[key]` 用在读路径（get / update）上，会默认插入空条目。
- `ttl <= 0` 时算 `now + ttl` 当成已过期，或把 `LLONG_MAX` 也塞进索引 / 做加法溢出。
