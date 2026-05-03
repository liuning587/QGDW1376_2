# 金样报文（fixtures）

- **用途**：`GDW1376_2_parse` / `GDW1376_2_parse_foy_py` 回归；与 `TESTING.md` §4 约定一致。  
- **命名建议**：`afn{XX}_fn{YYY}_{brief}.hex`，内容为**一行十六进制**（可含空格）。  
- **CS**：与实现一致，自帧内 **C（控制域）** 起至 **CS 前一字节** 的算术和（见 `chkfrm.c` / `get_cs`）。

当前冒烟用例仍集中在 `scripts/smoke_m0.py`；可将本目录样例逐步迁入并由脚本批量加载。
