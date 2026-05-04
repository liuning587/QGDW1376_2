"""line box class"""
from PyQt4 import QtGui
from PyQt4 import QtCore


class LineNumberArea(QtGui.QWidget):

    def __init__(self, editor):
        super().__init__(editor)
        self.myeditor = editor

    def sizeHint(self):
        return QtCore.QSize(self.myeditor.lineNumberAreaWidth(), 0)

    def paintEvent(self, event):
        self.myeditor.lineNumberAreaPaintEvent(event)


class CodeEditor(QtGui.QPlainTextEdit):
    def __init__(self):
        super().__init__()
        self.lineNumberArea = LineNumberArea(self)

        self.connect(self, QtCore.SIGNAL('blockCountChanged(int)'), self.updateLineNumberAreaWidth)
        self.connect(self, QtCore.SIGNAL('updateRequest(QRect,int)'), self.updateLineNumberArea)
        self.connect(self, QtCore.SIGNAL('cursorPositionChanged()'), self.highlightCurrentLine)

        self.updateLineNumberAreaWidth(0)

        self.font_size = 9

    def lineNumberAreaWidth(self):
        digits = len(str(max(1, self.blockCount())))
        fm = self.fontMetrics()
        w9 = fm.width('9')
        return 3 + w9 * digits

    def updateLineNumberAreaWidth(self, _):
        self.setViewportMargins(self.lineNumberAreaWidth(), 0, 0, 0)

    def updateLineNumberArea(self, rect, dy):

        if dy:
            self.lineNumberArea.scroll(0, int(dy))
        else:
            self.lineNumberArea.update(
                0, int(rect.y()), self.lineNumberArea.width(), int(rect.height()))

        if rect.contains(self.viewport().rect()):
            self.updateLineNumberAreaWidth(0)

    def resizeEvent(self, event):
        super().resizeEvent(event)

        cr = self.contentsRect()
        self.lineNumberArea.setGeometry(QtCore.QRect(cr.left(), cr.top(),
                    self.lineNumberAreaWidth(), cr.height()))

    def lineNumberAreaPaintEvent(self, event):
        painter = QtGui.QPainter(self.lineNumberArea)
        try:
            painter.fillRect(event.rect(), QtCore.Qt.lightGray)

            block = self.firstVisibleBlock()
            blockNumber = block.blockNumber()
            geo = self.blockBoundingGeometry(block).translated(self.contentOffset())
            top = int(geo.top())
            bottom = int(top + self.blockBoundingRect(block).height())

            height = int(self.fontMetrics().height())
            while block.isValid() and (top <= event.rect().bottom()):
                if block.isVisible() and (bottom >= event.rect().top()):
                    number = str(blockNumber + 1)
                    painter.setPen(QtCore.Qt.black)
                    painter.drawText(
                        0, top, self.lineNumberArea.width(), height,
                        QtCore.Qt.AlignRight, number)

                block = block.next()
                top = bottom
                bottom = int(top + self.blockBoundingRect(block).height())
                blockNumber += 1
        finally:
            if painter.isActive():
                painter.end()

    def highlightCurrentLine(self):
        extraSelections = []

        if not self.isReadOnly():
            selection = QtGui.QTextEdit.ExtraSelection()

            lineColor = QtGui.QColor(QtCore.Qt.yellow).lighter(160)

            selection.format.setBackground(lineColor)
            selection.format.setProperty(QtGui.QTextFormat.FullWidthSelection, True)
            selection.cursor = self.textCursor()
            selection.cursor.clearSelection()
            extraSelections.append(selection)
        self.setExtraSelections(extraSelections)

    def set_font_size(self, font_size):
        """set font size"""
        self.font_size = font_size
        font = QtGui.QFont("Courier", self.font_size)
        self.setFont(font)

    def get_font_size(self):
        """get font size"""
        return self.font_size

    def zoomOut(self):
        """change font"""
        self.font_size = max(self.font_size - 1, 1)
        font = QtGui.QFont("Courier", self.font_size)
        self.setFont(font)

    def zoomIn(self):
        """change font"""
        self.font_size = min(self.font_size + 1, 30)
        font = QtGui.QFont("Courier", self.font_size)
        self.setFont(font)
