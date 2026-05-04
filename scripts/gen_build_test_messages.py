# -*- coding: utf-8 -*-
"""
Build minimal Q/GDW 10376.2-style frames (68 L L C A AFN DT1 DT2 payload CS 16),
CS = sum(bytes from C through last payload byte) mod 256 (same as lib.c get_cs range).

For each (AFN, Fn) tries zero padding up to max_pad until GDW1376_2_parse_foy_py returns 0.
Writes build/测试报文_全量生成.txt
"""
import ctypes
import os
import sys

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
DLL_CAND = [
    os.path.join(REPO, "GDW1376_2", "GDW1376_2_64.dll"),
    os.path.join(REPO, "py103762", "GDW1376_2_64.dll"),
    os.path.join(REPO, "py103762", "GDW1376_2.dll"),
]


def fn_to_dt1_dt2(fn):
    """Inverse of get_fn: one DT1 bit + DT2 such that DT2*8+bit+1==fn, bit 0..7."""
    if fn < 1 or fn > 256:
        return None, None
    x = fn - 1
    dt2 = x // 8
    bit = x % 8
    dt1 = 1 << bit
    if dt1 > 255:
        return None, None
    return dt1, dt2


def build_frame(afn, dt1, dt2, payload, c_ctrl):
    addr = bytes([0x01, 0x00, 0xFF, 0x00, 0x00, 0x00])
    pdu = bytes([afn & 0xFF, dt1 & 0xFF, dt2 & 0xFF]) + payload
    mid = bytes([c_ctrl & 0xFF]) + addr + pdu
    lo = (3 + len(mid) + 2) & 0xFF
    hi = ((3 + len(mid) + 2) >> 8) & 0xFF
    b = bytearray([0x68, lo, hi]) + mid
    cs = sum(b[3:]) % 256
    b.append(cs)
    b.append(0x16)
    return bytes(b)


def load_dll():
    for p in DLL_CAND:
        if os.path.isfile(p):
            return ctypes.CDLL(p), p
    return None, None


def main():
    dll, dll_path = load_dll()
    if dll is None:
        print("SKIP: no DLL", file=sys.stderr)
        return 1
    out_n = 1024 * 64
    dll.GDW1376_2_parse_foy_py.argtypes = [
        ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
    dll.GDW1376_2_parse_foy_py.restype = ctypes.c_int
    buf = ctypes.create_string_buffer(out_n)

    def parse_hex(bs):
        r = dll.GDW1376_2_parse_foy_py(
            bs.hex().encode("ascii"), buf, out_n)
        return r

    # (afn_hex, list of fn, c_down, c_up)  c_down=0x41 bit7=0, c_up=0xC1 bit7=1
    catalog = [
        ("00", [1, 2], False, True),
        ("01", [1, 2, 3], True, False),
        ("02", [1], True, True),
        ("03", [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 100], True, True),
        ("04", [1, 2, 3], True, False),
        ("05", [1, 2, 3, 4, 5, 6, 16, 100, 101, 200], True, False),
        ("06", [1, 2, 3, 4, 5], False, True),
        ("10", [1, 2, 3, 4, 5, 6, 7, 9, 21, 31, 40, 100, 101, 104, 111, 112], True, True),
        ("11", [1, 2, 3, 4, 5, 6, 100, 101, 102], True, False),
        ("12", [1, 2, 3], True, False),
        ("13", [1], True, True),
        ("14", [1, 2, 3, 4], True, True),
        ("15", [1], True, True),
        ("f0", list(range(1, 9)), True, True),
        ("f1", [1], True, True),
    ]

    lines = []
    lines.append("# 本文件由 scripts/gen_build_test_messages.py 自动生成")
    lines.append("# DLL: %s" % dll_path)
    lines.append("# 规则：下行 C=41H，上行 C=C1H（与 GDW1376_2.c 中 dir=BITS(C,7) 一致）")
    lines.append("# 每条为「零填充搜索」后第一组使解析 ret=0 的帧；若仍失败则标注 SKIP")
    lines.append("")

    ok = 0
    skip = 0
    for afn_s, fns, allow_down, allow_up in catalog:
        afn = int(afn_s, 16)
        for fn in fns:
            dt1, dt2 = fn_to_dt1_dt2(fn)
            if dt1 is None:
                lines.append("# AFN=%s Fn=%d SKIP(fn out of range)" % (afn_s.upper(), fn))
                skip += 1
                continue
            # 每个 (AFN, Fn) 只输出一条：先下行（若允许），再上行（若允许）
            found = None
            dir_zh = ""
            for uplink, allow in ((False, allow_down), (True, allow_up)):
                if not allow:
                    continue
                c_ctrl = 0xC1 if uplink else 0x41
                for pad in range(0, 129):
                    payload = bytes([0] * pad)
                    bs = build_frame(afn, dt1, dt2, payload, c_ctrl)
                    if parse_hex(bs) == 0:
                        found = (pad, bs, "上行" if uplink else "下行")
                        break
                if found is not None:
                    break
            if found is None:
                lines.append(
                    "# SKIP AFN=%02XH Fn=%d (下行/上行 pad 0..128 均 ret!=0)"
                    % (afn, fn))
                skip += 1
            else:
                pad, bs, dir_zh = found
                hx = " ".join("%02X" % b for b in bs)
                lines.append(
                    "# AFN=%02XH Fn=%d %s pad=%d"
                    % (afn, fn, dir_zh, pad))
                lines.append(hx)
                lines.append("")
                ok += 1

    out_path = os.path.join(REPO, "build", "测试报文_全量生成.txt")
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))
    print("Wrote %s ok=%d skip=%d" % (out_path.encode("unicode_escape").decode("ascii"), ok, skip))
    return 0


if __name__ == "__main__":
    sys.exit(main())
