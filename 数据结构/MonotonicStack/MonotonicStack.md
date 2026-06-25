---
export_on_save:
  html: true
---

# 单调栈

单调栈（Monotonic Stack）是一种栈内元素**单调递增**或**单调递减**的栈结构。用于解决**寻找下一个更大（或更小）元素**的一类问题，典型应用包括：柱状图中最大矩形、接雨水、每日温度等。

## 核心思想

单调栈的核心是**维护栈内元素的单调性**。当新元素入栈时，不断弹出破坏单调性的栈顶元素，从而在弹出过程中获取所需的"下一个更大（或更小）元素"信息。

- **单调递增栈**：栈底到栈顶元素递增（即栈顶最小），用于找**下一个更小元素**。
- **单调递减栈**：栈底到栈顶元素递减（即栈顶最大），用于找**下一个更大元素**。

---

## 算法流程

以 **单调递减栈**（找下一个更大元素）为例：

1. 遍历数组元素 $a_i$。
2. 当栈非空且 $a_i > \text{栈顶元素}$ 时，弹出栈顶：此时 $a_i$ 就是被弹出元素的**下一个更大元素**。
3. 将 $a_i$ 入栈。
4. 遍历结束后，栈中剩余元素没有下一个更大元素（通常处理为 $-1$）。

时间复杂度 $O(n)$，每个元素至多入栈一次、出栈一次。

### 单调递增栈

同理，找**下一个更小元素**时，维护栈内元素递增。当 $a_i < \text{栈顶元素}$ 时弹出栈顶，此时 $a_i$ 即为被弹出元素的**下一个更小元素**。

---

## 核心代码

```cpp
namespace MonotonicStack {
    int stk[maxn], top;   // stk 存下标
    int ans[maxn];        // ans[i] 表示 a[i] 的下一个更大元素的下标（不存在则为 -1）

    void solve(int a[], int n) {
        top = 0;
        for (int i = 1; i <= n; i++) {
            while (top && a[stk[top]] < a[i]) {
                ans[stk[top]] = i;
                top--;
            }
            stk[++top] = i;
        }
        while (top) {
            ans[stk[top]] = -1;
            top--;
        }
    }
}
```

### 常见变体

- 找**下一个更大元素**：递减栈，`a[stk[top]] < a[i]` 时弹出。
- 找**下一个更小元素**：递增栈，`a[stk[top]] > a[i]` 时弹出。
- 找**上一个更大元素**：递减栈，在 `while` 弹出后，栈顶即为上一个更大元素。
- 找**上一个更小元素**：递增栈，在 `while` 弹出后，栈顶即为上一个更小元素。

---

## 例题

### 例 1：[P5788 【模板】单调栈](https://www.luogu.com.cn/problem/P5788)

给定一个长度为 $n$ 的序列 $a$，求每个数后面第一个比它大的数的下标。

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 3e6 + 5;
int n, a[maxn], stk[maxn], top, ans[maxn];

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++) {
        while (top && a[stk[top]] < a[i]) {
            ans[stk[top]] = i;
            top--;
        }
        stk[++top] = i;
    }
    for (int i = 1; i <= n; i++) printf("%d ", ans[i]);
    return 0;
}
```

### 例 2：[P1904 天际线问题](https://www.luogu.com.cn/problem/P1904)

输入每个建筑的左右坐标和高度，输出城市天际线的关键点（轮廓线）。可以用单调栈维护当前的最高高度变化。

（代码略，思路见上）

### 例 3：[SP1805 HISTOGRA - Largest Rectangle in a Histogram](https://www.luogu.com.cn/problem/SP1805)

在柱状图中找到最大的矩形面积。利用单调栈计算每个柱子左右两侧第一个比它矮的位置。

```cpp
#include <bits/stdc++.h>
#define int long long
using namespace std;
const int maxn = 1e5 + 5;
int n, h[maxn], l[maxn], r[maxn], stk[maxn], top;

signed main() {
    while (scanf("%lld", &n), n) {
        for (int i = 1; i <= n; i++) scanf("%lld", &h[i]);
        // 找左边第一个更小的位置
        top = 0;
        for (int i = 1; i <= n; i++) {
            while (top && h[stk[top]] >= h[i]) top--;
            l[i] = top ? stk[top] : 0;
            stk[++top] = i;
        }
        // 找右边第一个更小的位置
        top = 0;
        for (int i = n; i >= 1; i--) {
            while (top && h[stk[top]] >= h[i]) top--;
            r[i] = top ? stk[top] : n + 1;
            stk[++top] = i;
        }
        int ans = 0;
        for (int i = 1; i <= n; i++)
            ans = max(ans, h[i] * (r[i] - l[i] - 1));
        printf("%lld\n", ans);
    }
    return 0;
}
```

---

## 总结

| 需求                     | 栈类型     | 判断条件               |
| ------------------------ | ---------- | ---------------------- |
| 下一个更大元素           | 递减栈     | `a[top] < a[i]` 弹出  |
| 下一个更小元素           | 递增栈     | `a[top] > a[i]` 弹出  |
| 上一个更大元素           | 递减栈     | 弹出后栈顶即为答案     |
| 上一个更小元素           | 递增栈     | 弹出后栈顶即为答案     |

单调栈将 $O(n^2)$ 的暴力枚举优化为 $O(n)$ 的线性扫描，是处理**相邻大小关系**问题的利器。
