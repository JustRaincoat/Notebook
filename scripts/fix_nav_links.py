#!/usr/bin/env python3
"""修复：
1. [HTML] 链接从 viewer 改回 MkDocs 渲染页 URL（目录 URL）
2. nav 显示名统一为「中文 英文」
"""
import os
import re
import urllib.parse

ROOT = '/srv/notebook'

# ---------- 1. [HTML] 链接修复 ----------
INDEX_FILES = [
    'docs/index.md',
    'docs/图论 Graph/index.md',
    'docs/数据结构 DataStructure/index.md',
    'docs/基础算法与常用技巧 BA&CT/index.md',
    'docs/图论 Graph/树 Tree/最近公共祖先 LCA/最近公共祖先 LCA.md',
]

def html_url(file_param):
    """viewer 的 file 参数（解码后相对 docs 路径）-> MkDocs 目录 URL"""
    p = file_param
    if p.endswith('/index.md'):
        p = p[:-len('index.md')]
    else:
        p = p[:-3]  # 去 .md
    p = p.replace(' ', '%20')
    return '/' + p + '/'

LINK_RE = re.compile(r'\[([^\]]*)\]\(([^)]*)\)')

def fix_html_links(fp):
    with open(fp, encoding='utf-8') as f:
        content = f.read()
    n = 0
    def repl(m):
        nonlocal n
        text, href = m.group(1), m.group(2)
        if text != 'HTML':
            return m.group(0)
        mm = re.match(r'^/viewer\.html\?file=(.+)$', href)
        if not mm:
            return m.group(0)
        decoded = urllib.parse.unquote(mm.group(1))
        n += 1
        return '[HTML](' + html_url(decoded) + ')'
    new = LINK_RE.sub(repl, content)
    if n:
        with open(fp, 'w', encoding='utf-8') as f:
            f.write(new)
    print(f'  {fp}: {n} 处 [HTML] 链接已改回 MkDocs 页')
    return n

print('==> [1/2] [HTML] 链接修复')
total = 0
for fp in INDEX_FILES:
    if os.path.exists(fp):
        total += fix_html_links(fp)
print(f'  共 {total} 处')

# ---------- 2. nav 显示名 ----------
NAV_TITLE = {
    '二分搜索': '二分搜索 BinarySearch',
    '反悔贪心': '反悔贪心 RegretGreedy',
    '线段树': '线段树 SegmentTree',
    '矩阵': '矩阵 Matrix',
    '单调栈': '单调栈 MonotonicStack',
    '单调队列': '单调队列 MonotonicQueue',
    '平衡树': '平衡树 BalancedTree',
    '最短路': '最短路 ShortestPath',
    '差分约束': '差分约束 DiffConstraints',
    '欧拉路': '欧拉路径 Euler Path',
    'LCA': '最近公共祖先 LCA',
    'MST': '最小生成树 MST',
    '树的直径': '树的直径 TreeDiameter',
    '强联通分量 SCC': '强连通分量 SCC',
    '质因数分解': '质因数分解 PrimeFactorization',
    '位运算': '位运算 BitOperator',
    '逆元': '逆元 Inverse',
    '丢番图方程': '丢番图方程 IndeterminateEquation',
    'STL': '标准模板库 STL',
    'P10449 费解的开关': '费解的开关 P10449',
    'P1450 [HAOI2008] 硬币购物': '硬币购物 P1450',
    'P1553 数字反转（升级版）': '数字反转 P1553',
    'P1593 因子和': '因子和 P1593',
    'P1879 [USACO06NOV] Corn Fields G': '玉米田 Corn Fields P1879',
    'P2114 [NOI2014] 起床困难综合症': '起床困难综合症 P2114',
    'P2827 [NOIP 2016 提高组] 蚯蚓': '蚯蚓 P2827',
    'P7077 [CSP-S 2020] 函数调用': '函数调用 P7077',
}

print('==> [2/2] nav 显示名更新')
yml = os.path.join(ROOT, 'mkdocs.yml')
with open(yml, encoding='utf-8') as f:
    lines = f.readlines()
n = 0
for i, line in enumerate(lines):
    if '.md' in line and ': ' in line:
        head, _, tail = line.rpartition(': ')
        key = tail.strip()
        if key.endswith('.md'):
            title = head.strip().lstrip('- ').strip()
            if title in NAV_TITLE:
                indent = head[:len(head) - len(head.lstrip())]
                new_head = head.replace(title, NAV_TITLE[title], 1)
                lines[i] = new_head + ': ' + key + '\n'
                n += 1
with open(yml, 'w', encoding='utf-8') as f:
    f.writelines(lines)
print(f'  nav 更新 {n} 处显示名')