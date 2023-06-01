from PySide2.QtCore import QTimer
from PySide2.QtWidgets import QMainWindow, QApplication
from UI_PC import Ui_MainWindow
import serial

command1 = 's'
command2 = 'a'
command3 = 'x'
command4 = 'o'
command5 = 'q'
command6 = 'w'
command7 = 'd'
command8 = 'f'

data1 = command1.encode()
data2 = command2.encode()
data3 = command3.encode()
data4 = command4.encode()
data5 = command5.encode()
data6 = command6.encode()
data7 = command7.encode()
data8 = command8.encode()

class Mainwidow(QMainWindow, Ui_MainWindow):
	def __init__(self):
		super().__init__()
		self.setupUi(self)
		self.pushButton.clicked.connect(self.send_data3)
		self.pushButton_2.clicked.connect(self.send_data4)
		self.pushButton_3.clicked.connect(self.send_data1)
		self.pushButton_4.clicked.connect(self.send_data2)
		self.pushButton_5.clicked.connect(self.send_data5)
		self.pushButton_6.clicked.connect(self.send_data6)
		self.pushButton_7.clicked.connect(self.send_data7)
		self.pushButton_8.clicked.connect(self.send_data8)
		self.serial_port = serial.Serial('COM3', 9600)    
        
	
	def send_data1(self):
		if self.serial_port is not None:
			self.serial_port.write(data1)
	def send_data2(self):
		if self.serial_port is not None:
			self.serial_port.write(data2)

	def send_data3(self):
		if self.serial_port is not None:
			self.serial_port.write(data3)

	def send_data4(self):
		if self.serial_port is not None:
			self.serial_port.write(data4)

	def send_data5(self):
		if self.serial_port is not None:
			self.serial_port.write(data5)

	def send_data6(self):
		if self.serial_port is not None:
			self.serial_port.write(data6)

	def send_data7(self):
		if self.serial_port is not None:
			self.serial_port.write(data7)

	def send_data8(self):
		if self.serial_port is not None:
			self.serial_port.write(data8)      
        
if __name__ == '__main__':
	app = QApplication()
	window = Mainwidow()
	window.show()
	app.exec_()