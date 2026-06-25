---
export_on_save:
  html: true
---

# 单调队列

单调队列（Monotonic Queue）是一种队列内元素**单调递增**或**单调递减**的队列结构。主要用于解决**滑动窗口最值**问题，以及优化某些 DP 转移（如斜率优化中可转化为单调队列的形式）。

## 核心思想

单调队列的核心是使用 **双端队列**（deque）维护一个单调的候选值序列。当窗口滑动时：

1. **淘汰过期元素**：队首元素如果不在当前窗口内，则弹出。
2. **维护单调性**：新元素入队时，不断弹出队尾破坏单调性的元素，再将新元素入队。
3. **获取答案**：队首元素即为当前窗口的最值。

时间复杂度 $O(n)$，每个元素至多入队一次、出队一次。

- **单调递增队列**：队首到队尾递增（队首最小），用于求滑动窗口**最小值**。
- **单调递减队列**：队首到队尾递减（队首最大），用于求滑动窗口**最大值**。

---

## 算法流程

以 **单调递减队列**（求滑动窗口最大值）为例：

1. 初始化一个空的双端队列（存下标）。
2. 从左到右遍历数组 $a_i$：
   - 若队首下标 $< i - k + 1$（窗口左边界），弹出队首。
   - 当队非空且 $a_i > \text{队尾元素}$ 时，弹出队尾。
   - 将下标 $i$ 入队。
   - 当 $i \ge k$ 时，队首下标对应的值即为当前窗口最大值。
3. 遍历结束。

---

## 核心代码

```cpp
namespace MonotonicQueue {
    int q[maxn], head, tail;  // 模拟双端队列，存下标
    int ans[maxn];            // ans[i] 表示以 i 为右端点的窗口最值

    // 求滑动窗口最小值（单调递增队列）
    void minWindow(int a[], int n, int k) {
        head = 1, tail = 0;
        for (int i = 1; i <= n; i++) {
            // 淘汰队首过期元素
            while (head <= tail && q[head] <= i - k) head++;
            // 维护单调递增
            while (head <= tail && a[q[tail]] > a[i]) tail--;
            q[++tail] = i;
            if (i >= k) ans[i] = a[q[head]];
        }
    }

    // 求滑动窗口最大值（单调递减队列）
    void maxWindow(int a[], int n, int k) {
        head = 1, tail = 0;
        for (int i = 1; i <= n; i++) {
            while (head <= tail && q[head] <= i - k) head++;
            while (head <= tail && a[q[tail]] < a[i]) tail--;
            q[++tail] = i;
            if (i >= k) ans[i] = a[q[head]];
        }
    }
}
```

---

## 例题

### 例 1：[P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)

给定长度为 $n$ 的序列和大小为 $k$ 的滑动窗口，求所有滑动窗口的最小值和最大值。

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e6 + 5;
int n, k, a[maxn], q[maxn], head, tail;

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

    // 最小值（单调递增队列）
    head = 1, tail = 0;
    for (int i = 1; i <= n; i++) {
        while (head <= tail && q[head] <= i - k) head++;
        while (head <= tail && a[q[tail]] >= a[i]) tail--;
        q[++tail] = i;
        if (i >= k) printf("%d ", a[q[head]]);
    }
    printf("\n");

    // 最大值（单调递减队列）
    head = 1, tail = 0;
    for (int i = 1; i <= n; i++) {
        while (head <= tail && q[head] <= i - k) head++;
        while (head <= tail && a[q[tail]] <= a[i]) tail--;
        q[++tail] = i;
        if (i >= k) printf("%d ", a[q[head]]);
    }
    return 0;
}
```

### 例 2：[P1714 切蛋糕](https://www.luogu.com.cn/problem/P1714)

给定长度为 $n$ 的序列，找长度不超过 $m$ 的最大子段和。利用前缀和转化为滑动窗口最小值问题。

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 5e5 + 5;
int n, m, a[maxn], sum[maxn], q[maxn], head, tail, ans = -2e9;

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        sum[i] = sum[i - 1] + a[i];
    }
    head = 1, tail = 0;
    q[++tail] = 0;
    for (int i = 1; i <= n; i++) {
        while (head <= tail && q[head] < i - m) head++;
        ans = max(ans, sum[i] - sum[q[head]]);
        while (head <= tail && sum[q[tail]] >= sum[i]) tail--;
        q[++tail] = i;
    }
    printf("%d\n", ans);
    return 0;
}
```

---

## 单调栈 vs 单调队列

|          | 单调栈                     | 单调队列                       |
| -------- | -------------------------- | ------------------------------ |
| 数据结构 | 栈（单端）                 | 双端队列                       |
| 典型问题 | 下一个更大/更小元素        | 滑动窗口最值                   |
| 淘汰机制 | 栈顶弹出（破坏单调性）     | 队首淘汰（过期），队尾弹出     |
| 时间复杂度 | $O(n)$                   | $O(n)$                         |

两者核心思想相同——**维护单调性，剔除冗余候选**，区别在于栈只在一端操作，队列两端都需要操作。

---

## 总结

单调队列是一种高效的**在线**处理滑动窗口最值问题的工具，也常用于优化 DP。核心要点：

1. **队首淘汰**：移除超出窗口范围的元素。
2. **队尾维护**：新元素入队时弹出破坏单调性的元素。
3. **队首即答案**：窗口移动后，队首元素即为当前窗口的最值。

牢记这三点，即可灵活运用单调队列解决各类问题。
