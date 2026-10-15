## 25. Meeting Rooms II 与巴士复用（I01 · LC 253；252 为单房间判断原型）
**证据：** Intern 平台标签（Jobright J16976）；巴士追问来自地里 C06，职级不明；Phone Screen。P0。
**本文件合同：**
- 会议为半开区间 `[start, end)`：首尾相接（end == start）不冲突，房间可立即复用；输入可无序、可为空。
- `canAttendAll`：一间房能否容纳全部；`minRooms` / `minRooms2`：最少房间数。
- 巴士：trip = `{startTime, endTime, fromStation, toStation}`，要求 `endTime > startTime`（时长为正）。
- 一辆车能接 trip，当且仅当它在 `startTime` 之前（含等于）已空闲停在 `fromStation`；无周转时间。
- 不允许空车调度（deadhead）；新车可以在任意车站凭空投入，`minBuses` 返回总车数。

**先问面试官：**
- 区间开闭：end == start 是否冲突？零长度会议？
- 巴士：到达与同刻发车能否衔接？有无周转时间？
- 允许空车开到别的站吗？车最初停在哪里（任意站 / 固定车库）？
- 只要数量，还是要输出每辆车的 trip 分配？

**思路：** 房间：按开始时间排序，最小堆存每间房当前会议的结束时间；堆顶 `<= start` 说明最早空出的房可复用，否则加房；堆只在加房时变大，最终大小即峰值。sweep 版本把开始记 +1、结束记 -1，同一时刻 -1 排在 +1 前（pair 自然排序），运行和的峰值就是答案。巴士：车只在同站之间可互换，所以每个车站一个最小堆存“车在此站空闲的时刻”；按出发时间处理 trip，本站有已空闲的车就复用，否则新增一辆，然后把车按 `endTime` 放进 `toStation` 的堆。时长为正保证所有能在 `startTime` 前到站的车都来自更早出发、已处理的 trip；同站空闲车完全等价，所以贪心复用不会更差，新增车数等于每站“发车减到达”的峰值缺口之和，这正是下界。

**复杂度：** N 为会议或 trip 数。
- `canAttendAll`：T O(NlogN)，S O(N)（排序副本）。
- `minRooms`（堆）：T O(NlogN)，S O(N)。
- `minRooms2`（sweep）：T O(NlogN)，S O(N)。
- `minBuses`：T O(NlogN)，S O(N)。

**Follow-up：**
- 已实现 `minRooms2`（sweep）与 `minBuses`（按站复用的巴士版）。
- 巴士的 sweep 写法（未实现）：每站收集 `(start, +1 发车)` 与 `(end, -1 到达)`，排序后求运行和峰值，各站峰值相加即答案。
- 周转时间 g：把到站空闲时刻改成 `end + g`。
- 允许空车调度：变成带时间窗的最小路径覆盖（二分图匹配），不再是简单贪心，说清楚即可。
- 输出房间分配：堆里存 `(end, roomId)`。

**易错点：**
- 用 `<` 而不是 `<=` 判断复用，首尾相接被当成冲突。
- sweep 里同一时刻 +1 排在 -1 前，峰值多算 1（测试 "end and two starts at 3"）。
- 巴士只用一个全局堆，忽略了车停在哪个站（测试 "wrong station"）。
- trip 未排序就处理，或允许零时长 trip 导致同刻依赖顺序错误。
