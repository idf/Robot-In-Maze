from serial_comminication import *
import sys

__author__ = 'Danyang'

def main(args):
    try:
        port = "/dev/ttyACM%s"%args[1]
        seria_api = SerialAPI(port=port)
    except IndexError:
        print "No command argument received, default to /dev/ttyACM0"
        seria_api = SerialAPI() # shared resources


    serialThread = SerialExecutionThread("serial", seria_api, production=False)
    # serialMessagingThread = SerialMessagingThread("msg", serialApi production=False)
    serialThread.setDaemon(False)
    serialThread.start()
    # serialMessagingThread.start()


if __name__=="__main__":
    main(sys.argv)


