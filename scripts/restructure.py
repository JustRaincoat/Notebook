#!/usr/bin/env python3
"""
Notebook 全站重构脚本
1. git mv 重命名所有 md（中文 英文 格式，目录+文件同名）
2. git mv 重命名 Code/*.cpp（Luogu_P_xxx_xxx.cpp -> Pxxx.cpp）
3. 重写所有 md 内站内链接 -> /viewer.html?file=<urlencoded 相对 docs 路径>
4. 重写 mkdocs.yml nav 路径

用法: python3 scripts/restructure.py [--dry-run]
"""
import os
import re
import sys
import posixpath
import urllib.parse
import subprocess

ROOT = '/srv/notebook'
DOCS = os.path.join(ROOT, 'docs')

DRY = '--dry-run' in sys.argv

# ---------- 1. md 文件映射: 旧相对 docs 路径 -> 新相对 docs 路径 ----------
FILE_MAP = {
    'BA&CT/BinarySearch/BinarySearch.md': '基础算法与常用技巧 BA&CT/二分搜索 BinarySearch/二分搜索 BinarySearch.md',
    'BA&CT/RegretGreedy/RegretGreedy.md': '基础算法与常用技巧 BA&CT/反悔贪心 RegretGreedy/反悔贪心 RegretGreedy.md',
    'Libs/STL/STL.md': '库 Libs/标准模板库 STL/标准模板库 STL.md',
    'Libs/EXT/__gnu_cxx/cxx.md': '库 Libs/扩展 EXT/__gnu_cxx/平衡树 Rope.md',
    'Libs/EXT/__gnu_pbds/pbds.md': '库 Libs/扩展 EXT/__gnu_pbds/PBDS.md',
    '数据结构/BalancedTree/BlancedTree.md': '数据结构 DataStructure/平衡树 BalancedTree/平衡树 BalancedTree.md',
    '数据结构/DSU/DSU.md': '数据结构 DataStructure/并查集 DSU/并查集 DSU.md',
    '数据结构/Matrix/Matrix.md': '数据结构 DataStructure/矩阵 Matrix/矩阵 Matrix.md',
    '数据结构/MonotonicQueue/MonotonicQueue.md': '数据结构 DataStructure/单调队列 MonotonicQueue/单调队列 MonotonicQueue.md',
    '数据结构/MonotonicStack/MonotonicStack.md': '数据结构 DataStructure/单调栈 MonotonicStack/单调栈 MonotonicStack.md',
    '数据结构/SegmentTree/SegmentTree.md': '数据结构 DataStructure/线段树 SegmentTree/线段树 SegmentTree.md',
    '数据结构/SparseTable/SparseTable.md': '数据结构 DataStructure/稀疏表 SparseTable/稀疏表 SparseTable.md',
    '数据结构/TrieTree/Trie.md': '数据结构 DataStructure/字典树 TrieTree/字典树 TrieTree.md',
    '图论/Connectivity/DCC/DCC.md': '图论 Graph/连通性 Connectivity/双连通分量 DCC/双连通分量 DCC.md',
    '图论/Connectivity/SCC/SCC.md': '图论 Graph/连通性 Connectivity/强连通分量 SCC/强连通分量 SCC.md',
    '图论/DiffConstraints/DC.md': '图论 Graph/差分约束 DiffConstraints/差分约束 DiffConstraints.md',
    '图论/H&E Path/Euler Path/Euler Path.md': '图论 Graph/欧拉与哈密顿路径 H&E Path/欧拉路径 Euler Path/欧拉路径 Euler Path.md',
    '图论/H&E Path/Hamiltonian Path/Hamiltonian Path.md': '图论 Graph/欧拉与哈密顿路径 H&E Path/哈密顿路径 Hamiltonian Path/哈密顿路径 Hamiltonian Path.md',
    '图论/ShortestPath/ShortestPath.md': '图论 Graph/最短路 ShortestPath/最短路 ShortestPath.md',
    '图论/Tree/Heavy-Light Decoposition/HLD.md': '图论 Graph/树 Tree/重链剖分 HLD/重链剖分 HLD.md',
    '图论/Tree/LCA/LCA.md': '图论 Graph/树 Tree/最近公共祖先 LCA/最近公共祖先 LCA.md',
    '图论/Tree/MST/MST.md': '图论 Graph/树 Tree/最小生成树 MST/最小生成树 MST.md',
    '图论/Tree/TreeDiameter/TD.md': '图论 Graph/树 Tree/树的直径 TreeDiameter/树的直径 TreeDiameter.md',
    '数论/丢番图方程/IndeterminateEquation.md': '数论 NumberTheory/丢番图方程 IndeterminateEquation/丢番图方程 IndeterminateEquation.md',
    '数论/位运算/BitOperator.md': '数论 NumberTheory/位运算 BitOperator/位运算 BitOperator.md',
    '数论/质因数分解/PrimeFactorization.md': '数论 NumberTheory/质因数分解 PrimeFactorization/质因数分解 PrimeFactorization.md',
    '数论/逆元/Inverse.md': '数论 NumberTheory/逆元 Inverse/逆元 Inverse.md',
    '刷题笔记/P10449 费解的开关/P10449.md': '刷题笔记 ProblemSet/费解的开关 P10449/费解的开关 P10449.md',
    '刷题笔记/P1450 [HAOI2008] 硬币购物/P1450.md': '刷题笔记 ProblemSet/硬币购物 P1450/硬币购物 P1450.md',
    '刷题笔记/P1553 数字反转（升级版）/P1553.md': '刷题笔记 ProblemSet/数字反转 P1553/数字反转 P1553.md',
    '刷题笔记/P1593 因子和/P1593.md': '刷题笔记 ProblemSet/因子和 P1593/因子和 P1593.md',
    '刷题笔记/P1879 [USACO06NOV] Corn Fields G/P1879.md': '刷题笔记 ProblemSet/玉米田 Corn Fields P1879/玉米田 Corn Fields P1879.md',
    '刷题笔记/P2114 [NOI2014] 起床困难综合症/P2114.md': '刷题笔记 ProblemSet/起床困难综合症 P2114/起床困难综合症 P2114.md',
    '刷题笔记/P2827 [NOIP 2016 提高组] 蚯蚓/P2827.md': '刷题笔记 ProblemSet/蚯蚓 P2827/蚯蚓 P2827.md',
    '刷题笔记/P7077 [CSP-S 2020] 函数调用/P7077.md': '刷题笔记 ProblemSet/函数调用 P7077/函数调用 P7077.md',
    'index.md': 'index.md',
    # index.md 只随目录移动，文件名保留
    'BA&CT/index.md': '基础算法与常用技巧 BA&CT/index.md',
    '图论/index.md': '图论 Graph/index.md',
    '数据结构/index.md': '数据结构 DataStructure/index.md',
}

