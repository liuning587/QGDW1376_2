"""config"""
import os
import sys


MASTER_SOFTWARE_VERSION = 'V1.0Beta1'
MASTER_SOFTWARE_DT = '2021.04.16'
MASTER_WINDOW_TITLE_ADD = '_%s(%s)'%(MASTER_SOFTWARE_VERSION, MASTER_SOFTWARE_DT)
MASTER_ICO_PATH = 'imgs/Penguin.ico'
TRANS_SOFTWARE_VERSION = MASTER_SOFTWARE_VERSION
TRANS_SOFTWARE_DT = MASTER_SOFTWARE_DT
TRANS_WINDOW_TITLE_ADD = '_%s(%s)'%(TRANS_SOFTWARE_VERSION, TRANS_SOFTWARE_DT)
TRANS_ICO_PATH = 'imgs/Penguin.ico'

TRANS_WINDOW = None
ABOUT_WINDOW = None

CONFIG_DIR = os.path.join(os.path.expanduser('~'), '.103762master/')
CONFIG_FILE_PATH = os.path.join(CONFIG_DIR, '103762master.conf')
MSG_LOG_DIR = os.path.join(CONFIG_DIR, 'logs/')

IS_USE_PYSIDE = False

IS_FILETER_CA = True

RUN_EXE_PATH = ''
LOG_PATH = ''

if getattr(sys, 'frozen', False):
    SOFTWARE_PATH = sys._MEIPASS
else:
    SOFTWARE_PATH = os.path.join(os.path.split(os.path.realpath(__file__))[0], '..')
