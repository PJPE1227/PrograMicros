from PySide2.QtCore import QTimer
from PySide2.QtWidgets import QMainWindow, QApplication
from uiv2 import Ui_MainWindow
import serial

class Mainwidow(QMainWindow, Ui_MainWindow):
	def __init__(self):
		super().__init__()
		self.setupUi(self)
		
		self.serial_port = serial.Serial('COM3', 9600)
		
		self.pushButton.clicked.connect(self.send_data)
		
		self.timer = QTimer(self)
		self.timer.timeout.connect(self.receive_data)
		self.timer.start(100)
		
	def send_data(self):
		if self.serial_port is not None:
			data = self.lineEdit.text().encode()
			self.serial_port.write(data)
			
	def receive_data(self):
		if self.serial_port is not None:
			data = self.serial_port.read_all().decode()
			if data:
				self.textBrowser.append(data)
if __name__ == '__main__':
	app = QApplication()
	window = Mainwidow()
	window.show()
	app.exec_()