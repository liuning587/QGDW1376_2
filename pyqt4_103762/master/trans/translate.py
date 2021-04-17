"""translate 10376.2 messages"""
import os
import ctypes
import traceback
import master.trans.common as commonfun
from master import config

QGDW13762_DLL_PATH_DEFAULT = os.path.join(config.SOFTWARE_PATH, 'GDW1376_2.dll')
QGDW13762_DLL_PATH_64 = os.path.join(config.SOFTWARE_PATH, 'GDW1376_2_64.dll')

class QGDW103762Class():
    """QGDW10376.2 Parse"""
    def __init__(self):
        try:
            self.dll = ctypes.CDLL(QGDW13762_DLL_PATH_DEFAULT)
        except Exception:
            print('try dll 64')
            self.dll = ctypes.CDLL(QGDW13762_DLL_PATH_64)

        self.dll.GDW1376_2_parse_foy_py.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
        self.dll.GDW1376_2_parse_foy_py.restype = ctypes.c_int

        self.dll.GDW1376_2_error.argtypes = [ctypes.c_int]
        self.dll.GDW1376_2_error.restype = ctypes.c_char_p

    def parse_all(self, instr):
        """parse all"""
        try:
            out = ctypes.create_string_buffer(1024*8, "\0")
            ret = self.dll.GDW1376_2_parse_foy_py(str.encode(instr), out, 1024*8)
        except Exception as e:
            return -1, str(e)
        if ret != 0:
            return ret, out.value.decode("gbk")
        return ret, out.value.decode("gbk")

    def get_error_str(self, err):
        """parse all"""
        try:
            ret = self.dll.GDW1376_2_error(err)
        except Exception as e:
            return str(e)
        return ret.decode("gbk")

class Translate:
    """translate class"""
    def __init__(self, m_text):
        """init"""
        self.parse = QGDW103762Class()
        self.source_msg = commonfun.format_text(m_text)
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

    def get_full(self, is_show_level=True, is_show_type=True, is_output_html=True, has_linklayer=True):
        """get full translate"""
        if self.is_success:
            # res_text = '<table style="table-layout:fixed; word-wrap:break-word; border-style:solid;">' if is_output_html else ''
            res_text = ''
        else:
            if is_output_html:
                res_text = '<p style="color: red">报文解析过程出现问题，请检查报文。若报文无问题请反馈660316，谢谢！</p><p> </p>'
            else:
                res_text = '报文解析过程出现问题，请检查报文。若报文无问题请反馈660316，谢谢！\n\n'

        res_text += self.res_str

        if is_output_html:
            res_text += '</table>'
        # print(res_text)
        return res_text
    
    def get_brief(self):
        """get brief translate"""
        return ''

    def get_clipboard_text(self, is_show_level=True, is_show_type=True):
        """get_clipboard_text"""
        msg = self.source_msg
        full = self.get_full(is_show_level, is_show_type, is_output_html=False)
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
