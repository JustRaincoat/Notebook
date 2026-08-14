#!/usr/bin/env bash
# JustRaincoat/Notebook 一键构建脚本
# 用法: ./build.sh   （在 /srv/notebook 目录下执行）
set -euo pipefail

cd "$(dirname "$0")"
SITE_DIR="/var/www/anki.justraincoat.top"
SRC_DIR="/srv/notebook"
CODE_DIR="$SRC_DIR/Code"

MKDOCS="${MKDOCS:-/opt/mkdocs-venv/bin/mkdocs}"
[ -x "$MKDOCS" ] || MKDOCS="$(command -v mkdocs || echo /root/mkdocs-venv/bin/mkdocs)"

echo "==> [1/7] mkdocs build"
rm -rf "$SITE_DIR"
"$MKDOCS" build 2>&1 | tail -5 || { echo "mkdocs build 失败"; exit 1; }

echo "==> [2/7] 复制 docs/*.md 到站点（viewer 用）"
cd "$SRC_DIR/docs"
find . -name "*.md" -type f | while read -r f; do
  mkdir -p "$SITE_DIR/$(dirname "$f")"
  cp "$f" "$SITE_DIR/$f"
done
cd "$SRC_DIR"
MD_COUNT=$(find "$SITE_DIR" -name "*.md" | wc -l)
echo "    已复制 $MD_COUNT 个 md 文件"

echo "==> [3/7] 部署 viewer.html 到站点根"
cp "$SRC_DIR/viewer.html" "$SITE_DIR/viewer.html"
echo "    viewer.html -> /viewer.html"

echo "==> [4/7] 复制 Code/*.cpp 到站点"
mkdir -p "$SITE_DIR/Code"
find "$CODE_DIR" -maxdepth 1 -name "*.cpp" -type f -exec cp {} "$SITE_DIR/Code/" \;
COUNT=$(find "$SITE_DIR/Code" -maxdepth 1 -name "*.cpp" | wc -l)
echo "    已复制 $COUNT 个 cpp 文件"

echo "==> [5/7] 生成 Code/index.html 文件列表"
{
  echo '<!DOCTYPE html><html lang="zh"><head><meta charset="utf-8">'
  echo '<meta name="viewport" content="width=device-width,initial-scale=1">'
  echo '<title>代码库 - JustRaincoat/Notebook</title>'
  echo '<style>body{font-family:system-ui,-apple-system,sans-serif;max-width:760px;margin:40px auto;padding:0 16px;line-height:1.8}'
  echo 'a{color:#3f51b5;text-decoration:none}a:hover{text-decoration:underline}'
  echo 'code{background:#f0f0f0;padding:2px 6px;border-radius:4px;font-size:14px}</style></head><body>'
  echo "<h1>代码库</h1><p>共 $COUNT 份代码，点击查看源码（高亮）。</p><ul>"
  find "$SITE_DIR/Code" -maxdepth 1 -name "*.cpp" -printf "%f\n" | sort | while read -r f; do
    echo "<li><a href=\"/viewer.html?file=Code/$(python3 -c "import urllib.parse,sys;print(urllib.parse.quote(sys.argv[1]))" "$f")\"><code>$f</code></a></li>"
  done
  echo '</ul><p><a href="/">← 返回首页</a></p></body></html>'
} > "$SITE_DIR/Code/index.html"

echo "==> [6/7] 悬空链接扫描"
FAIL=0

# 6.1 扫描 HTML 的 href/src
while IFS= read -r -d '' html; do
  dir=$(dirname "$html")
  grep -oE '(href|src)="[^"]+"' "$html" | sed -E 's/^(href|src)="//;s/"$//' | while IFS= read -r link; do
    case "$link" in
      http*|mailto:*|javascript:*|data:*|\#*) continue ;;
    esac
    target="${link%%#*}"
    target="${target%%\?*}"
    if [ -z "$target" ]; then continue; fi
    if [ "${target:0:1}" = "/" ]; then
      full="$SITE_DIR$target"
    else
      full="$dir/$target"
    fi
    full=$(realpath -m "$full" 2>/dev/null || echo "$full")
    if [ ! -e "$full" ]; then
      echo "  [悬空] $html -> $link"
      FAIL=1
    fi
  done
done < <(find "$SITE_DIR" -name "*.html" -type f)

# 6.2 扫描 md 内的 /viewer.html?file= 链接
while IFS= read -r -d '' md; do
  grep -oE 'viewer\.html\?file=[^)" ]+' "$md" | while IFS= read -r link; do
    raw="${link#viewer.html?file=}"
    # URL 解码
    dec=$(python3 -c "import urllib.parse,sys;print(urllib.parse.unquote(sys.argv[1]))" "$raw")
    dec="${dec%%#*}"
    if [ -z "$dec" ]; then continue; fi
    full="$SITE_DIR/$dec"
    full=$(realpath -m "$full" 2>/dev/null || echo "$full")
    if [ ! -e "$full" ]; then
      echo "  [悬空] $md -> viewer.html?file=$raw"
      FAIL=1
    fi
  done
done < <(find "$SITE_DIR" -name "*.md" -type f)

if [ "$FAIL" = "1" ]; then
  echo "!! 发现悬空链接（见上），构建未通过"
  exit 1
else
  echo "    无悬空链接 ✓"
fi

echo "==> [7/7] 构建完成: $SITE_DIR"
