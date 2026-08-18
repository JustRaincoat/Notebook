# 线段树 SegmentTree

## 问题

区间信息的维护与查询

## 写有哪些运算可以使用线段树来维护？

线段树是一种非常强大的数据结构，它的核心功能是高效地处理**区间查询**和**区间更新**操作。线段树能够维护的运算必须满足一个**关键性质：结合律**。

### 核心要求：运算满足结合律

对于一个二元运算 $\oplus$，它必须满足结合律：$(a \oplus b) \oplus c = a \oplus (b \oplus c)$

只有满足结合律的运算，我们才能将一个大区间 $[L,R]$ 的信息，通过将其分割成两个（或多个）子区间 $[L,M]$ 和 $[M+1,R]$，然后合并这两个子区间的结果 $Info_{left} \oplus Info_{right}$ 来高效地计算得到。线段树节点存储的就是其对应区间的运算结果。

### 常见的可以被线段树维护的运算 / 信息

1. **区间和 ($+$):**
    
   - 查询：`query(l, r) = a[l] + a[l+1] + ... + a[r]`
   - 更新：点更新（修改单个元素值），区间更新（给区间内每个元素加 / 减一个值 - 需要懒惰标记）
   - **满足结合律：**$(a + b) + c = a + (b + c)$

2. **区间最小值 ($\min$):**
    
   - 查询：`query(l, r) = min(a[l], a[l+1], ..., a[r])`
   - 更新：点更新（修改单个元素值），区间更新（将区间内每个元素设为某个值 - 需要懒惰标记）。注意：区间加 / 减一个值也能维护最小值（因为最小值是线性变化的）。
   - **满足结合律：**$\min ( \min (a, b), c) = \min (a, \min (b, c)) = \min (a, b, c)$

3. **区间最大值 ($\max$):**
    
   - 查询：`query(l, r) = max(a[l], a[l+1], ..., a[r])`
   - 更新：类似最小值（点更新，区间设值，区间加 / 减）。
   - **满足结合律：**$\max ( \max (a, b), c) = \max (a, \max (b, c)) = \max (a, b, c)$

4. **区间乘积 ($\times$):**
    
   - 查询：`query(l, r) = a[l] * a[l+1] * ... * a[r]`
   - 更新：点更新，区间乘一个值（需要懒惰标记）。注意零值需要特殊处理（如果元素可能为 0）。
   - **满足结合律：**$(a \times b) \times c = a \times (b \times c)$

5. **区间最大公约数 ($\gcd$):**
    
   - 查询：`query(l, r) = gcd(a[l], a[l+1], ..., a[r])`
   - 更新：点更新（修改单个元素值）。区间更新（如区间加）维护 GCD 比较复杂，通常结合差分或其他技巧。
   - **满足结合律：**$\gcd ( \gcd (a, b), c) = \gcd (a, \gcd (b, c)) = \gcd (a, b, c)$

6. **区间异或 (`xor` 或 `^`):**
    
   - 查询：`query(l, r) = a[l] ^ a[l+1] ^ ... ^ a[r]`
   - 更新：点更新，区间异或一个值（需要懒惰标记）。
   - **满足结合律：**`(a ^ b) ^ c = a ^ (b ^ c)`

7. **区间按位与 (`&`):**
    
   - 查询：`query(l, r) = a[l] & a[l+1] & ... & a[r]`
   - 更新：点更新，区间与一个值（需要懒惰标记，但逻辑可能复杂）。
   - **满足结合律：**`(a & b) & c = a & (b & c)`

8. **区间按位或 (`|`):**
    
   - 查询：`query(l, r) = a[l] | a[l+1] | ... | a[r]`
   - 更新：点更新，区间或一个值（需要懒惰标记，逻辑可能复杂）。
   - **满足结合律：**`(a | b) | c = a | (b | c)`

9. **区间最值差 (`max - min`):**
    
   - 查询：`query(l, r) = (max in [l, r]) - (min in [l, r])`
   - 更新：点更新（修改单个元素值），区间加 / 减（因为最值差在区间加 / 减下不变，只需更新 max 和 min）。
   - 需要同时维护区间最大值和最小值信息，查询时计算差值。结合律体现在如何合并子区间的 max 和 min 上。


