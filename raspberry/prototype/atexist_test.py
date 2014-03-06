import time

__author__ = 'Yanl56'

class Test(object):
    msg = "Test Class"


def goodbye(msg):
    print msg

test = Test()
import atexit
atexit.register(goodbye, test.msg)
while True:
    time.sleep(1)
    pass

