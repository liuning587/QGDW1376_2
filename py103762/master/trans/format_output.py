# -*- coding: utf-8 -*-
"""Post-format DLL parse text for py103762 UI (indent / block spacing)."""
import html
import re

# none | block | tree
INDENT_NONE = 'none'
INDENT_BLOCK = 'block'
INDENT_TREE = 'tree'
INDENT_CHOICES = (
    (INDENT_NONE, '原始'),
    (INDENT_BLOCK, '块分隔'),
    (INDENT_TREE, '树状竖线'),
)


def format_parse_text(text, style):
    """Reformat plain parse output. `style` is none|block|tree."""
    if not text or style == INDENT_NONE:
        return text
    text = text.replace('\r\n', '\n')
    lines = text.split('\n')
    if style == INDENT_BLOCK:
        return _format_block(lines)
    if style == INDENT_TREE:
        return _format_tree(lines)
    return text


def _format_block(lines):
    """Insert blank line before major separators and result banner."""
    sep_re = re.compile(r'^[-=]{6,}\s*$')
    out = []
    for line in lines:
        stripped = line.strip()
        if sep_re.match(line) or (
            stripped and ('解析成功' in line or '解析失败' in line)
        ):
            if out and out[-1] != '':
                out.append('')
        out.append(line)
    return '\n'.join(out)


def _format_tree(lines):
    """Block spacing + vertical bar for non-separator body lines."""
    block = _format_block(lines).split('\n')
    sep_re = re.compile(r'^[-=]{6,}\s*$')
    out = []
    for line in block:
        s = line.strip()
        if not s:
            out.append('')
            continue
        if sep_re.match(line):
            out.append(line)
            continue
        if '解析成功' in line or '解析失败' in line:
            out.append(line)
            continue
        if line.startswith('  ') or line.startswith('\t'):
            out.append('│   ' + line.lstrip())
        else:
            out.append('│ ' + line)
    return '\n'.join(out)


_SEP_LINE = re.compile(r'^[-=]{4,}\s*$')
# 行首为连续十六进制对（报文/数据行），至少 4 字节
_HEX_LEAD = re.compile(r'^[0-9a-fA-F]{2}(?:\s+[0-9a-fA-F]{2}){3,}')
_AFN_HEAD = re.compile(r'AFN\s*[0-9A-Fa-f]', re.I)


def parse_plain_to_display_html(plain):
    """UI-only: plain parse text -> HTML fragment (line breaks as <br/>). Clipboard stays plain."""
    if not plain:
        return ''
    text = plain.replace('\r\n', '\n')
    parts = []
    for line in text.split('\n'):
        esc = html.escape(line)
        st = line.strip()
        if '解析成功' in line:
            parts.append(
                '<span style="color:#0b7a3a;font-weight:600">%s</span>' % esc)
        elif '解析失败' in line:
            parts.append(
                '<span style="color:#c62828;font-weight:600">%s</span>' % esc)
        elif _SEP_LINE.match(line) or (
                st and len(st) >= 4 and all(c in '-=' for c in st)):
            parts.append(
                '<span style="color:#5c6bc0;font-weight:500">%s</span>' % esc)
        elif st and _HEX_LEAD.match(st):
            parts.append('<span style="color:#37474f">%s</span>' % esc)
        elif st and _AFN_HEAD.search(st):
            parts.append('<span style="color:#1565c0">%s</span>' % esc)
        else:
            parts.append('<span style="color:#212121">%s</span>' % esc)
    return '<br/>'.join(parts)
