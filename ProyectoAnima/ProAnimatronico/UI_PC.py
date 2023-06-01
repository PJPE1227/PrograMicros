# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'designerJIJYCK.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(537, 429)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.pushButton = QPushButton(self.centralwidget)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setGeometry(QRect(30, 70, 111, 111))
        font = QFont()
        font.setPointSize(48)
        self.pushButton.setFont(font)
        self.pushButton_2 = QPushButton(self.centralwidget)
        self.pushButton_2.setObjectName(u"pushButton_2")
        self.pushButton_2.setGeometry(QRect(140, 70, 111, 111))
        self.pushButton_2.setFont(font)
        self.pushButton_3 = QPushButton(self.centralwidget)
        self.pushButton_3.setObjectName(u"pushButton_3")
        self.pushButton_3.setGeometry(QRect(390, 70, 111, 111))
        self.pushButton_3.setFont(font)
        self.pushButton_4 = QPushButton(self.centralwidget)
        self.pushButton_4.setObjectName(u"pushButton_4")
        self.pushButton_4.setGeometry(QRect(280, 70, 111, 111))
        self.pushButton_4.setFont(font)
        self.pushButton_5 = QPushButton(self.centralwidget)
        self.pushButton_5.setObjectName(u"pushButton_5")
        self.pushButton_5.setGeometry(QRect(140, 250, 111, 111))
        self.pushButton_5.setFont(font)
        self.pushButton_6 = QPushButton(self.centralwidget)
        self.pushButton_6.setObjectName(u"pushButton_6")
        self.pushButton_6.setGeometry(QRect(30, 250, 111, 111))
        self.pushButton_6.setFont(font)
        self.pushButton_7 = QPushButton(self.centralwidget)
        self.pushButton_7.setObjectName(u"pushButton_7")
        self.pushButton_7.setGeometry(QRect(390, 250, 111, 111))
        self.pushButton_7.setFont(font)
        self.pushButton_8 = QPushButton(self.centralwidget)
        self.pushButton_8.setObjectName(u"pushButton_8")
        self.pushButton_8.setGeometry(QRect(280, 250, 111, 111))
        self.pushButton_8.setFont(font)
        self.label = QLabel(self.centralwidget)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(60, 20, 171, 41))
        font1 = QFont()
        font1.setPointSize(20)
        font1.setBold(True)
        font1.setWeight(75)
        self.label.setFont(font1)
        self.label_2 = QLabel(self.centralwidget)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setGeometry(QRect(330, 20, 171, 41))
        self.label_2.setFont(font1)
        self.label_3 = QLabel(self.centralwidget)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setGeometry(QRect(70, 200, 171, 41))
        self.label_3.setFont(font1)
        self.label_4 = QLabel(self.centralwidget)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setGeometry(QRect(320, 200, 171, 41))
        self.label_4.setFont(font1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 537, 26))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"PANEL DE CONTROL", None))
        self.pushButton.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.pushButton_2.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.pushButton_3.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.pushButton_4.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.pushButton_5.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.pushButton_6.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.pushButton_7.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.pushButton_8.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"TORRETA", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"GARRA", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"BRAZO 1", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"BRAZO 2", None))
    # retranslateUi

