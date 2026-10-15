## 24. 滑动窗口限流器 + 多规则（B15 · 无直接 LC；359、362 为窗口原型）
**证据：** 职级未确认；传统 coding 与题库记录（Hack2Hire A26-C07、地里 C18、Jobright）；Onsite coding，fake clock 测试条目来自 Phone Screen。P0 基础，P1 多规则。
**本文件合同：**
- `SlidingWindowLimiter(limit, W)`：每个 client 独立；窗口为 `(t - W, t]`，即时间戳 `<= t - W` 的记录过期。
- 只记录 **被接受** 的请求；被拒绝的请求不占额度。`limit = 0` 永远拒绝。
- 同一 client 的时间戳非递减（测试用显式 timestamp 作为 fake clock，不读系统时钟）。
- `MultiRuleLimiter`：全局（不分 client）；`allowRequest(t)` 先检查所有规则，全部通过才在每条规则里记一次；任一失败则不扣任何规则。
- 无规则时一律允许；中途 `registerRule` 的新规则从空日志开始，只统计注册之后被接受的请求。

**先问面试官：**
- 窗口端点：`(t-W, t]` 还是 `[t-W, t]`？同一毫秒多个请求如何算？
- 被拒绝的请求是否计入？多规则时拒绝是否消耗其他规则额度？
- 时间戳是否保证单调？乱序到达怎么处理？
- 多规则是全局还是按 client？新注册的规则是否回看历史请求？
- 内存要求：是否需要 O(1) 空间（会改成近似算法，必须说明语义变化）？

**思路：** 滑动窗口日志：每个 client 一个 deque 存窗口内被接受的时间戳，请求到来时先从队头弹出 `<= t - W` 的过期项，剩余数量 `< limit` 才接受并入队。多规则版本分两遍：第一遍对每条规则清理过期并检查，任何失败立即返回 false（此时尚未写入任何规则）；第二遍才把 t 写入所有规则，从而保证“全过才扣”。

**复杂度：** C 为 client 数，R 为规则数。
- `allow`：T 摊还 O(1)（每个时间戳最多入队出队各一次），S O(C·limit)。
- `allowRequest`：T 摊还 O(R)，S O(Σ limit_i)。

**Follow-up：**
- 已实现第二个类 `MultiRuleLimiter`（registerRule + allowRequest，全过才扣）。
- 可控时间测试：本文件所有测试都传显式时间戳；若接口不带时间，就注入 `Clock` 接口 / 函数指针，测试里用可手动推进的 fake clock。
- 常数空间追问：fixed window counter 或 sliding window counter（两个桶加权）是 **近似**，会改变语义，必须明确说出来；token bucket 是另一种策略，不能悄悄替换。
- 多 client + 多规则：把 `logs` 放进 `unordered_map<string, vector<deque<long long>>>`。
- 线程安全：每个 client 一把锁，或整体一把锁；检查与扣减必须在同一临界区。

**易错点：**
- 端点写成 `< t - W`，导致恰好 W 前的请求仍被计入（测试 a@1000、a@1100）。
- 被拒绝的请求也入队，后面正常请求被误拒。
- 多规则逐条“检查并写入”，前面的规则已扣额度、后面的规则才拒绝（测试 @5 / @995）。
- 时间戳用 int，毫秒 epoch 溢出。