10. **区间第 K 小 / 大数：**
    - 通常使用**权值线段树**或 **主席树（可持久化线段树）** 来实现。这需要离散化数据，并在每个节点存储其值域范围内元素出现的总次数。结合律体现在子区间计数值的加和上。

11. **矩阵运算 (广义结合律):**
     
    - 如果区间中的每个元素是一个矩阵，并且你关心的运算是矩阵乘法（或其他满足结合律的矩阵运算），那么线段树也可以维护区间矩阵乘积。这在处理动态线性变换问题时非常有用。
    - **满足结合律：**  矩阵乘法满足结合律 `(A * B) * C = A * (B * C)`。

12. **自定义满足结合律的运算：**
    - 只要你能定义一个二元运算 `⊕` 并证明它满足结合律，你就可以用线段树来维护这个运算在任意区间上的计算结果。例如：
      - 字符串连接（如果连接是需要的操作，但注意字符串长度增长很快，效率可能不高）。
      - 模意义下的乘法或加法。
      - 结构体（如记录最大连续子段和、区间总和、前缀最大和、后缀最大和等信息，通过定义 **合适的合并规则** 来维护区间最大连续子段和）。

## 思路

每个节点表示原数组的一个区间。

**懒标记**：在修改区间时，不直接更新叶子节点，而是在区间上进行暂存，查询时再把懒标记下放给叶子结点，避免大规模的数据存储与修改。

