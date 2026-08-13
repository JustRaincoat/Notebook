#!/usr/bin/env python3
import os, re, urllib.parse

DOCS = "/srv/notebook/docs"
LINK_RE = re.compile(r'\]\(([^)]+)\)')

def fix_target(target, md_path):
    anchor = ""
    if "#" in target:
        target, anchor = target.split("#", 1)
        anchor = "#" + anchor
    if target.startswith(("http", "mailto:", "javascript:", "data:")):
        return target + anchor
    if target.startswith("<") or target.startswith("$"):
        return target + anchor

    # .cpp -> viewer
    if target.lower().endswith(".cpp"):
        fname = urllib.parse.unquote(target.split("/")[-1])
        return "/Code/viewer.html?file=" + urllib.parse.quote(fname) + anchor

    base = None
    if target.lower().endswith((".md", ".html")):
        base = re.sub(r'\.(md|html)$', '', target, flags=re.I)
    if base is None:
        return target + anchor

    # README 被 exclude -> 指向首页
    if base.rstrip("/").endswith("/README") or base.rstrip("/") == "README":
        return "/" + anchor

    if base.startswith("/"):
        rel = base.lstrip("/")
        rel = urllib.parse.unquote(rel)
    else:
        cur_dir = os.path.dirname(md_path)
        joined = os.path.normpath(os.path.join(cur_dir, urllib.parse.unquote(base)))
        rel = os.path.relpath(joined, DOCS)
    rel = rel.replace("\\", "/")

    if rel == "index" or rel.endswith("/index"):
        rel = rel[: -len("index")].rstrip("/") if rel.endswith("/index") else ""
        url = "/" + rel if rel else "/"
    else:
        url = "/" + rel + "/"
    # 重新 URL 编码路径段
    url = "/".join(urllib.parse.quote(seg) for seg in url.split("/"))
    return url + anchor

MODE = os.environ.get("MODE", "apply")
changed = 0
for root, dirs, files in os.walk(DOCS):
    for fn in files:
        if not fn.endswith(".md"):
            continue
        p = os.path.join(root, fn)
        text = open(p, encoding="utf-8").read()
        def repl(m):
            return "](" + fix_target(m.group(1), p) + ")"
        new = LINK_RE.sub(repl, text)
        if new != text:
            changed += 1
            if MODE == "apply":
                open(p, "w", encoding="utf-8").write(new)
            else:
                print("== " + os.path.relpath(p, DOCS))
                old_lines = text.splitlines()
                new_lines = new.splitlines()
                for i in range(min(len(old_lines), len(new_lines))):
                    if old_lines[i] != new_lines[i]:
                        print("  - " + old_lines[i].strip())
                        print("  + " + new_lines[i].strip())
                if len(old_lines) != len(new_lines):
                    print("  (line count diff)")
print("changed files: %d (mode=%s)" % (changed, MODE))
