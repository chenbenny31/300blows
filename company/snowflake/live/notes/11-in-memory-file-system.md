## 11. 内存文件系统（B07 · LC 588）
**证据：** 职级未确认；phone screen（C11、C19、J15724）；Phone Screen；P0。

**本文件合同：**
- 路径都是合法绝对路径：以 `/` 开头，除根 `"/"` 外无结尾 `/`，无 `//`、`.`、`..`（不做规范化）。
- `mkdir` 创建所有缺失的父目录，目录已存在则 no-op；`addContentToFile` 文件不存在就创建（连同缺失父目录），存在则**追加**。
- `ls(dir)` 返回子项名，按 `std::string` 字节序（`"B" < "a10" < "a2" < "b"`），文件与目录混排；`ls(file)` 返回 `{文件名}`。
- 缺失路径：`ls` 返回 `{}`，`readContentFromFile` 返回 `""`，且读操作绝不创建节点。
- 不处理文件 / 目录冲突（例如对文件路径 `mkdir` 或在文件下建子项），按 LC 保证不会出现。
- 内存：`Node*` 原始指针 + `FileSystem` 析构函数后序释放，ASan/LSan 无泄漏；对象不拷贝（未写拷贝构造，拷贝会 double free）。

**先问面试官：**
- 写文件是追加还是覆盖？读 / ls 不存在的路径返回什么（空、报错）？
- `ls` 的排序规则：纯字典序还是目录在前？大小写？
- 路径是否需要规范化（`.`、`..`、重复 `/`、结尾 `/`）？
- 文件与目录同名 / 对文件 `mkdir` 等冲突要不要报错？
- 是否需要 delete / mv、并发访问？

**思路：** 路径组件组成一棵 trie，每个节点 `map<name, Node*>`，`map` 有序所以 `ls` 直接遍历即可。四个操作共用一个 `walk(path, create)`：逐段 `find`，缺失时按 `create` 决定新建还是返回 `nullptr`；根路径 `"/"` 不进循环直接返回根。文件节点就是 `isFile = true` 的叶子，`ls(file)` 取最后一个 `/` 之后的部分。

**复杂度：** p = 路径组件数，c = 单个目录的子项数，len = 内容长度，N = 节点总数（组件名长度视为常数）。
- `mkdir`：T O(p log c)，S 最多新建 p 个节点。
- `ls`：T O(p log c + c)，S O(c) 输出。
- `addContentToFile` / `readContentFromFile`：T O(p log c + len)，S O(len)。
- 析构：T O(N)，S O(depth) 递归栈。总空间 O(N + 内容总长)。

**Follow-up：**
- **本文件未单独实现 method2**（设计题，主体即答案）。
- 为什么不用 `map<string, Node>` 按值存：标准只保证 `vector/list/forward_list` 支持不完整类型，`map<string, Node>` 放在 `Node` 里在 libstdc++/libc++ 上能编译但标准不保证；要零手动释放可用 `map<string, std::unique_ptr<Node>>`，或 `vector<Node>` 节点池 + `map<string, int>` 下标。现场选原始指针 + 析构函数最好打也最好解释。
- 加 `rm(path)`：找到父节点，`freeChildren(child)` 后 `delete child` 并从父 map erase。
- 文件 / 目录冲突：walk 中途遇到 `isFile` 节点就报错 / 返回 false；对已存在的目录 `addContentToFile` 报错。
- 路径规范化：先按 `/` split，用栈处理 `.` 和 `..`（LC71），再走 trie。
- 并发：全局读写锁最简单；细粒度可每个目录一把锁，按路径从根向下加锁避免死锁。

**易错点：**
- 读路径上用 `children[name]`，`ls("/nope")` 会悄悄建出节点。
- `ls(file)` 返回父目录内容或整条路径，而不是文件名本身。
- 根 `"/"`：split 出一个空名字 `""` 并建了一个空名子节点，导致 `ls("/")` 多出 `""`。
- 写成覆盖 `content = ...` 而不是追加；忘了 `isFile = true`；`new` 了节点却没有析构释放（LSan 报泄漏）。
