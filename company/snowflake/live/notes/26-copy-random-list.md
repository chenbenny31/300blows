## 26. 复制带随机指针的链表（I05 · LC 138）
**证据：** Intern；平台标签（Jobright J15194）；Virtual Onsite。P0。
**本文件合同：**
- `Node{val, next, random}`，`random` 可为空、可指向自己、可多个节点指向同一目标；值可以重复。
- 返回全新节点组成的深拷贝：拷贝中任何 `next` / `random` 都不指向原链表节点。
- 原链表在调用后必须与调用前完全相同（`copyRandomList2` 临时修改后恢复）。
- 空链表返回 `nullptr`；拷贝由调用方负责释放（测试里 `freeList`，保持 ASan 干净）。

**先问面试官：**
- random 能否为空 / 指向自己 / 指向同一个节点？值是否唯一（决定能否按值建 map：不能）？
- 能否临时修改原链表？若能，结束时必须恢复吗？
- 要求额外 O(1) 空间吗？
- 返回的内存归谁释放？

**思路：** 方法一：第一遍为每个原节点建拷贝，存 `原 → 拷贝` 的 map；第二遍用 map 翻译 `next` 和 `random`，键是节点指针不是值，所以重复值无影响。方法二（O(1) 额外空间）：把每个拷贝插在原节点后面形成 A→A'→B→B'；于是 `X.random` 的拷贝正是 `X.random->next`；最后一遍拆分，先恢复原节点的 `next`，再把拷贝的 `next` 接到下一个拷贝。

**复杂度：** n 为节点数。
- `copyRandomList`（hash map）：T O(n)，S O(n)。
- `copyRandomList2`（交织）：T O(n)，S O(1) 额外（不计输出）。

**Follow-up：**
- 已实现 `copyRandomList2`：交织法，并恢复原链表。
- 只读原链表 + O(1) 空间：做不到通用 O(1)（需要某种 原 → 拷贝 映射），说明交织法正是用临时改原链表换空间。
- 单遍 hash map：`getCopy(node)` 懒创建，一边走一边连 next 和 random。
- 图 / 树的深拷贝（LC 133）：同一个 `原 → 拷贝` map + DFS/BFS。

**易错点：**
- 按 `val` 建 map，重复值时 random 指错（LC 示例 3）。
- 交织法第二遍写成 `cur->next->random = cur->random`，拷贝的 random 指回原链表。
- 拆分时没恢复原链表，或最后一个拷贝的 `next` 访问 `nullptr->next`。
- 测试只比较值，不检查拷贝是否和原链表共享节点。
