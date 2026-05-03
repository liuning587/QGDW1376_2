"""translate 10376.2 messages"""
import logging
import os
import ctypes
import traceback

_logger = logging.getLogger(__name__)
import master.trans.common as commonfun
from master import config
from master.trans.format_output import format_parse_text

# Must match `py_out` size in GDW1376_2.c (GDW1376_2_parse_foy_py).
PARSE_OUT_BUF_SIZE = 1024 * 64

_REPO_GDW_DIR = os.path.normpath(
    os.path.join(config.SOFTWARE_PATH, '..', 'GDW1376_2'))

def _dll_search_paths():
    """Prefer sibling repo build, then py103762 bundle directory."""
    base = config.SOFTWARE_PATH
    return [
        os.path.join(_REPO_GDW_DIR, 'GDW1376_2_64.dll'),
        os.path.join(_REPO_GDW_DIR, 'GDW1376_2.dll'),
        os.path.join(base, 'GDW1376_2_64.dll'),
        os.path.join(base, 'GDW1376_2.dll'),
    ]


def _decode_dll_buffer(raw):
    """DLL / mixed legacy output: try GBK then UTF-8."""
    if raw is None:
        return ''
    if isinstance(raw, str):
        return raw
    for enc in ('gbk', 'utf-8'):
        try:
            return raw.decode(enc)
        except UnicodeDecodeError:
            continue
    return raw.decode('utf-8', errors='replace')

class QGDW103762Class():
    """QGDW10376.2 Parse"""
    def __init__(self):
        self.dll = None
        self.dll_path = None
        last_err = None
        for path in _dll_search_paths():
            if not os.path.isfile(path):
                continue
            try:
                self.dll = ctypes.CDLL(path)
                self.dll_path = path
                _logger.debug('loaded DLL: %s', path)
                break
            except OSError as ex:
                last_err = ex
        if self.dll is None:
            raise OSError(
                '无法加载 GDW1376_2 DLL，已尝试: %s; 最后错误: %s'
                % ('; '.join(_dll_search_paths()), last_err))

        self.dll.GDW1376_2_parse_foy_py.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
        self.dll.GDW1376_2_parse_foy_py.restype = ctypes.c_int

        self.dll.GDW1376_2_error.argtypes = [ctypes.c_int]
        self.dll.GDW1376_2_error.restype = ctypes.c_char_p

    def parse_all(self, instr):
        """parse all"""
        try:
            out = ctypes.create_string_buffer(PARSE_OUT_BUF_SIZE)
            ret = self.dll.GDW1376_2_parse_foy_py(
                instr.encode('ascii', errors='ignore'), out, PARSE_OUT_BUF_SIZE)
        except Exception as e:
            return -1, str(e)
        text = _decode_dll_buffer(out.value)
        return ret, text

    def get_error_str(self, err):
        """parse all"""
        try:
            ret = self.dll.GDW1376_2_error(err)
        except Exception as e:
            return str(e)
        return _decode_dll_buffer(ret)

class Translate:
    """translate class"""
    def __init__(self, m_text, parser=None):
        """init; optional `parser` (QGDW103762Class) avoids reloading DLL per parse."""
        self.source_msg = commonfun.format_text(m_text)
        if parser is not None:
            self.parse = parser
            self.res_str, self.is_success = self.__trans_all(m_text)
            return
        try:
            self.parse = QGDW103762Class()
        except OSError as ex:
            self.parse = None
            self.res_str = '无法加载解析库: %s' % ex
            self.is_success = False
            return
        self.res_str, self.is_success = self.__trans_all(m_text)

    def __trans_all(self, m_text):
        """translate all messages"""

        res_str = ''
        chk_res = True
        try:
            ret, res_str = self.parse.parse_all(m_text)
            if ret != 0:
                chk_res = False
        except Exception:
            traceback.print_exc()
            return res_str, False
        
        return res_str, chk_res

    def get_full(self, is_show_level=True, is_show_type=True, is_output_html=True,
                 has_linklayer=True, indent_style='none'):
        """get full translate; indent_style: none|block|tree (py103762 only)."""
        if self.is_success:
            # res_text = '<table style="table-layout:fixed; word-wrap:break-word; border-style:solid;">' if is_output_html else ''
            res_text = ''
        else:
            if is_output_html:
                res_text = '<p style="color: red">报文解析过程出现问题，请检查报文。若报文无问题请反馈660316，谢谢！</p><p> </p>'
            else:
                res_text = '报文解析过程出现问题，请检查报文。若报文无问题请反馈660316，谢谢！\n\n'

        res_text += self.res_str
        if not is_output_html:
            res_text = format_parse_text(res_text, indent_style)

        if is_output_html:
            res_text += '</table>'
        # print(res_text)
        return res_text
    
    def get_brief(self):
        """get brief translate"""
        return ''

    def get_clipboard_text(self, is_show_level=True, is_show_type=True, indent_style='none'):
        """get_clipboard_text"""
        msg = self.source_msg
        full = self.get_full(is_show_level, is_show_type, is_output_html=False,
                             indent_style=indent_style)
        brief = self.get_brief()

        if brief == '':
            text = '【报文】\n{msg}\n\n【完整解析】\n{full}'.format(msg=msg, brief=brief, full=full)
        else:
            line_list = full.split('\n')
            explain = ''
            for cnt, line in enumerate(line_list, start=0):
                if cnt % 2 == 1:
                    explain += line + '  ——  '
                else:
                    explain += line + '\n'
            text = '【报文】\n{msg}\n\n【概览】\n{brief}\n\n【完整解析】\n{full}'.format(msg=msg, brief=brief, full=full)
        return text
