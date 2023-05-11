import serial
from Adafruit_IO import Client, RequestError, Feed

ADAFRUIT_IO_KEY = 'aio_FeMZ373KYnClSALYqTaMKOM34p8B'
ADAFRUIT_IO_USERNAME = 'x1134x'
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
ser = serial.Serial("COM3",9600,timeout=(1))
counter_temp=255
slider_temp = 255
counter_IO = aio.feeds('var00')
slider_IO = aio.feeds('var01')
while True:
    counter = ser.read(1)
    int_counter = int.from_bytes(counter, "big")
    slider_data = int(aio.receive(slider_IO.key).value)
    slider_data_byte = bytes([slider_data])
    if slider_temp != slider_data:
        ser.write(slider_data_byte)
        #print(int_counter)
        slider_temp = slider_data
        #print(slider_data)
    if int_counter == 0:
        continue
    else:
        while counter_temp != int_counter:
            #counter = ser.read(3)
            counter_temp = int_counter
            aio.send_data(counter_IO.key, int_counter)
            print(int_counter)