# ---------- 2. cpp 映射: 旧文件名 -> 新文件名 ----------
CPP_MAP = {
    'Luogu_P_10449_费解的开关.cpp': 'P10449.cpp',
    'Luogu_P_13825_模板_线段树_1_5.cpp': 'P13825.cpp',
    'Luogu_P_13825_模板_线段树_1_5.old.cpp': 'P13825.old.cpp',
    'Luogu_P_1450_HAOI_2008_硬币购物.cpp': 'P1450.cpp',
    'Luogu_P_1553_数字反转_升级版.cpp': 'P1553.cpp',
    'Luogu_P_1593_因子和.cpp': 'P1593.cpp',
    'Luogu_P_2114_NOI_2014_起床困难综合症.cpp': 'P2114.cpp',
    'Luogu_P_2146_NOI_2015_软件包管理器.cpp': 'P2146.cpp',
    'Luogu_P_3369_模板_普通平衡树.cpp': 'P3369.cpp',
    'Luogu_P_3391_模板_文艺平衡树.cpp': 'P3391.cpp',
    'Luogu_P_3976_TJOI_2015_旅游.cpp': 'P3976.cpp',
    'Luogu_P_4315_月下_毛景树.cpp': 'P4315.cpp',
    'Luogu_P_7077_CSP_S_2020_函数调用.cpp': 'P7077.cpp',
    'P_2827.cpp': 'P2827.cpp',
}

# ---------- 3. 特殊悬空链接映射 ----------
SPECIAL_MAP = {
    '/gnu_cxx/PBDS/pbds/': '库 Libs/扩展 EXT/__gnu_pbds/PBDS.md',
    '/gnu_cxx/ROPE/rope/': '库 Libs/扩展 EXT/__gnu_cxx/平衡树 Rope.md',
}

