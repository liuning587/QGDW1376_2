"""log files trans ui"""
import logging
import re
import os
import sys
import threading
import chardet

_log = logging.getLogger(__name__)
from master.UI.trans_ui_setup import TransWindowUi
from master.trans.translate import Translate, QGDW103762Class
from master.trans.format_output import INDENT_NONE, parse_plain_to_display_html
from master.others import master_config
from master import config
if config.IS_USE_PYSIDE:
    from PySide2 import QtGui, QtCore, QtWidgets
else:
    from PyQt5 import QtGui, QtCore, QtWidgets


class TransWindow(QtWidgets.QMainWindow, TransWindowUi):
    """translate window"""
    load_file = QtCore.Signal(str) if config.IS_USE_PYSIDE else QtCore.pyqtSignal(str)
    set_progress = QtCore.Signal(int) if config.IS_USE_PYSIDE else QtCore.pyqtSignal(int)

    def __init__(self):
        super(TransWindow, self).__init__()
        qss_file = open(os.path.join(config.SOFTWARE_PATH, 'styles/white_blue.qss')).read()
        self.setStyleSheet(qss_file)
        if config.IS_USE_PYSIDE:
            self.setup_ui()
        self.proc_bar.setVisible(False)
        self.show_level_cb.setChecked(True)
        self.auto_wrap_cb.setChecked(False)

        apply_config = master_config.MasterConfig()
        file_list = apply_config.get_last_file()[::-1]
        for file_name in file_list:
            self.file_action = QtWidgets.QAction('%s'%file_name, self)
            self.file_menu.addAction(self.file_action)
            self.file_action.triggered.connect(self.openfile)
        font_size = apply_config.get_font_size()
        self.input_box.set_font_size(font_size)
        self.update_wrap_mode()

        saved_ind = apply_config.get_trans_indent_style()
        for i in range(self.indent_style_cb.count()):
            if self.indent_style_cb.itemData(i) == saved_ind:
                self.indent_style_cb.blockSignals(True)
                self.indent_style_cb.setCurrentIndex(i)
                self.indent_style_cb.blockSignals(False)
                break

        self._parser = None
        try:
            self._parser = QGDW103762Class()
            if getattr(self._parser, 'dll_path', None):
                self.proc_l.setText('就绪 · %s' % self._parser.dll_path)
        except OSError:
            self.proc_l.setText('DLL 未加载')

        self._input_scan_timer = QtCore.QTimer(self)
        self._input_scan_timer.setSingleShot(True)
        self._input_scan_timer.timeout.connect(self._apply_input_scan)
        self._clip_cache_key = None
        self._clip_cache_text = None

        self.setAcceptDrops(True)
        self.input_box.cursorPositionChanged.connect(self.cursor_changed)
        self.input_box.textChanged.connect(self._schedule_input_scan)
        self.msg_box.textChanged.connect(self.start_trans)
        self.find_last_b.clicked.connect(lambda: self.find_last(True))
        self.find_next_b.clicked.connect(lambda: self.find_next(True))
        self.msg_next_b.clicked.connect(lambda: self.jump_to_msg('next'))
        self.msg_priv_b.clicked.connect(lambda: self.jump_to_msg('priv'))
        self.input_zoom_in_b.clicked.connect(self.input_box.zoomIn)
        self.input_zoom_out_b.clicked.connect(self.input_box.zoomOut)
        self.output_zoom_in_b.clicked.connect(self.output_box.zoomIn)
        self.output_zoom_out_b.clicked.connect(self.output_box.zoomOut)
        self.output_copy_b.clicked.connect(self.copy_to_clipboard)
        self.auto_wrap_cb.clicked.connect(self.update_wrap_mode)
        self.show_level_cb.clicked.connect(self.start_trans)
        self.show_dtype_cb.clicked.connect(self.start_trans)
        self.indent_style_cb.currentIndexChanged.connect(self._on_indent_style_changed)
        self.always_top_cb.clicked.connect(self.set_always_top)
        self.open_action.triggered.connect(self.openfile)
        self.reload_action.triggered.connect(lambda: self.openfile(filepath=self.file_now))
        self.clear_action.triggered.connect(self.clear_box)
        self.about_action.triggered.connect(config.ABOUT_WINDOW.show)
        self.find_action.triggered.connect(lambda: self.find_box.setFocus() or self.find_box.selectAll())
        self.next_msg_action.triggered.connect(lambda: self.jump_to_msg('next'))
        self.priv_msg_action.triggered.connect(lambda: self.jump_to_msg('priv'))
        self.load_file.connect(self.load_text, QtCore.Qt.QueuedConnection)
        self.set_progress.connect(self.set_progressbar, QtCore.Qt.QueuedConnection)
        self.find_box.returnPressed.connect(lambda: self.find_next(False))

        self.file_now = ''
        self.msg_find_dict = []
        self.last_selection = (0, 0)
        self.text_find_list = []
        self.last_find_text = ''


    def dragEnterEvent(self, event):
        """drag"""
        if event.mimeData().hasUrls:
            event.accept()
        else:
            event.ignore()


    def dropEvent(self, event):
        """drop file"""
        if event.mimeData().hasUrls:
            event.setDropAction(QtCore.Qt.CopyAction)
            event.accept()
            links = []
            for url in event.mimeData().urls():
                links.append(str(url.toLocalFile()))
            _log.debug('drop url: %s', links[0])
            self.openfile(links[0])
        else:
            event.ignore()


    def load_text(self, file_text):
        """load text"""
        self.proc_bar.setVisible(False)
        self.open_action.setEnabled(True)
        self.setAcceptDrops(True)
        self.input_box.setPlainText(file_text)


    def set_progressbar(self, percent):
        """set progress bar in main process"""
        self.proc_bar.setValue(percent)


    def openfile(self, filepath=''):
        """open file"""
        action = self.sender()
        if isinstance(action, QtWidgets.QAction) and os.path.isfile(action.text()):
            filepath = action.text()
        if not os.path.isfile(filepath):
            filepath,_ = QtWidgets.QFileDialog.getOpenFileName(self, caption='请选择10376.2日志文件', filter='*')
        if filepath:
            save_config = master_config.MasterConfig()
            save_config.add_last_file(filepath)
            save_config.commit()
            file_size = os.path.getsize(filepath)
            if file_size > 5*0x100000:
                reply = QtWidgets.QMessageBox.question(self, '警告', '打开大型文件会使用较长时间，确定打开吗？',\
                                QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No, QtWidgets.QMessageBox.No)
                if reply != QtWidgets.QMessageBox.Yes:
                    return
            self.proc_bar.setVisible(True)
            self.open_action.setEnabled(False)
            self.setAcceptDrops(False)
            self.proc_l.setText('处理中')
            self.setWindowTitle('10376.2日志解析工具 {ver} - {file}'.\
                        format(ver=config.MASTER_WINDOW_TITLE_ADD, file=filepath))
            self.file_now = filepath
            threading.Thread(
                target=self.read_file, args=(filepath,), daemon=True).start()


    def read_file(self, filepath):
        """read file thread"""
        # get file encoding
        with open(filepath, "rb") as file:
            encoding = chardet.detect(file.read(65535))
            _log.debug('chardet: %s', encoding)
            if encoding['confidence'] > 0.95:
                file_encoding = encoding['encoding']
            else:
                file_encoding = 'gb2312'

        with open(filepath, encoding=file_encoding, errors='ignore') as file:
            count = 0
            for _ in file:
                count += 1
        with open(filepath, encoding=file_encoding, errors='ignore') as file:
            file_text = ''
            for i, line in enumerate(file):
                file_text += line
                if i % 500 == 0:
                    self.set_progress.emit(i*95 / count)
        self.load_file.emit(file_text)
        _log.debug('read_file thread done')


    def cursor_changed(self):
        """cursor changed to trans"""
        document = self.input_box.document()
        cursor = self.input_box.textCursor()
        scroll = self.input_box.verticalScrollBar()
        _log.debug(
            'cursor: %d, scroll: %d, document: %d',
            cursor.blockNumber(), scroll.value(),
            document.findBlock(cursor.position()).blockNumber())

        if self.last_selection[0] <= int(self.input_box.textCursor().position())\
                                    <= self.last_selection[1]:
            return
        else:
            self.msg_box.clear()
            for row in self.msg_find_dict:
                if row['span'][0] <= int(self.input_box.textCursor().position()) <= row['span'][1]:
                    self.msg_box.setPlainText(row['message'])
                    cursor = self.input_box.textCursor()
                    cursor.setPosition(row['span'][1])
                    cursor.setPosition(row['span'][0], QtGui.QTextCursor.KeepAnchor)
                    self.input_box.setTextCursor(cursor)
                    self.last_selection = row['span']
                    break
            else:
                # self.output_box.setText('请点选一条报文')
                self.last_selection = (0, 0)


    def _schedule_input_scan(self):
        """Debounce hex scan on large pastes."""
        self._input_scan_timer.stop()
        self._input_scan_timer.start(280)

    def _apply_input_scan(self):
        """handle with input text"""
        input_text = self.input_box.toPlainText()
        res = re.compile(r'([0-9a-fA-F]{2} ?){6,}[0-9a-fA-F]{2}')
        all_match = res.finditer(input_text)
        self.msg_find_dict = []
        find_num = 0
        for mes in all_match:
            self.msg_find_dict += [{'message': mes.group(), 'span': mes.span()}]
            find_num += 1
        self.proc_l.setText('找到报文%d条'%find_num)
        if len(self.msg_find_dict) == 1 and self.msg_find_dict[0]['message'].strip() == input_text.strip():
            self.msg_box.setPlainText(self.msg_find_dict[0]['message'])

        self.find_l.setText('')

    def _on_indent_style_changed(self):
        """Persist indent and refresh parse."""
        ind = self.indent_style_cb.currentData()
        if ind is None:
            ind = INDENT_NONE
        save_config = master_config.MasterConfig()
        save_config.set_trans_indent_style(ind)
        save_config.commit()
        self.start_trans()

    def start_trans(self):
        """start_trans"""
        if len(self.msg_box.toPlainText()) < 5:
            self.output_box.setText('请点选一条报文。\n若软件无法识别请手动复制到上方框中。')
            self._clip_cache_key = None
            self._clip_cache_text = None
            return
        if self._parser is None:
            self.output_box.setText(
                '<p style="color:red">无法加载 GDW1376_2 解析库。请将 GDW1376_2_64.dll '
                '放在本程序目录或上级 GDW1376_2 编译输出目录。</p>')
            self._clip_cache_key = None
            self._clip_cache_text = None
            return
        msg = self.msg_box.toPlainText()
        trans = Translate(msg, parser=self._parser)
        brief = trans.get_brief()
        ind = self.indent_style_cb.currentData()
        if ind is None:
            ind = INDENT_NONE
        full = trans.get_full(
            self.show_level_cb.isChecked(),
            self.show_dtype_cb.isChecked(),
            is_output_html=False,
            indent_style=ind,
        )
        self._clip_cache_key = (
            msg, ind, self.show_level_cb.isChecked(), self.show_dtype_cb.isChecked())
        self._clip_cache_text = trans.get_clipboard_text(
            self.show_level_cb.isChecked(),
            self.show_dtype_cb.isChecked(),
            indent_style=ind,
        )
        _out_style = (
            'white-space:pre-wrap;font-family:Consolas,\'Courier New\',monospace;'
            'font-size:13px;line-height:1.35;margin:0.2em 0;'
        )
        if brief == '':
            self.output_box.setText(
                '<b>【解析】</b><div style="%s">%s</div>'
                % (_out_style, parse_plain_to_display_html(full)))
        else:
            self.output_box.setText(
                '<b>【概览】</b><div style="%s">%s</div><hr>'
                '<b>【完整】</b><div style="%s">%s</div>'
                % (
                    _out_style,
                    parse_plain_to_display_html(brief),
                    _out_style,
                    parse_plain_to_display_html(full),
                ))


    def clear_box(self):
        """clear_box"""
        self.input_box.setPlainText('')
        self.output_box.setText('')
        self.setWindowTitle('10376.2日志解析工具 {ver}'.format(ver=config.MASTER_WINDOW_TITLE_ADD))
        self.input_box.setFocus()


    def search_text(self, text):
        """search_text"""
        input_text = self.input_box.toPlainText()
        res = re.compile(r'%s'%text)
        all_match = res.finditer(input_text)
        self.text_find_list = [mes.span() for mes in all_match]
        if self.text_find_list:
            self.find_l.setText('0/%d'%len(self.text_find_list))
        else:
            self.find_l.setText('未找到！')


    def find_next(self, is_setfocus=True):
        """find_next"""
        find_text = self.find_box.text()
        if not find_text:
            return
        if self.find_l.text() == '' or find_text != self.last_find_text:
            self.search_text(find_text)
        if self.find_l.text() == '未找到！':
            return
        cursor = self.input_box.textCursor()
        position = cursor.position()
        for count, text_find in enumerate(self.text_find_list, 1):
            if text_find[0] >= position:
                self.find_l.setText('%d/'%count + self.find_l.text().split('/')[1])
                cursor.setPosition(text_find[0])
                cursor.setPosition(text_find[1], QtGui.QTextCursor.KeepAnchor)
                self.input_box.setTextCursor(cursor)
                break
        else:
            self.find_l.setText('1/' + self.find_l.text().split('/')[1])
            cursor.setPosition(self.text_find_list[0][0])
            cursor.setPosition(self.text_find_list[0][1], QtGui.QTextCursor.KeepAnchor)
            self.input_box.setTextCursor(cursor)
        if is_setfocus:
            self.input_box.setFocus()

    def find_last(self, is_setfocus=True):
        """find_last"""
        find_text = self.find_box.text()
        if not find_text:
            return
        if self.find_l.text() or find_text != self.last_find_text:
            self.search_text(find_text)
        if self.find_l.text() == '未找到！':
            return
        cursor = self.input_box.textCursor()
        position = cursor.position()
        for count, text_find in enumerate(self.text_find_list[::-1], 0):
            if text_find[1] < position:
                self.find_l.setText('%d/'%(len(self.text_find_list) - count)\
                                        + self.find_l.text().split('/')[1])
                cursor.setPosition(text_find[0])
                cursor.setPosition(text_find[1], QtGui.QTextCursor.KeepAnchor)
                self.input_box.setTextCursor(cursor)
                break
        else:
            self.find_l.setText('%d/%d'%(len(self.text_find_list),\
                                    len(self.text_find_list)))
            cursor.setPosition(self.text_find_list[::-1][0][0])
            cursor.setPosition(self.text_find_list[::-1][0][1], QtGui.QTextCursor.KeepAnchor)
            self.input_box.setTextCursor(cursor)
        if is_setfocus:
            self.input_box.setFocus()


    def jump_to_msg(self, mode='next'):
        """go to next or privious msg"""
        cursor = self.input_box.textCursor()
        pos_now = int(cursor.position())
        last_msg_pos = 0
        for row in self.msg_find_dict:
            if mode == 'priv':
                if row['span'][1] < pos_now:
                    last_msg_pos = row['span'][0]
                else:
                    cursor.setPosition(last_msg_pos)
                    self.input_box.setTextCursor(cursor)
                    break
            if mode == 'next':
                if row['span'][0] > pos_now:
                    cursor.setPosition(row['span'][1] if self.auto_wrap_cb.isChecked() else row['span'][0])
                    self.input_box.setTextCursor(cursor)
                    break
        self.input_box.setFocus()


    def copy_to_clipboard(self):
        """copy_to_clipboard"""
        msg = self.msg_box.toPlainText()
        ind = self.indent_style_cb.currentData()
        if ind is None:
            ind = INDENT_NONE
        key = (msg, ind, self.show_level_cb.isChecked(), self.show_dtype_cb.isChecked())
        if self._clip_cache_key == key and self._clip_cache_text is not None:
            text = self._clip_cache_text
        else:
            trans = Translate(msg, parser=self._parser)
            text = trans.get_clipboard_text(
                self.show_level_cb.isChecked(),
                self.show_dtype_cb.isChecked(),
                indent_style=ind,
            )
        clipboard = QtWidgets.QApplication.clipboard()
        clipboard.clear()
        clipboard.setText(text)


    def set_always_top(self):
        """set_always_top"""
        window_pos = self.pos()
        if self.always_top_cb.isChecked() is True:
            self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)
            self.show()
        else:
            self.setWindowFlags(QtCore.Qt.Widget)
            self.show()
        self.move(window_pos)


    def update_wrap_mode(self):
        """update_wrap_mode"""
        self.input_box.setLineWrapMode(QtWidgets.QPlainTextEdit.WidgetWidth\
                if self.auto_wrap_cb.isChecked() else QtWidgets.QPlainTextEdit.NoWrap)


    def closeEvent(self, event):
        """close event"""
        # save config
        save_config = master_config.MasterConfig()
        save_config.set_font_size(self.input_box.get_font_size())
        ind = self.indent_style_cb.currentData()
        save_config.set_trans_indent_style(ind if ind is not None else INDENT_NONE)
        save_config.commit()

        event.accept()


if __name__ == '__main__':
    APP = QtWidgets.QApplication(sys.argv)
    dialog = TransWindow()
    dialog.show()
    APP.exec_()
    sys.exit(0)
