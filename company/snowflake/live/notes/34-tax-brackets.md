## 34. 分段税档计算（B22 · LC 2303）
**证据：** 职级未确认；平台有 Phone Screen / Virtual Onsite（P0）
**本文件合同：**
- `brackets` 非空，`[upper, percent]` 按 upper 严格递增，第一档下界为 0；`0 <= income <= INT_MAX`，percent 为 0..100 的整数。
- 收入落在 `(upper_{i-1}, upper_i]` 的部分按 `percent_i` 计税；恰好等于某个 upper 的收入完全属于该档。
- **超过最高档：由构造参数 `taxAboveTop` 决定**——`false` 忽略（不计税，LC 2303 保证不会出现这种输入）、`true` 按最高档税率继续计。两种都测。
- 用 `long long` 存"税额 × 100"（`slice * percent`），最后只除一次 100.0 返回 `double`；不做四舍五入到分。
- 封装成 `TaxCalculator` 类：构造时存档位并预计算前缀税额，之后可多次查询。

**先问面试官：**
- 收入超过最高档 upper 时：不计税、按最高档税率、还是报错？
- 返回 `double` 即可，还是要精确到分（返回 cents 的整数 / 规定舍入）？
- 档位是否已排序、upper 是否严格递增？税率是整数百分比吗？
- 是否会对同一组档位做大量查询（决定要不要预处理 + 二分）？

**思路：** 逐档遍历，每档只对 `min(income, upper_i) - prevUpper` 这一段计税，收入用完就停；超过最高档的部分按合同处理。整数"百分比 × 金额"本身就是"税 × 100"，在 `long long` 中精确累加，避免浮点逐段误差。`calculateTax2` 预先算出收入恰好等于每个 upper 时的累计税额 `prefix`，查询时 `lower_bound` 找第一个 `upper >= income` 的档，答案 = 前一档的 prefix + 本档内的零头。

**复杂度：** B = 档位数。`calculateTax`：T O(B)，S O(1)。`calculateTax2`：构造 O(B)，每次查询 T O(log B)，S O(B)。

**Follow-up：**
- 大量查询（已实现：`calculateTax2`，前缀和 + 二分）。
- 超过最高档的两种规则（已实现：构造参数 `taxAboveTop`）。
- 精确到分的舍入：返回 `long long` cents，`(hundredths + 50) / 100` 之类，需先和面试官定舍入规则（四舍五入 / 银行家舍入）。
- 反向问题：给目标税额求最低收入——在 prefix 上二分找档，再在档内解一次线性方程。
- 动态增删档位：用 `std::map<upper, percent>`，前缀和改为每次查询时线性累加或用 Fenwick 树。

**易错点：**
- `slice * percent` 用 `int` 相乘溢出（收入接近 2e9 时 × 100）。
- 每档都用 `upper_i - upper_{i-1}` 而没和 income 取 min，收入在档内时多算。
- 收入用完后没停，`min(income, upper) - prevUpper` 变成负数被累加。
- 逐段 `slice * percent / 100.0` 累加浮点，测试用 `==` 比较时出现 `2.6499999`。
