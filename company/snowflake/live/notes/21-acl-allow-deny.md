## 21. 多重继承 ACL：allow / deny（A18 · 无直接 LC；210、2192 为原型）
**证据：** IC1 二手摘编（PracHub）；其他版本含 IC4；两轮 coding；详细规则须澄清。P0 基础，P1 覆盖规则。
**本文件合同：**
- 输入 `parents[v]` = v 的父节点列表，节点 0..V-1；`allow[v]` / `deny[v]` 为本地权限（字符串）。
- `effective(v) = (∪ effective(p) ∪ allow[v]) − deny[v]`：只继承父节点的 **effective set**，不传播原始 deny。
- 子节点 deny 覆盖继承来的 allow；子节点本地 allow 可以重新授予父节点 deny 掉的权限。
- 同一节点同时 allow 与 deny 同一权限：deny 赢（deny 最后执行）。
- 多重继承中，任何一条父路径带来的权限都算（祖先在一条路径上 deny，另一条路径仍可带进来）。
- 有环（包括只有部分节点在环上）返回空 `{}`；每个节点的输出按字典序排序。

**先问面试官：**
- 继承的是父节点的最终 effective set，还是祖先的原始 allow/deny 一路传播（deny 是否“粘住”）？
- 子节点能否 re-grant 父节点 deny 的权限？同一节点 allow 与 deny 冲突谁赢？
- 有环怎么办：返回空、报错，还是把环当成一个整体？
- 输出顺序：排序、还是按插入顺序？权限数量规模（影响用 set 还是 bitset）？

**思路：** 每个节点的结果只依赖父节点的最终结果，所以按拓扑序计算：Kahn 中节点出队时所有父节点已定，先并父集合，再加本地 allow，最后删本地 deny。出队节点数 < V 说明有环。`effective2` 用 DFS 后序 + 三色（1 = 在当前路径上，遇到即环），计算顺序同样保证父先子后。

**复杂度：** V 节点数，E 父子边数，A 权限种类数，L 本地 allow/deny 总条数。
- `effective`（Kahn）：T O(V + E·A·logA + L·logA)，S O(V·A)。
- `effective2`（DFS）：同上，额外递归栈 O(V)。

**Follow-up：**
- **另一种合同“上游任意 deny 都赢”**（未实现）：每个节点传播两套集合 `allowUp(v) = ∪allowUp(p) ∪ allow[v]`、`denyUp(v) = ∪denyUp(p) ∪ deny[v]`，最终 `effective = allowUp − denyUp`。代码改动：`resolve` 维护两个 `set`，最后做差；子节点无法再 re-grant。本文件测试里节点 3、4 会因此失去 `write`。
- 环 / 非递归（IC4 原帖的扩展）：Kahn 本身就是非递归且自带环检测；DFS 版本链太深时改显式栈。
- 已实现 `effective2`：DFS 三色版本，作为另一种拓扑实现。
- 权限种类少（≤64）时可用 `unsigned long long` bitmask，合并变 O(1)。

**易错点：**
- 先删 deny 再并父集合，导致继承来的权限绕过 deny。
- 按节点编号顺序算，而不是拓扑序（测试 "parent has larger index"）。
- 只检查“全部在环上”的情况；环挂在合法根下面时也要返回空。
- 把“deny 粘住”和“只继承 effective”两种合同混着写：一条路径 deny、另一条路径 allow 时结果不同。
