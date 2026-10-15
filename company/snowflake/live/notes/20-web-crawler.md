## 20. Web Crawler：最少点击数与路径（A01 · 无直接 LC；LC 1236 / 127 原型）
**证据：** IC1；平台标签／二手摘编；Virtual Onsite / 两轮 coding；P0（J15972 / H01）

**本文件合同：**
- `getAllLinks` 以 `std::function<vector<string>(const string&)>` 注入；测试里用 `unordered_map<string, vector<string>>` 包一层 lambda，未知页面返回空列表。
- 每次点击成本 1；URL 按字符串原样比较（不做规范化），不限制 hostname（不能照搬 LC 1236 的同域规则）。
- `minClicks`：start == target 返回 0，不可达返回 -1。
- `shortestPath`：返回任意一条最短路径（含首尾），start == target 返回 `[start]`，不可达返回空。
- 每个页面最多调用一次 `getAllLinks`（测试里计数校验）；发现 target 即返回，不再抓取剩余页面。

**先问面试官：**
- 只返回点击数，还是也要实际路径？多条最短路返回哪条都行吗？
- 是否限制同 hostname？URL 是否需要规范化（末尾斜杠、`#fragment`、http/https、大小写）？
- start == target、不可达分别返回什么？
- `getAllLinks` 会失败 / 超时 / 很慢吗？失败是重试、跳过还是整体失败？是否需要限流或并发抓取？
- 页面规模多大，visited 集合能放进内存吗？

**思路：** 所有边权相同，BFS 按层扩展，第一次发现某页面时的层数就是到它的最少点击数。`dist`（或 `parent`）哈希表同时充当 visited，在入队时标记，保证每页只入队一次、只抓取一次，环和自链接不会死循环。在「发现」时就判断是否是 target，可以少抓取一整层页面（`getAllLinks` 是昂贵的网络调用）。路径版本记录每个页面第一次从哪个页面被发现，找到 target 后沿 parent 回溯再反转。

**复杂度：** V = 访问到的页面数，E = 这些页面上的链接总数。
- `minClicks`：T O(V+E) 次链接检查，`getAllLinks` 调用 ≤ V 次；S O(V)。
- `shortestPath`：T O(V+E)，S O(V)（parent 表 + 队列；路径长度 ≤ V）。
- 未计入字符串哈希的长度开销（URL 长度 L 时每次查表 O(L)）。

**Follow-up：**
- 已实现 `shortestPath`（parent 表回溯真实路径），对应「另要求实际路径」的来源版本。
- 双向 BFS：只有当能拿到反向链接（谁链接到 target）时才可用；一般网页只能正向抓取，要先说明。
- `getAllLinks` 慢 / 失败：同一层的页面可并发抓取（线程池 + 每层 barrier 保持 BFS 层序）；失败做有限次数的指数退避重试，最终失败的页面当作无出边并记录；按 hostname 限流。
- 规模太大：visited 改用 Bloom filter（会有误判漏抓）或分布式 KV；限制最大深度。
- 同域爬取（LC 1236）：入队前加 `hostOf(nxt) == hostOf(start)` 过滤。

**易错点：**
- 用 DFS：沿第一条链接 a→b→d→e→t 找到 4 次点击，而最短是 a→c→t 的 2 次；DFS 求最短要遍历所有路径。
- 出队时才标记 visited：同一页面可被多次入队、多次抓取（有环时队列膨胀）。
- 忘记处理 start == target：返回 1 或空路径，而不是 0 / `[start]`。
- 回溯时 `parent[start]` 没设好：用 `operator[]` 读不存在的键会插入空串并无限循环；用 start 指向自己作为终止条件。