# ---------- 4. 工具 ----------
def sh(cmd):
    if DRY:
        print('[dry]', cmd)
        return
    r = subprocess.run(cmd, shell=True, cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0:
        print(f'!! FAIL: {cmd}\n{r.stderr}')
        sys.exit(1)

def git_mv(old, new):
    """git mv 单个文件（自动建目录）"""
    os.makedirs(os.path.dirname(new), exist_ok=True)
    sh(f'git mv -- "{old}" "{new}"')

def build_old_index_map():
    """新旧路径 -> 新md路径 查找表"""
    m = {}
    for old, new in FILE_MAP.items():
        old_stem = old[:-3]  # 去 .md
        new_stem = new[:-3]
        m[old] = new
        m[new] = new
        m[old_stem] = new
        m[new_stem] = new
        m['/' + old_stem] = new
        m['/' + old_stem + '/'] = new
        m['/' + new_stem] = new
        m['/' + new_stem + '/'] = new
    return m

OLD_INDEX = build_old_index_map()

def resolve_link(href, cur_dir):
    """把 md 内的站内链接解析成 新md相对docs路径；无法解析返回 None"""
    href = href.strip()
    if href.startswith('#'):
        return None  # 锚点，保留
    if '://' in href or href.startswith('mailto:') or href.startswith('javascript:'):
        return None
    # viewer 链接（cpp）
    mm = re.match(r'^/Code/viewer\.html\?file=([^)]+)$', href)
    if mm:
        f = urllib.parse.unquote(mm.group(1))
        if f in CPP_MAP:
            return 'Code/' + CPP_MAP[f]
        return None
    mm = re.match(r'^viewer\.html\?file=([^)]+)$', href)
    if mm:
        f = urllib.parse.unquote(mm.group(1))
        if f in CPP_MAP:
            return 'Code/' + CPP_MAP[f]
        return None
    if href in SPECIAL_MAP:
        return SPECIAL_MAP[href]
    if href == '/':
        return None  # 首页，保留
    if href.startswith('/'):
        # 绝对路径：URL 解码后查表
        p = urllib.parse.unquote(href).strip('/')
        cands = [p, p + '.md', p + '/index.md']
        for c in cands:
            if c in OLD_INDEX:
                return OLD_INDEX[c]
        # 目录形式 /dir/file/ -> dir/file
        for c in cands:
            if c in OLD_INDEX:
                return OLD_INDEX[c]
        return None
    # 相对路径
    p = urllib.parse.unquote(href).split('#')[0].split('?')[0]
    full = posixpath.normpath(posixpath.join(cur_dir, p))
    cands = [full, full + '.md', full + '/index.md']
    for c in cands:
        if c in OLD_INDEX:
            return OLD_INDEX[c]
    return None

def viewer_link(new_rel):
    return '/viewer.html?file=' + urllib.parse.quote(new_rel, safe='/')

LINK_RE = re.compile(r'(!?)\[([^\]]*)\]\(([^)]*)\)')

def rewrite_md(path, cur_dir):
    """重写单个 md 的链接，返回改动数"""
    with open(path, encoding='utf-8') as f:
        lines = f.readlines()
    out = []
    in_fence = False
    n_changed = 0
    for line in lines:
        if line.strip().startswith('```'):
            in_fence = not in_fence
            out.append(line)
            continue
        if in_fence:
            out.append(line)
            continue
        def repl(m):
            nonlocal n_changed
            bang, text, href = m.group(1), m.group(2), m.group(3)
            if bang == '!':
                return m.group(0)  # 图片
            target = resolve_link(href, cur_dir)
            if target is None:
                return m.group(0)
            n_changed += 1
            return f'[{text}]({viewer_link(target)})'
        out.append(LINK_RE.sub(repl, line))
    if n_changed:
        with open(path, 'w', encoding='utf-8') as f:
            f.writelines(out)
    return n_changed

def rewrite_nav():
    """重写 mkdocs.yml 里的 nav 路径，返回改动数"""
    yml = os.path.join(ROOT, 'mkdocs.yml')
    with open(yml, encoding='utf-8') as f:
        lines = f.readlines()
    out = []
    n = 0
    for line in lines:
        if '.md' in line and ': ' in line:
            head, _, tail = line.rpartition(': ')
            key = tail.strip()
            if key.endswith('.md') and key in FILE_MAP:
                line = head + ': ' + FILE_MAP[key] + '\n'
                n += 1
        out.append(line)
    with open(yml, 'w', encoding='utf-8') as f:
        f.writelines(out)
    return n

def main():
    # A. git mv md 文件
    print('==> [1/4] 重命名 md 文件')
    for old, new in sorted(FILE_MAP.items()):
        oldp = os.path.join(DOCS, old)
        newp = os.path.join(DOCS, new)
        if not os.path.exists(oldp):
            print(f'  !! 不存在: {oldp}')
            continue
        if os.path.exists(newp):
            print(f'  !! 目标已存在: {newp}')
            continue
        git_mv(oldp, newp)
        print(f'  mv {old} -> {new}')

    # B. git mv cpp 文件
    print('==> [2/4] 重命名 Code/*.cpp')
    for old, new in sorted(CPP_MAP.items()):
        oldp = os.path.join(ROOT, 'Code', old)
        newp = os.path.join(ROOT, 'Code', new)
        if not os.path.exists(oldp):
            print(f'  !! 不存在: {oldp}')
            continue
        git_mv(oldp, newp)
        print(f'  mv {old} -> {new}')

    # C. 重写 md 链接（在新路径上）
    print('==> [3/4] 重写 md 内链接')
    total = 0
    for new in sorted(FILE_MAP.values()):
        p = os.path.join(DOCS, new)
        cur_dir = os.path.dirname(new)
        if not os.path.exists(p):
            print(f'  !! 缺失: {p}')
            continue
        n = rewrite_md(p, cur_dir)
        if n:
            print(f'  {new}: {n} 处链接')
        total += n
    print(f'  共改写 {total} 处链接')

    # D. 重写 nav
    print('==> [4/4] 重写 mkdocs.yml nav')
    n = rewrite_nav()
    print(f'  nav 更新 {n} 处')

    print('完成。')

if __name__ == '__main__':
    main()
