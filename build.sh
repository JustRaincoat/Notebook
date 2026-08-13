#!/usr/bin/env bash
# JustRaincoat/Notebook 一键构建脚本
# 用法: ./build.sh   （在 /srv/notebook 目录下执行）
set -euo pipefail

cd "$(dirname "$0")"
SITE_DIR="/var/www/anki.justraincoat.top"
SRC_DIR="/srv/notebook"
CODE_DIR="$SRC_DIR/Code"
VIEWER="Code/viewer.html"
LISTING="Code/index.html"

MKDOCS="${MKDOCS:-/opt/mkdocs-venv/bin/mkdocs}"
[ -x "$MKDOCS" ] || MKDOCS="$(command -v mkdocs || echo /root/mkdocs-venv/bin/mkdocs)"

echo "==> [1/5] mkdocs build"
rm -rf "$SITE_DIR"
"$MKDOCS" build 2>&1 | tail -5 || { echo "mkdocs build 失败"; exit 1; }

echo "==> [2/5] 复制 Code/*.cpp 到站点"
mkdir -p "$SITE_DIR/Code"
find "$CODE_DIR" -maxdepth 1 -name "*.cpp" -type f -exec cp {} "$SITE_DIR/Code/" \;
COUNT=$(find "$SITE_DIR/Code" -maxdepth 1 -name "*.cpp" | wc -l)
echo "    已复制 $COUNT 个 cpp 文件"

echo "==> [3/5] 生成 Code/index.html 文件列表"
{
  echo '<!DOCTYPE html><html lang="zh"><head><meta charset="utf-8">'
  echo '<meta name="viewport" content="width=device-width,initial-scale=1">'
  echo '<title>代码库 - JustRaincoat/Notebook</title>'
  echo '<style>body{font-family:system-ui,-apple-system,sans-serif;max-width:760px;margin:40px auto;padding:0 16px;line-height:1.8}'
  echo 'a{color:#3f51b5;text-decoration:none}a:hover{text-decoration:underline}'
  echo 'code{background:#f0f0f0;padding:2px 6px;border-radius:4px;font-size:14px}</style></head><body>'
  echo "<h1>代码库</h1><p>共 $COUNT 份代码，点击查看纯文本源码。</p><ul>"
  find "$SITE_DIR/Code" -maxdepth 1 -name "*.cpp" -printf "%f\n" | sort | while read -r f; do
    echo "<li><a href=\"viewer.html?file=$(python3 -c "import urllib.parse,sys;print(urllib.parse.quote(sys.argv[1]))" "$f")\"><code>$f</code></a></li>"
  done
  echo '</ul><p><a href="/">← 返回首页</a></p></body></html>'
} > "$SITE_DIR/$LISTING"

echo "==> [4/5] 生成 Code/viewer.html 纯文本查看页"
{
  echo '<!DOCTYPE html><html lang="zh"><head><meta charset="utf-8">'
  echo '<meta name="viewport" content="width=device-width,initial-scale=1">'
  echo '<title>代码查看 - JustRaincoat/Notebook</title>'
  echo '<style>body{font-family:ui-monospace,Consolas,monospace;font-size:14px;margin:0;background:#1e1e1e;color:#d4d4d4}'
  echo '.bar{position:sticky;top:0;background:#252526;padding:10px 16px;display:flex;gap:12px;align-items:center;border-bottom:1px solid #333}'
  echo '.bar a{color:#4fc1ff;text-decoration:none;font-family:system-ui,sans-serif}'
  echo '.bar a:hover{text-decoration:underline}'
  echo '#fname{flex:1;color:#ccc;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}'
  echo '#copy{background:#0e639c;color:#fff;border:none;padding:6px 14px;border-radius:4px;cursor:pointer;font-size:13px}'
  echo '#copy:hover{background:#1177bb}'
  echo 'pre{margin:0;padding:16px;overflow-x:auto;tab-size:4}'
  echo '#err{padding:40px;text-align:center;color:#f48771;font-family:system-ui,sans-serif;font-size:16px}</style></head><body>'
  echo '<div class="bar"><a href="index.html">← 代码库</a><span id="fname"></span><button id="copy">复制</button></div>'
  echo '<pre id="code"></pre><div id="err" style="display:none"></div>'
  echo '<script>'
  echo 'const f=new URLSearchParams(location.search).get("file");'
  echo 'if(!f){document.getElementById("err").style.display="block";document.getElementById("err").textContent="未指定文件参数 (?file=xxx.cpp)";'
  echo 'document.getElementById("code").style.display="none";}'
  echo 'else{'
  echo 'document.getElementById("fname").textContent=f;'
  echo 'fetch(encodeURIComponent(f)).then(r=>{if(!r.ok)throw new Error(r.status);return r.text();})'
  echo '.then(t=>{document.getElementById("code").textContent=t;}).catch(e=>{'
  echo 'document.getElementById("err").style.display="block";document.getElementById("err").textContent="无法加载文件 "+f+"（"+e.message+"）";'
  echo 'document.getElementById("code").style.display="none";});'
  echo 'document.getElementById("copy").onclick=()=>{'
  echo 'const c=document.getElementById("code").textContent;'
  echo 'navigator.clipboard.writeText(c).then(()=>{const b=document.getElementById("copy");b.textContent="已复制";setTimeout(()=>b.textContent="复制",1500);});};}'
  echo '</script></body></html>'
} > "$SITE_DIR/$VIEWER"

echo "==> [5/5] 悬空链接扫描"
FAIL=0
while IFS= read -r -d '' html; do
  dir=$(dirname "$html")
  # 提取本页所有 href / src（跳过 http、mailto、锚点、javascript）
  grep -oE '(href|src)="[^"]+"' "$html" | sed -E 's/^(href|src)="//;s/"$//' | while IFS= read -r link; do
    case "$link" in
      http*|mailto:*|javascript:*|data:*|\#*) continue ;;
    esac
    # 去掉锚点和 query 后解析
    target="${link%%#*}"
    target="${target%%\?*}"
    if [ -z "$target" ]; then continue; fi
    if [ "${target:0:1}" = "/" ]; then
      full="$SITE_DIR$target"
    else
      full="$dir/$target"
    fi
    # 处理相对路径中的 ../
    full=$(realpath -m "$full" 2>/dev/null || echo "$full")
    if [ ! -e "$full" ]; then
      echo "  [悬空] $html -> $link"
      FAIL=1
    fi
  done
done < <(find "$SITE_DIR" -name "*.html" -type f)

if [ "$FAIL" = "1" ]; then
  echo "!! 发现悬空链接（见上），构建未通过"
  exit 1
else
  echo "    无悬空链接 ✓"
fi

echo "==> 构建完成: $SITE_DIR"
