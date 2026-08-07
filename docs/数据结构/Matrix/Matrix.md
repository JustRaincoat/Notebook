---
export_on_save:
  html: true
---

# 矩阵

用于快速进行**递推**，常与快速幂结合使用。

矩阵作为一种存储向量的方式，是一种必须的数据结构。

## Code

**注意（重载输入输出流时）：** 要记得返回 `istream` 或 `ostream`，并且仔细思考，哪些需要**引用传参**，哪些**不能使用引用传参**。

```cpp
struct Matrix{
    int n,m;
    std::vector<std::vector<int>> info;
    std::vector<int>& operator[](const int& k){return info[k];}
    const std::vector<int>& operator[](const int& k) const {return info[k];}
    Matrix(const int& _n,const int& _m,const int& a,const int& b){//a = 全局，b = 主对角线
        n = _n,m = _m;
        info.resize(n,std::vector<int>(m,a));//info初始化 + 全局赋值
        for(int i=0;i<std::min(n,m);i++)info[i][i] = b;//主对角线赋值
    }
    friend Matrix operator *(const Matrix& a,const Matrix& b){
        if(a.m != b.n)throw std::runtime_error("矩阵必须可乘");
        Matrix res(a.n,b.m,0,0);
        for(int i=0;i<a.n;i++)
            for(int k=0;k<a.m;k++)
                if(a[i][k] != 0)for(int j=0;j<b.m;j++)res[i][j] += a[i][k] * b[k][j],res[i][j] %= mod;
        return res;
    }
    friend Matrix operator^(Matrix mat,int p){
        if(mat.m != mat.n)throw std::runtime_error("矩阵必须可幂");
        Matrix ret(mat.n,mat.n,0,1);
        for(;p;p>>=1,mat = mat * mat)if(p&1)ret = ret * mat;
        return ret;
    }
};
```


## 矩阵快速幂

[P3390 【模板】矩阵快速幂](https://www.luogu.com.cn/problem/P3390)

[Code](/Code/P3390.cpp)

## 矩阵加速递推

[P1939 矩阵加速（数列）](https://www.luogu.com.cn/problem/P1939)

**通过这道题看懂矩阵快速幂的应用**

1. 观察题目给出的式子
   
$$
a_x=
\begin{cases}
 1 & x \in\{1,2,3\}\\ 
 a_{x-1}+a_{x-3} & x \geq 4
\end{cases}
$$

2. 设计出关于 $a_x$ 的矩阵

$$
\begin{bmatrix} a_x\\a_{x-1}\\a_{x-2} \end{bmatrix}
$$

3. 设计转移矩阵:**列表看系数**
   |  | $a_{x-1}$ | $a_{x-2}$ | $a_{x-3}$ |
   |--|----|----|----|
   | $a_x$ | $1$ | $0$ | $1$ |
   | $a_{x-1}$ | $1$ | $0$ | $0$ |
   | $a_{x-2}$ | $0$ | $1$ | $0$ |

    **Explain**$$
    a_{x} = 1 \times a_{x-1} + 0 \times a_{x-2} + 1 \times a_{x-3}$$ $$a_{x-1} = 1 \times a_{x-1} + 0 \times a_{x-2} + 0 \times a_{x-3}$$ $$a_{x-2} = 0 \times a_{x-1} + 1 \times a_{x-2} + 0 \times a_{x-3}$$

把系数填到表里我们就得到了**转移矩阵**

$$
\begin{bmatrix} 1 & 0 & 1 \\ 1 & 0 & 0 \\ 0 & 1 & 0 \end{bmatrix}
$$

[Code](/Code/P1939.cpp)

## 矩阵加速dp

这类题目通常有明显的标志，如 **数量级达到 $1 \times 10^{15}$**，这时候我们通常需要一个 $O(\log n)$ 的算法。于是如果 **恰好DP转移是一维齐次式**，那么我们就可以使用矩阵快速幂优化DP。其实DP就是利用递推完成的，状态设计可以参考[矩阵加速递推](#矩阵加速递推)。