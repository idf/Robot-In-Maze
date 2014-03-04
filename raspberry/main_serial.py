from serial_comminication import *
import sys

__author__ = 'Danyang'

def main(args):
    try:
        port = "/dev/ttyACM%s"%args[1]
        serialCommander = SerialCommander(port=port)
    except IndexError:
        print "No command argument received, default to /dev/ttyACM0"
        serialCommander = SerialCommander() # shared resources


    serialThread = SerialExecutionThread("serial", serialCommander, production=False)
    # serialMessagingThread = SerialMessagingThread("msg", serialCommander, production=False)
    serialThread.start()
    # serialMessagingThread.start()


if __name__=="__main__":
    main(sys.argv)


