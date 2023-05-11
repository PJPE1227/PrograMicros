# Import standard python modules.
import sys, serial
import time, random, serial

from Adafruit_IO import Client, Feed, RequestError

# This example uses the MQTTClient instead of the REST client
from Adafruit_IO import MQTTClient

ADAFRUIT_IO_KEY = 'aio_FeMZ373KYnClSALYqTaMKOM34p8B'

ADAFRUIT_IO_USERNAME = 'x1134x'

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

ser = serial.Serial('COM3', 9600)

start_time = time.time()

try:
	feed_00 = aio.feeds('var00')
except RequestError:
	feed = Feed(name='var00')
	feed_00 = aio.create_feed(feed)

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

client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

client.connect()

client.loop_background()
time.sleep(2)

while True:
    var = random.randrange(0,100)
    #var = ser.read().decode().strip()
    aio.send(feed_00.key, var)
    print('sent -> '+str(var))
    time.sleep(2)