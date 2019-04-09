# -*- coding: utf-8 -*-
import random
from PyQt5.QtGui import QPixmap

CELL_WIDTH = 22

class Block():

    def __init__(self, shape=-1):
        self.x = float(0)
        self.y = float(0)
        self.cells = []
        self.img = ''
        if shape == -1:
            shape = random.randint(0, 6)
        self.shape = shape
        self.status = 0

        self.pool_blocks = []

        self.__new_by_shape()

    def __new_by_shape(self):
        if self.shape == 0:
            self.x = 2
            self.y = 1
            self.img = './T.png'
            self.__init_t()
        elif self.shape == 1:
            self.x = 2.5
            self.y = 1.5
            self.img = './L.png'
            self.__init_l()
        elif self.shape == 2:
            self.x = 1.5
            self.y = 1.5
            self.img = './J.png'
            self.__init_j()
        elif self.shape == 3:
            self.x = 2
            self.y = 2
            self.img = './Z.png'
            self.__init_z()
        elif self.shape == 4:
            self.x = 2
            self.y = 2
            self.img = './S.png'
            self.__init_s()
        elif self.shape == 5:
            self.x = 1.5
            self.y = 1.5
            self.img = './I.png'
            self.__init_i()
        elif self.shape == 6:
            self.x = 1.5
            self.y = 1
            self.img = './O.png'
            self.__init_o()


    def __init_t(self):
        x = self.x
        y = self.y
        cells = []
        cells.append([x, y])
        if self.status == 0:
            cells.append([x - 1, y])
            cells.append([x + 1, y])
            cells.append([x, y + 1])
        elif self.status == 1:
            cells.append([x, y - 1])
            cells.append([x - 1, y])
            cells.append([x, y + 1])
        elif self.status == 2:
            cells.append([x, y - 1])
            cells.append([x - 1, y])
            cells.append([x + 1, y])
        elif self.status == 3:
            cells.append([x, y - 1])
            cells.append([x + 1, y])
            cells.append([x, y + 1])
        self.cells = cells


    def __init_l(self):
        x = self.x
        y = self.y
        cells = []
        cells.append([x, y])
        if self.status == 0:
            cells.append([x - 1, y - 1])
            cells.append([x, y - 1])
            cells.append([x, y + 1])
        elif self.status == 1:
            cells.append([x + 1, y - 1])
            cells.append([x - 1, y])
            cells.append([x + 1, y])
        elif self.status == 2:
            cells.append([x, y - 1])
            cells.append([x, y + 1])
            cells.append([x + 1, y + 1])
        elif self.status == 3:
            cells.append([x - 1, y])
            cells.append([x + 1, y])
            cells.append([x - 1, y + 1])
        self.cells = cells

    def __init_j(self):
        x = self.x
        y = self.y
        cells = []
        cells.append([x, y])
        if self.status == 0:
            cells.append([x + 1, y - 1])
            cells.append([x, y - 1])
            cells.append([x, y + 1])
        elif self.status == 1:
            cells.append([x + 1, y + 1])
            cells.append([x - 1, y])
            cells.append([x + 1, y])
        elif self.status == 2:
            cells.append([x, y - 1])
            cells.append([x, y + 1])
            cells.append([x - 1, y + 1])
        elif self.status == 3:
            cells.append([x - 1, y])
            cells.append([x + 1, y])
            cells.append([x - 1, y - 1])
        self.cells = cells

    def __init_z(self):
        x = self.x
        y = self.y
        cells = []
        cells.append([x, y])
        if self.status == 0:
            cells.append([x - 1, y - 1])
            cells.append([x, y - 1])
            cells.append([x + 1, y])
        elif self.status == 1:
            cells.append([x + 1, y - 1])
            cells.append([x + 1, y])
            cells.append([x, y + 1])
        self.cells = cells

    def __init_s(self):
        x = self.x
        y = self.y
        cells = []
        cells.append([x, y])
        if self.status == 0:
            cells.append([x + 1, y - 1])
            cells.append([x, y - 1])
            cells.append([x - 1, y])
        elif self.status == 1:
            cells.append([x, y - 1])
            cells.append([x + 1, y])
            cells.append([x + 1, y + 1])
        self.cells = cells

    def __init_i(self):
        x = self.x
        y = self.y
        cells = []
        cells.append([x, y])
        if self.status == 0:
            cells.append([x + 1, y])
            cells.append([x + 2, y])
            cells.append([x - 1, y])
        elif self.status == 1:
            cells.append([x + 1, y - 1])
            cells.append([x + 1, y + 1])
            cells.append([x + 1, y + 2])
            cells[0][0] = x + 1
        self.cells = cells

    def __init_o(self):
        x = self.x
        y = self.y
        cells = []
        cells.append([x, y])
        cells.append([x + 1, y])
        cells.append([x + 1, y + 1])
        cells.append([x, y + 1])
        self.cells = cells


    def move_left(self):
        self.x -= 1
        self.__reset_block()
        if not self.__check_border() or not self.__check_pool_blocks():
            self.move_right()
            return False
        return True
    def move_left_3(self):
        times = 0
        while times < 3 and self.move_left():
            times += 1
    def move_left_end(self):
        while self.move_left():
            pass


    def move_right(self):
        self.x += 1
        self.__reset_block()
        if not self.__check_border() or not self.__check_pool_blocks():
            self.move_left()
            return False
        return True
    def move_right_3(self):
        times = 0
        while times < 3 and self.move_right():
            times += 1
    def move_right_end(self):
        while self.move_right():
            pass


    def move_down(self):
        self.y += 1
        self.__reset_block()
        if not self.__check_border() or not self.__check_pool_blocks():
            self.move_top()
            return False
        return True


    def move_bottom(self):
        while self.move_down():
            pass


    def move_top(self):
        self.y -= 1
        self.__reset_block()
        if not self.__check_border():
            self.move_down()


    def turn_d(self):
        self.status += 1
        self.__reset_block()
        if not self.__check_border() or not self.__check_pool_blocks():
            self.turn_s()


    def turn_s(self):
        self.status -= 1
        self.__reset_block()
        if not self.__check_border() or not self.__check_pool_blocks():
            self.turn_d()


    def show_in_pool(self, pool_blocks):
        self.x = 5
        self.y = 1
        self.status = 0
        self.__reset_block()
        self.pool_blocks = pool_blocks


    def __check_pool_blocks(self):
        for cell in self.cells:
            if self.pool_blocks[cell[1]][cell[0]]:
                return False
        return True


    def __check_border(self):
        for cell in self.cells:
            if cell[0] < 0 or cell[0] > 11 or cell[1] < 0 or cell[1] > 20:
                return False

        return True


    def __reset_block(self):
        max_status = 3
        if self.shape > 2:
            max_status = 1

        if self.shape == 6:
            self.status = 0
        else:
            if self.status > max_status:
                self.status = 0
            elif self.status < 0:
                self.status = max_status

        if self.shape == 0:
            self.__init_t()
        elif self.shape == 1:
            self.__init_l()
        elif self.shape == 2:
            self.__init_j()
        elif self.shape == 3:
            self.__init_z()
        elif self.shape == 4:
            self.__init_s()
        elif self.shape == 5:
            self.__init_i()
        elif self.shape == 6:
            self.__init_o()


    def draw_block(self, painter):
        pixmap = QPixmap('./block/' + self.img)
        for cell in self.cells:
            painter.drawPixmap(cell[0] * CELL_WIDTH, cell[1] * CELL_WIDTH,\
                    CELL_WIDTH, CELL_WIDTH, pixmap)
