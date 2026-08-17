# 平衡树 BalancedTree

## Preface

平衡树是一种数据结构，它能在插入、删除等**操作后**保持树的**平衡**，从而保证各种操作的**时间复杂度**为 $O(\log n)$。常见的平衡树有**AVL树、红黑树、Splay树、Treap**等。

在C++中，除了手写平衡树，还可以使用标准库以外的库，例如 [**pbds**(Policy-Based Data Structures)](/viewer.html?file=%E5%BA%93%20Libs/%E6%89%A9%E5%B1%95%20EXT/__gnu_pbds/PBDS.md)。pbds是GNU扩展库中的一个部分，提供了多种基于策略的数据结构，其中就有平衡树。值得一提的是，GNU中还包含了一个平衡树数据结构——[**rope**](/viewer.html?file=%E5%BA%93%20Libs/%E6%89%A9%E5%B1%95%20EXT/__gnu_cxx/%E5%B9%B3%E8%A1%A1%E6%A0%91%20Rope.md)。

在竞赛中我们如果没有特殊需求，通常直接使用 pbds 和 rope。