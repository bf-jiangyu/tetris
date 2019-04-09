# -*- coding: utf-8 -*-
from PyQt5.QtWidgets import QDialog
from ui.ui_keyconfig import Ui_KeyConfig

class KeyConfig(QDialog, Ui_KeyConfig):

    def __init__(self, parent):
        QDialog.__init__(self)
        Ui_KeyConfig.__init__(self)
        super(KeyConfig, self).__init__(parent)
        self.setupUi(self)

        self.button_save.clicked.connect(self.__save_key_config)
        self.button_close.clicked.connect(self.close)

        self.parent = parent

        self.__set_key_value()


    def __save_key_config(self):
        key_map = {}
        key_map['left'] = self.lineEdit_left.text().upper()
        key_map['right'] = self.lineEdit_right.text().upper()
        key_map['left3'] = self.lineEdit_left_3.text().upper()
        key_map['right3'] = self.lineEdit_right_3.text().upper()
        key_map['leftDirect'] = self.lineEdit_left_direct.text().upper()
        key_map['rightDirect'] = self.lineEdit_right_direct.text().upper()
        key_map['down'] = self.lineEdit_down.text().upper()
        key_map['bottom'] = self.lineEdit_bottom.text().upper()
        key_map['turn'] = self.lineEdit_turn.text().upper()
        key_map['turnAnti'] = self.lineEdit_turn_anti.text().upper()

        with open('./config/key_config.txt', 'w') as f_config:
            for key, value in key_map.items():
                f_config.writelines(key + '=' + value + '\r\n')

        self.parent.key_map = key_map


    def __set_key_value(self):
        key_map = self.parent.read_key_config()
        self.lineEdit_left.setText(key_map['left'])
        self.lineEdit_right.setText(key_map['right'])
        self.lineEdit_left_3.setText(key_map['left3'])
        self.lineEdit_right_3.setText(key_map['right3'])
        self.lineEdit_left_direct.setText(key_map['leftDirect'])
        self.lineEdit_right_direct.setText(key_map['rightDirect'])
        self.lineEdit_down.setText(key_map['down'])
        self.lineEdit_bottom.setText(key_map['bottom'])
        self.lineEdit_turn.setText(key_map['turn'])
        self.lineEdit_turn_anti.setText(key_map['turnAnti'])
