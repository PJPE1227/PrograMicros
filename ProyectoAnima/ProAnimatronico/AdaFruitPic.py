import serial
from Adafruit_IO import Client, RequestError, Feed

command1 = 'u'
command2 = 'z'
command3 = 't'
command4 = 'y'
command5 = 'p'
command6 = 'g'
command7 = 'r'
command8 = 'n'

data1 = command1.encode()
data2 = command2.encode()
data3 = command3.encode()
data4 = command4.encode()
data5 = command5.encode()
data6 = command6.encode()
data7 = command7.encode()
data8 = command8.encode()

ADAFRUIT_IO_KEY = 'aio_uIoU75lpYPPm5WU9ljirDEacARqS'
ADAFRUIT_IO_USERNAME = 'x1134x'

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
ser = serial.Serial("COM3",9600,timeout=(1))

but0 = aio.feeds('testbutton')
but1 = aio.feeds('testbutton1')
but2 = aio.feeds('testbutton2')
but3 = aio.feeds('testbutton3')
but4 = aio.feeds('testbutton4')
but5 = aio.feeds('testbutton5')
but6 = aio.feeds('testbutton6')
but7 = aio.feeds('testbutton7')

while True:
    button0 = int(aio.receive(but0.key).value)
    if button0 == 1:
        ser.write(data3)
        button0 = 0
        print(data3)
        
    button1 = int(aio.receive(but1.key).value)
    if button1 == 1:
        ser.write(data4)
        print(data4)
        button1 = 0
        
    button2 = int(aio.receive(but2.key).value)
    if button2 == 1:
        ser.write(data1)
        print(data1)
        button2 = 0
        
    button3 = int(aio.receive(but3.key).value)
    if button3 == 1:
        ser.write(data2)
        print(data2)
        button3 = 0
        
    button4 = int(aio.receive(but4.key).value)
    if button4 == 1:
        ser.write(data5)
        print(data5)
        button4 = 0
        
    button5 = int(aio.receive(but5.key).value)
    if button5 == 1:
        ser.write(data6)
        print(data6)
        button5 = 0
        
    button6 = int(aio.receive(but6.key).value)
    if button6 == 1:
        ser.write(data7)
        print(data7)
        button6 = 0
        
    button7 = int(aio.receive(but7.key).value)
    if button7 == 1:
        ser.write(data8)
        print(data8)
        button7 = 0