详见 [OI Wiki](https://oi-wiki.org/ds/seg/)

## 代码模板 —— Tag + Info + SegTree 解耦设计

为了最大化可扩展性，将线段树拆分为三个独立模块：

- **`Info`（信息）**：节点区间上的聚合信息（如区间和、最大值）。通过重载 `operator+` 定义**合并规则**（必须满足结合律）；通过 `apply(Tag)` 定义**懒标记作用到信息**的规则。
- **`Tag`（懒标记）**：暂存的修改。通过 `overlay(const Tag&)` 定义**标记叠加**规则（多个标记先后作用于同一节点时如何合并）。
- **`SegTree`（框架）**：只负责区间划分、`pushup`/`pushdown` 调度与动态开点，不关心维护的具体信息。更换题目时**只需重写 `Info` 与 `Tag`**，框架代码一字不动。

节点使用 **`std::deque<Node> pool` 内存池 + 指针** 动态开点。`deque` 的 `push_back` 不会使已有元素的指针失效，因此可以安全保存 `Node*`；相比 `new` 分配，避免了内存泄漏和大量小对象分配的开销。修改/查询区间通过成员变量 `ql, qr` 传入。

### Tag（以区间赋值为例）

```cpp
struct Tag{//懒标记：区间赋值
    int set = 0;bool setTag = false;//赋的值 & 是否有赋值标记
    void overlay(const Tag& t){set = t.set,setTag = true;}//标记叠加：后者覆盖前者
};
```

### Info（以区间和 + 区间最大值为例）

```cpp
struct Info{//节点信息：区间和 + 区间最大值
    int sum = 0,max = 0;
    Info operator+(Info x) const {return {sum + x.sum,std::max(max,x.max)};}//合并（满足结合律）
    void apply(Tag t){sum = t.set,max = t.set;}//懒标记作用到信息
};
```

### SegTree 框架

```cpp
struct SegTree{//动态开点线段树
    struct Node{
        const int l,r;//本节点管理的区间
        Tag tag;
        Info info;
        Node *ls = nullptr,*rs = nullptr;
        Node(int _l,int _r):l(_l),r(_r),tag{0,false},info{0,0}{};
    };
    const int n;
    int ql,qr;//本次操作的区间
    std::deque<Node> pool;//内存池
    Node* newNode(const int l,const int r){return pool.emplace_back(l,r),&pool.back();};
    Node *root;
    void apply(Node* p,const Tag TAG){//将懒标记作用到节点
        p->info.apply(TAG);
        p->tag.overlay(TAG);
    }
    void pushup(Node *p){p->info = (p->ls?p->ls->info:Info{0,0}) + (p->rs?p->rs->info:Info{0,0});}
    void pushdown(Node *p){//下传懒标记（必要时新建子节点）
        if(p->tag.setTag){
            if(!p->ls)p->ls = (pool.emplace_back(p->l, (p->l + p->r)/2),&pool.back());
            if(!p->rs)p->rs = (pool.emplace_back((p->l + p->r)/2+1,p->r),&pool.back());
            apply(p->ls,p->tag);
            apply(p->rs,p->tag);
            p->tag = Tag();
        }
    }
    SegTree(const int _n):n(_n),root(newNode(1,n)){};
    void update(Node* u,const Tag t){//区间修改
        if(ql <= u->l && u->r <= qr){apply(u,t);return;}
        pushdown(u);
        int m = (u->l + u->r)/2;
        if(ql <= m)update(u->ls = (u->ls?u->ls:newNode(u->l,m)),t);
        if(qr >  m)update(u->rs = (u->rs?u->rs:newNode(m+1,u->r)),t);
        pushup(u);
    }
    Info query(Node* u){//区间查询
        if(ql <= u->l && u->r <= qr)return u->info;
        pushdown(u);
        int m = (u->l + u->r)/2;
        if(qr <= m)return u->ls?query(u->ls):Info{};
        if(ql >  m)return u->rs?query(u->rs):Info{};
        return (u->ls?query(u->ls):Info{}) + (u->rs?query(u->rs):Info{});
    }
};
```

### 如何扩展：只换 Tag / Info

以「区间乘 + 区间加 + 区间求和」为例（P3373），`SegTree` 框架不变，仅把两个结构体换成：

```cpp
struct Tag{//懒标记：先乘 mul 再加 add
    int mul = 1,add = 0;
    void overlay(const Tag& t){//标记叠加：本标记先作用，t 后作用
        add = (add*t.mul + t.add)%p;
        mul = mul*t.mul%p;
    }
};
struct Info{
    int sum = 0;
    Info operator+(Info x) const {return {(sum + x.sum)%p};}
    void apply(Tag t,int len){sum = (sum*t.mul + t.add*len)%p;}//需要区间长度时把 len 一并传入
};
```

完整代码见例题 [P3373](#例题)。

### 动态开点的使用情景

| 情况 | 处理方法 | 说明 |
| :--- | :--- | :--- |
| **初始全为0** | **最简单** | 不存在的节点直接返回0，逻辑自然。 |
| **初始全为同一个值 `val`** | **修改 `pushdown` 逻辑** | 在创建新节点时，将其值初始化为 `val * len`，而不是0。 |
| **初始为一个（稀疏）数组** | **视情况通过 `update` 初始化** | 只为那些非默认值的元素创建节点。如果数组密集，则不适合用动态开点。 |
| **有值节点极多** | **不建议使用** | 退化成普通线段树 |

!!! tip 与树链剖分配合
    当 `Info` 同时维护多个量（如区间和、区间最大值）时，可以让树链剖分的 `query_on_path` 直接返回 `Info`，从而一次跳链同时完成多种聚合查询。详见「[重链剖分 HLD](/viewer.html?file=docs/图论 Graph/树 Tree/重链剖分 HLD/重链剖分 HLD.md)」中的 [P3313 [SDOI2014] 旅行](https://www.luogu.com.cn/problem/P3313) 例题。

## 例题

### [P3373 【模板】线段树 2](https://www.luogu.com.cn/problem/P3373)

区间乘、区间加、区间求和。`Tag` 需同时记录乘与加两种懒标记，`overlay` 时注意叠加顺序（先乘后加）。

[**参考代码**](/viewer.html?file=Code/P3373.cpp)

### [P13825 【模板】线段树 1.5](https://www.luogu.com.cn/problem/P13825)

1. [**Code**](/viewer.html?file=Code/P13825.old.cpp)
    使用数组写法。
2. [**Code**](/viewer.html?file=Code/P13825.cpp)
    使用指针优化写法。在保留原有代码的极高扩展性的情况下使代码时空效率更高。

## 应用

!!! tip 踩坑记录
    1. 记得判断输入区间是否合法，若没有声明 `l<r`，则默认进行交换操作。
    2. 不要滥用动态开点，新增节点会带来更多的时间复杂度。
    3. `Tag` 的 `overlay` 叠加顺序不可交换时（如先乘后加），务必写清楚「哪个标记先作用」。
