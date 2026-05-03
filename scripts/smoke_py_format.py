# -*- coding: utf-8 -*-
"""Smoke: py103762 format_output HTML highlight (no Qt)."""
import os
import sys

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
sys.path.insert(0, os.path.join(REPO, "py103762"))

from master.trans.format_output import parse_plain_to_display_html  # noqa: E402


def main():
    h = parse_plain_to_display_html("解析成功\n68 01 02 03 04\n----")
    if "解析成功" not in h or "<span" not in h:
        print("FAIL: expected spans", h[:200])
        return 1
    esc = parse_plain_to_display_html("<tag>")
    if "&lt;tag&gt;" not in esc:
        print("FAIL: escape", esc)
        return 1
    print("OK smoke_py_format")
    return 0


if __name__ == "__main__":
    sys.exit(main())
