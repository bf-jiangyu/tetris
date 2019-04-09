#-*- coding: utf-8 -*-
import os
from PyQt5.QtWidgets import QMainWindow, QAction, QDesktopWidget
from PyQt5.QtCore import Qt
from ui.ui_mainwindow import Ui_MainWindow
from gamepool import GamepoolWidget
from game_statistics import GameStateWidget
from keyconfig import KeyConfig

class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)

        self.setFixedSize(self.size())

        self.__center()

        self.pool_contianer = GamepoolWidget(self, self.poolContianerWidget)
        self.startBtn.clicked.connect(self.pool_contianer.start_game)
        self.key_map = self.read_key_config()

        self.state_widget = GameStateWidget(self.stateWidget)
        self.state_widget.update()

        self.actionKey_Config.triggered.connect(self.__key_config)


    def __center(self):
        geometry = self.frameGeometry()
        screen_index = 1
        center_pointer = QDesktopWidget().availableGeometry(screen_index).center()
        geometry.moveCenter(center_pointer)
#        self.move(geometry.topLeft())
        self.move(0, 0)


    def __key_config(self):
        key_config = KeyConfig(self)
        key_config.show()


    def read_key_config(self):
        key_map = {}
        if os.path.exists('./config/key_config.txt'):
            with open('./config/key_config.txt', 'r') as f_config:
                lines = f_config.readlines()
                for line in lines:
                    line = line.strip()
                    if not line:
                        continue
                    key_value = line.split('=')
                    key_map[key_value[0]] = key_value[1]
        else:
            print('no config')

        return key_map
