import sys, serial, time

from Adafruit_IO import MQTTClient, Client, Feed, RequestError

ADAFRUIT_IO_KEY = 'aio_FeMZ373KYnClSALYqTaMKOM34p8B'

ADAFRUIT_IO_USERNAME = 'x1134x'

FEED_ID = 'var01'

def connected(client):
    print('Subscribing to Feed {0}'.format(FEED_ID))
    client.subscribe(FEED_ID)
    print('Waiting for feed data...')

def disconnected(client):
    sys.exit(1)

def message(client, feed_id, payload):
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

client.connect()
slider_IO = aio.feeds('var01')
ser = serial.Serial('COM3', 9600, timeout =(1))
sliderInterval = 1;
lastSliderSendTime = time.time()
counter_temp = 255
slider_temp = 255
try:
	feed_00 = aio.feeds('var00')
except RequestError:
	feed = Feed(name='var00')
	feed_00 = aio.create_feed(feed)

while True:
    var = ser.read(1)
    int_var = int.from_bytes(var, "big")
    slider_data = int(aio.receive(slider_IO.key).value)
    slider_data_byte= bytes([slider_data])
    if slider_temp != slider_data:
        ser.write(slider_data_byte)
        slider_temp = slider_data
        
    print(slider_data)
    if int_var == 0:
        continue
    else:
        while counter_temp != int_var:
            var = ser.read(3)
            counter_temp = int_var
            aio.send_data('var00', int_var)
            print(int_var)