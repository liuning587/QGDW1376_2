# -*- coding: utf-8 -*-
"""Smoke: load GDW1376_2 DLL — AFN03 金样、粘包、AFN05-F200、AFN=F0 转储。"""
import ctypes
import os
import sys

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
DLL_CANDIDATES = [
    os.path.join(REPO, "GDW1376_2", "GDW1376_2_64.dll"),
    os.path.join(REPO, "py103762", "GDW1376_2_64.dll"),
    os.path.join(REPO, "py103762", "GDW1376_2.dll"),
    os.path.join(REPO, "build", "GDW1376_2.dll"),
]


def main():
    dll_path = None
    for p in DLL_CANDIDATES:
        if os.path.isfile(p):
            dll_path = p
            break
    if not dll_path:
        print("SKIP: no DLL found in", DLL_CANDIDATES)
        return 0

    dll = ctypes.CDLL(dll_path)
    dll.GDW1376_2_parse_foy_py.argtypes = [
        ctypes.c_char_p,
        ctypes.c_char_p,
        ctypes.c_int,
    ]
    dll.GDW1376_2_parse_foy_py.restype = ctypes.c_int
    dll.GDW1376_2_error.argtypes = [ctypes.c_int]
    dll.GDW1376_2_error.restype = ctypes.c_char_p

    def parse_hex(hexstr):
        out = ctypes.create_string_buffer(1024 * 8)
        b = hexstr.replace(" ", "").encode("ascii")
        ret = dll.GDW1376_2_parse_foy_py(b, out, ctypes.sizeof(out))
        msg = out.value.decode("gbk", errors="replace")
        err = dll.GDW1376_2_error(ret).decode("gbk", errors="replace") if ret else ""
        return ret, msg, err

    # README example (AFN03 F1)
    good = (
        "68 0F 00 41 01 00 FF 00 00 00 03 01 00 45 16"
    )
    ret, msg, err = parse_hex(good)
    if ret != 0:
        print("FAIL good frame ret=%r err=%r msg head=%r" % (ret, err, msg[:200]))
        return 1
    if "解析成功" not in msg and "厂商代码" not in msg:
        print("FAIL good frame unexpected output:", msg[:400])
        return 1

    # Same frame padded with extra byte (simulated sticky tail): must not report success.
    # New builds: chkfrm returns -ERR_CHKFRM_LEN_LONG (-8). Older DLLs returned +8; accept both.
    sticky = good + " 00"
    ret2, msg2, err2 = parse_hex(sticky)
    if ret2 == 0:
        print("FAIL sticky should not succeed, got ret=0", msg2[:200])
        return 1
    if ret2 not in (-8, 8):
        print("WARN sticky ret=%r (expected -8 or legacy +8) err=%r" % (ret2, err2))
    if ret2 == 8:
        print("NOTE: DLL is legacy build (ERR_CHKFRM_LEN_LONG as +8); rebuild GDW1376_2 for negative codes.")

    # AFN05 F200 下行（DT1=80H bit7, DT2=18H => Fn=200），拒绝上报标志=01
    f200 = "68 10 00 41 01 00 FF 00 00 00 05 80 18 01 DF 16"
    r3, msg3, _ = parse_hex(f200)
    if r3 != 0:
        print("FAIL AFN05 F200 ret=%r msg head=%r" % (r3, msg3[:300]))
        return 1
    if "80 18" not in msg3:
        print("FAIL AFN05 F200 missing DT in output:", msg3[:500])
        return 1

    # AFN=F0H，Fn=1，数据域 11 22（厂家自定义，仅转储）
    f0 = "68 11 00 41 01 00 FF 00 00 00 F0 01 00 11 22 65 16"
    r4, msg4, _ = parse_hex(f0)
    if r4 != 0:
        print("FAIL AFN F0 ret=%r msg head=%r" % (r4, msg4[:300]))
        return 1
    if "11 22" not in msg4.replace("\n", " "):
        print("FAIL AFN F0 payload not in output:", msg4[:500])
        return 1

    print("OK smoke_m0 DLL=%s" % dll_path)
    return 0


if __name__ == "__main__":
    sys.exit(main())
