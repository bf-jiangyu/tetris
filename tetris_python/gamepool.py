# -*- coding: utf-8 -*-
import time
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter, QPixmap, QFont
from PyQt5.QtCore import Qt, QRectF
from block.block import Block
from block.block import CELL_WIDTH

CELL_WIDTH = 22

class GamepoolWidget(QWidget):

    def __init__(self, mainwindow, parent):
        super(GamepoolWidget, self).__init__(parent)
        self.resize(parent.size())
        self.mainwindow = mainwindow

        self.direct_down = False
        self.gameover = False
        self.stats = None

        self.gamepool = Gamepool(mainwindow.blockPoolWidget, self)
        self.next_hint = NextHint(mainwindow.nextHintWidget, self)
        self.last_hint = LastHint(mainwindow.lastHintWidget, self)
        self.speed_hint = SpeedHint(mainwindow.speedHintWidget, self)
        self.start_game()


    def start_game(self):
        self.gameover = False
        self.direct_down = False
        self.stats = Stats()
        self.stats.time_start = time.time()

        self.gamepool.start_game()
        self.next_hint.start_game()
        self.last_hint.start_game()
        self.speed_hint.start_game()

        self.setFocus()


    def keyReleaseEvent(self, event):
        if event.key() == ord(self.mainwindow.key_map.get('down')):
            self.direct_down = False

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_F2 or event.key() == Qt.Key_R:
            self.mainwindow.startBtn.click()
            return
        elif self.gameover:
            return

        self.stats.count_click += 1

        if event.key() == ord(self.mainwindow.key_map.get('left')):
            if self.direct_down:
                self.gamepool.block.move_bottom()
                self.gamepool.block.move_left_end()
                self.__put_down_block()
            else:
                self.gamepool.block.move_left()
        elif event.key() == ord(self.mainwindow.key_map.get('right')):
            if self.direct_down:
                self.gamepool.block.move_bottom()
                self.gamepool.block.move_right_end()
                self.__put_down_block()
            else:
                self.gamepool.block.move_right()
        elif event.key() == ord(self.mainwindow.key_map.get('down')):
            self.gamepool.block.move_down()
            self.direct_down = True
        elif event.key() == ord(self.mainwindow.key_map.get('turnAnti')):
            self.gamepool.block.turn_s()
        elif event.key() == ord(self.mainwindow.key_map.get('turn')):
            self.gamepool.block.turn_d()
        elif event.key() == ord(self.mainwindow.key_map.get('left3')):
            self.gamepool.block.move_left_3()
        elif event.key() == ord(self.mainwindow.key_map.get('leftDirect')):
            self.gamepool.block.move_left_end()
        elif event.key() == ord(self.mainwindow.key_map.get('right3')):
            self.gamepool.block.move_right_3()
        elif event.key() == ord(self.mainwindow.key_map.get('rightDirect')):
            self.gamepool.block.move_right_end()
        elif event.key() == Qt.Key_Space:
            self.__put_down_block()
        self.gamepool.update()


    def __put_down_block(self):
        self.gamepool.block.move_bottom()
        self.gamepool.put_block()
        self.gamepool.block = self.next_hint.block
        self.gamepool.block.show_in_pool(self.gamepool.pool_blocks)
        self.next_hint.block = self.last_hint.block
        self.last_hint.block = Block()

        self.gamepool.check_gameover()

        self.stats.count_block += 1

        self.next_hint.update()
        self.last_hint.update()
        self.speed_hint.update()


    def game_over(self):
        if self.gameover:
            return

        self.gameover = True
        stats = self.stats
        total_stats = self.mainwindow.state_widget

        total_stats.total_block += stats.count_block
        total_stats.total_click += stats.count_click
        total_stats.total_line_useful += stats.clear_line_useful
        total_stats.total_line_all += stats.clear_line_all
        total_stats.total_time += time.time() - stats.time_start

        if stats.clear_line_useful >= total_stats.success_limit:
            total_stats.success_times += 1

        total_stats.game_times += 1
        total_stats.update()


