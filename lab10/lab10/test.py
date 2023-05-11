import time, random, serial

from Adafruit_IO import Client, Feed, RequestError

ADAFRUIT_IO_KEY = 'aio_FeMZ373KYnClSALYqTaMKOM34p8B'

ADAFRUIT_IO_USERNAME = 'x1134x'

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

ser = serial.Serial('COM3', 9600, timeout=(1))

try:
	feed_00 = aio.feeds('var00')
except RequestError:
	feed = Feed(name='var00')
	feed_00 = aio.create_feed(feed)
    
while True:
    #var = random.randrange(0,100)
    var = ser.read(1)
    int_var = int.from_bytes(var, "big")
    if int_var != 0:
        print(f"Enviando contador al Feed {feed_00}: {int_var}")
        aio.send_data('var00', int_var)