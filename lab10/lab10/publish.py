import time

# Import Adafruit IO REST client.
from Adafruit_IO import Client, Feed, RequestError

# holds the count for the feed
run_count = 0

ADAFRUIT_IO_KEY = 'aio_FeMZ373KYnClSALYqTaMKOM34p8B'

ADAFRUIT_IO_USERNAME = 'x1134x'

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

try:
    feed_00 = aio.feeds('counter')
except RequestError:
    feed = Feed(name='counter')
    feed_00 = aio.create_feed(feed)


while True:
    print('sending count: ', run_count)
    run_count += 1
    aio.send(feed_00.key, run_count))
    time.sleep(3)