class Gamepool(QWidget):

    POOL_WIDTH = 12
    POOL_HEIGHT = 21

    def __init__(self, parent, pool_widget):
        super(Gamepool, self).__init__(parent)
        self.resize(parent.size())
        self.pool_widget = pool_widget

        self.block = None
        self.pool_blocks = None
        self.timer_auto_rise = None


    def start_game(self):
        self.block = Block()
        self.pool_blocks = [['' for i in range(self.POOL_WIDTH)] for i in range(self.POOL_HEIGHT)]
        self.block.show_in_pool(self.pool_blocks)

        self.timer_auto_rise = self.startTimer(4000)

        self.update()


    def timerEvent(self, event):
        if event.timerId() == self.timer_auto_rise:
            self.__auto_rise_line()
            self.__auto_rise_line(1)
            self.update()
            if self.pool_widget.gameover:
                self.pool_widget.next_hint.update()
                self.pool_widget.last_hint.update()
                self.pool_widget.speed_hint.update()


    def paintEvent(self, event):
        painter = QPainter(self)
        if self.pool_widget.gameover:
            painter.setOpacity(0.5)
        self.block.draw_block(painter)

        for pos_y in range(self.POOL_HEIGHT):
            for pos_x in range(self.POOL_WIDTH):
                img = self.pool_blocks[pos_y][pos_x]
                if img:
                    pixmap = QPixmap('./block/' + img)
                    painter.drawPixmap(pos_x * CELL_WIDTH, pos_y * CELL_WIDTH,\
                        CELL_WIDTH, CELL_WIDTH, pixmap)

        if self.pool_widget.gameover:
            painter.setOpacity(1)
            self.__draw_game_over(painter)


    def __draw_game_over(self, painter):
        painter.setPen(Qt.white)
        painter.setFont(QFont('黑体', 24, QFont.Bold, False))
        pool_height = self.POOL_HEIGHT * CELL_WIDTH
        pool_width = self.POOL_WIDTH * CELL_WIDTH
        painter.drawText(0, (pool_height / 2 - 10), pool_width, 36, Qt.AlignHCenter|Qt.AlignVCenter, '游戏结束')

        painter.setFont(QFont('黑体', 12, QFont.Normal, False))

        margin_top = pool_height / 2 + 50
        offset = 25

        rect = QRectF(0, margin_top, pool_width, 18)
        stats = self.pool_widget.stats

        def test(message):
            nonlocal margin_top
            margin_top += offset
            rect.moveBottom(margin_top)
            painter.drawText(rect, Qt.AlignHCenter|Qt.AlignVCenter, message)

        test('块效率：%.2f' % (stats.clear_line_useful * 3 / stats.count_block))
        test('有用消层：%d' % (stats.clear_line_useful))
        test('总消层：%d' % (stats.clear_line_all))
        test('按键速度：%.2f' % (stats.count_click / (time.time() - stats.time_start)))


    def __auto_rise_line(self, first=0):
        if self.check_gameover():
            return
#        for col in range(self.POOL_WIDTH):
#            if self.pool_blocks[1][col]:
#                self.pool_widget.gameover = True

        for row in range(1, self.POOL_HEIGHT):
            for col in range(self.POOL_WIDTH):
                self.pool_blocks[row - 1][col] = self.pool_blocks[row][col]
        bottom = self.POOL_HEIGHT - 1
        for col in range(1 - first, self.POOL_WIDTH, 2):
            self.pool_blocks[bottom][col] = ''
        for col in range(first, self.POOL_WIDTH, 2):
            self.pool_blocks[bottom][col] = 'N.png'
        self.check_gameover()


    def put_block(self):
        for cell in self.block.cells:
            self.pool_blocks[int(cell[1])][int(cell[0])] = self.block.img
        self.check_clear()


    def check_clear(self):
        clear_count = 0
        for row in range(self.POOL_HEIGHT):
            count = 0
            for col in range(self.POOL_WIDTH):
                if self.pool_blocks[row][col]:
                    count += 1

            if count == self.POOL_WIDTH:
                clear_count += 1
                self.__clear_one_line(row)

        self.pool_widget.stats.clear_line_all += clear_count
        if clear_count >= 3:
            self.pool_widget.stats.clear_line_useful += clear_count


    def __clear_one_line(self, row):
        for col in range(self.POOL_WIDTH):
            self.pool_blocks[row][col] = ''

        while row:
            for col in range(self.POOL_WIDTH):
                self.pool_blocks[row][col] = self.pool_blocks[row - 1][col]
            row -= 1


    def check_gameover(self):
        rtn = False
        def game_over():
            self.pool_widget.game_over()
            if self.timer_auto_rise:
                self.killTimer(self.timer_auto_rise)
                self.timer_auto_rise = 0
            nonlocal rtn
            rtn = True

        for cell in self.block.cells:
            if self.pool_blocks[cell[1]][cell[0]]:
                self.pool_blocks[cell[1]][cell[0]] = ''
                game_over()
                break
        for point in self.pool_blocks[0]:
            if point != '':
                game_over()
                break
        return rtn


class NextHint(QWidget):

    def __init__(self, parent, pool_widget):
        super(NextHint, self).__init__(parent)
        self.resize(parent.size())
        self.pool_widget = pool_widget

        self.block = None


    def start_game(self):
        self.block = Block()
        self.update()


    def paintEvent(self, event):
        painter = QPainter(self)
        if self.pool_widget.gameover:
            painter.setOpacity(0.5)
        self.block.draw_block(painter)


class LastHint(QWidget):

    def __init__(self, parent, pool_widget):
        super(LastHint, self).__init__(parent)
        self.resize(parent.size())
        self.pool_widget = pool_widget

        self.block = None


    def start_game(self):
        self.block = Block()
        self.update()


    def paintEvent(self, event):
        painter = QPainter(self)
        if self.pool_widget.gameover:
            painter.setOpacity(0.5)
        self.block.draw_block(painter)


class SpeedHint(QWidget):

    def __init__(self, parent, pool_widget):
        super(SpeedHint, self).__init__(parent)
        self.resize(parent.size())
        self.pool_widget = pool_widget

        self.font = QFont()
        self.font.setPointSize(30)


    def start_game(self):
        self.update()


    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setPen(Qt.white)
        painter.setFont(self.font)

        stats = self.pool_widget.stats
        speed = str('%.2f' % (stats.count_block / (time.time() - stats.time_start)))
        painter.drawText(self.rect(), Qt.AlignHCenter|Qt.AlignVCenter, speed)


class Stats():

    def __init__(self):
        self.count_block = 0# 落块数
        self.count_click = 0# 点击次数
        self.clear_line_useful = 0# 有用消层
        self.clear_line_all = 0# 总消层
        self.time_start = 0# 游戏开始时间
