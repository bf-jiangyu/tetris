# -*- coding: utf-8 -*-
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter
from PyQt5.QtCore import Qt, QRectF

class GameStateWidget(QWidget):

    def __init__(self, parent):
        super(GameStateWidget, self).__init__(parent)
        self.resize(parent.size())

        self.total_block = 0# 落块数
        self.total_click = 0# 点击次数
        self.total_line_useful = 0# 有用消层
        self.total_line_all = 0# 总消层
        self.total_time = 0# 游戏时间

        self.success_times = 0
        self.success_limit = 20

        self.game_times = 0


    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setPen(Qt.white)
        margin_top = 40
        offset = 20
        rect = QRectF(0, margin_top, 12 * 22, 18)

        def test(message):
            nonlocal margin_top
            rect.moveBottom(margin_top)
            painter.drawText(rect, Qt.AlignHCenter|Qt.AlignVCenter, message)
            margin_top += offset

        test('游戏时长：%d' % (self.total_time))
        speed = 0 if self.game_times == 0 else self.total_block / self.total_time
        test('速度：%.2f' % (speed))

        margin_top += 10
        test('成功盘数：%d' % (self.success_times))
        test('盘数：%d' % (self.game_times))

        margin_top += 10
        test('攻击消层：%d' % (self.total_line_useful))
        test('消层：%d' % (self.total_line_all))
        useful = 1 if self.total_block == 0 else self.total_line_useful * 3 / self.total_block
        test('攻击效率：%.2f' % (useful))

        margin_top += 10
        speed = 0 if self.total_time == 0 else self.total_click / self.total_time
        test('按键速度：%.2f' % (speed))
