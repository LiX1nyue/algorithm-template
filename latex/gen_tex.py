#!/usr/bin/env python3
"""扫描仓库中所有 .cpp 模板，生成 LaTeX 源文件用于编译 PDF。

用法: python3 gen_tex.py   (在 latex/ 目录下运行)
输出: build/main.tex 与 build/code/*.cpp (UTF-8 净化副本)
"""
import re
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent   # 仓库根目录
BUILD = Path(__file__).resolve().parent / "build"
CODE_DIR = BUILD / "code"

# 目录 -> 章节标题
SECTION_MAP = {
    "data structure": "数据结构 Data Structure",
    "graph": "图论 Graph",
    "math": "数学 Math",
    "string": "字符串 String",
    ".": "其他 Others",
}
# 编译顺序
SECTION_ORDER = ["data structure", "graph", "math", "string", "."]

LSTSET = r"""
\lstset{
  language=C++,
  basicstyle=\ttfamily\footnotesize,
  keywordstyle=\color{keyword}\bfseries,
  commentstyle=\color{comment}\itshape,
  stringstyle=\color{string},
  numbers=left,
  numberstyle=\tiny\color{gray},
  frame=lines,
  rulecolor=\color{lightgray},
  breaklines=true,
  breakatwhitespace=true,
  tabsize=4,
  showstringspaces=false,
  columns=flexible,
  keepspaces=true,
  xleftmargin=2em,
}
"""

PREAMBLE = r"""\documentclass[12pt, UTF8]{ctexart}
\usepackage[a4paper, margin=1.8cm]{geometry}
\usepackage{fontspec}
\usepackage{listings}
\usepackage{xcolor}
\usepackage{hyperref}
% DejaVu Sans Mono 覆盖 ∨ ⊕ ∈ 等数学符号，避免缺字警告
\setmonofont{DejaVu Sans Mono}
\hypersetup{colorlinks=true, linkcolor=black, pdftitle={算法模板},
            pdfauthor={李欣悦}}

\definecolor{keyword}{RGB}{0,0,255}
\definecolor{comment}{RGB}{0,128,0}
\definecolor{string}{RGB}{170,0,0}
\definecolor{lightgray}{RGB}{200,200,200}
""" + LSTSET + r"""
\title{\textbf{算法竞赛模板集}\\ \large C++ Algorithm Templates}
\author{李欣悦}
\date{\today}

\begin{document}
\maketitle
\tableofcontents
\newpage
"""


def to_utf8(raw: bytes) -> str:
    for enc in ("utf-8", "gb18030", "latin-1"):
        try:
            return raw.decode(enc)
        except UnicodeDecodeError:
            continue
    return raw.decode("utf-8", errors="replace")


def tex_escape(s: str) -> str:
    """转义 LaTeX 特殊字符，用于标题。"""
    return s.replace("\\", r"\textbackslash{}").replace("_", r"\_") \
            .replace("&", r"\&").replace("#", r"\#").replace("%", r"\%") \
            .replace("$", r"\$").replace("{", r"\{").replace("}", r"\}") \
            .replace("~", r"\textasciitilde{}").replace("^", r"\textasciicircum{}")


def sanitize(name: str) -> str:
    return re.sub(r"[^A-Za-z0-9._-]", "_", name)


def collect():
    """返回 {目录名: [(源文件 Path, 净化名, 行数), ...]}，保持稳定排序。"""
    groups = {}
    for cpp in sorted(ROOT.rglob("*.cpp"), key=lambda p: str(p).lower()):
        if any(part.startswith(".") for part in cpp.relative_to(ROOT).parts):
            continue  # 跳过 .git 等隐藏目录
        rel = cpp.relative_to(ROOT)
        dirname = str(rel.parent) if str(rel.parent) != "." else "."
        stem = cpp.stem
        groups.setdefault(dirname, []).append((cpp, stem, sanitize(f"{dirname}_{stem}.cpp")))
    return groups


def main():
    if BUILD.exists():
        shutil.rmtree(BUILD)
    CODE_DIR.mkdir(parents=True)

    groups = collect()
    # 重新按预定顺序排列目录
    ordered = {}
    for key in SECTION_ORDER:
        if key in groups:
            ordered[key] = groups.pop(key)
    ordered.update(groups)  # 其余目录追加在末尾

    body = []
    stats = {"files": 0, "lines": 0}
    for dirname, files in ordered.items():
        title = SECTION_MAP.get(dirname, dirname)
        body.append(f"\\section{{{tex_escape(title)}}}\n")
        for src, stem, safe in files:
            text = to_utf8(src.read_bytes())
            (CODE_DIR / safe).write_text(text, encoding="utf-8")
            nlines = text.count("\n") + 1
            stats["files"] += 1
            stats["lines"] += nlines
            body.append(
                f"\\subsection{{{tex_escape(stem)}}}\\hfill\\normalsize\\textit{{{nlines} 行}}\n\n"
                f"\\lstinputlisting{{code/{safe}}}\n"
            )

    tex = PREAMBLE + "\n".join(body) + "\n\\end{document}\n"
    (BUILD / "main.tex").write_text(tex, encoding="utf-8")

    print(f"生成完成: {stats['files']} 个文件, 共 {stats['lines']} 行 -> {BUILD / 'main.tex'}")
    print("编译: latexmk -xelatex -cd build/main.tex")


if __name__ == "__main__":
    main()
