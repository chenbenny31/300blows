## 14. 王位继承顺序（A11 · LC 1600）
**证据：** New Grad 平台标签（Phone Screen）；另有 IC1 二手变体（H01，两轮 coding）

**本文件合同：**
- 接口与 LC 1600 完全一致：构造传 `kingName`，`birth(parent, child)`、`death(name)`、`getInheritanceOrder()`。
- 输入合法：parent 一定已存在，名字全局唯一，不做校验。
- 死者仍留在树里，只在输出时跳过；死者之后出生的孩子照常挂在他下面（包括国王死后）。
- 继承顺序 = 从国王开始的先序遍历，孩子按出生顺序；`getInheritanceOrder2` 是显式栈版本，结果相同。

**先问面试官：**
- 构造时给不给国王？（IC1 变体不给：可约定第一次 `birth` 的 parent 成为国王，或者谁从未当过 child 谁是根）
- `birth` 遇到未知 parent、重复名字怎么办：忽略、返回 false 还是抛错？
- 死者的后代是否继续继承？死者还能 `birth` 吗？
- `getInheritanceOrder` 调用频率与 `birth/death` 相比如何？家族树最深多少（决定递归还是显式栈）？

**思路：** LC 1600 定义的 Successor 递归恰好等价于先序遍历：一个人之后先轮到他所有后代（按出生顺序逐个子树），整棵子树结束后才轮到他的下一个弟妹。所以只需维护「人 -> 按出生顺序的孩子」和死亡集合，查询时做一次先序 DFS 并跳过死者；死亡不改树结构，所以后代位置不变。

**复杂度：** N = 已出生总人数（含国王），H = 树高。
- `birth` / `death`：T O(1) 均摊，S 总计 O(N)。
- `getInheritanceOrder`：T O(N)，S O(N) 输出 + O(H) 递归栈。
- `getInheritanceOrder2`：T O(N)，S O(N)。

**Follow-up：**
- 已实现 `getInheritanceOrder2`（显式栈）：LC 允许 1e5 次 `birth`，一条长链会让递归深度到 1e5，可能栈溢出；孩子逆序入栈，长子先出栈。
- IC1 无国王变体：构造无参，第一次 `birth` 时若根为空则 `king = parent`；或在查询时取「出现过但从未作为 child」的人作为根。须先和面试官确认，并说明未知 parent 的处理（参考 ref 版本 `birth` 返回 bool）。
- 查询远多于修改：可缓存上次顺序，`birth/death` 时标脏；或维护先序链表在出生时插入到「父亲子树的末尾」，但找子树末尾本身要额外结构，面试里讲清代价即可。

**易错点：**
- 按出生时间排序输出：孙子 c 比叔叔 b 晚出生，但 c 必须排在 b 前面。
- 死亡时把节点从树里删掉或把孩子提到上一层：会打乱顺序，正确做法是只标记。
- DFS 里用 `children[person]` 读：每个叶子都会插入一个空 vector，改变了容器状态；用 `find`。
- 显式栈版本按正序入栈，导致最小的孩子先输出。
