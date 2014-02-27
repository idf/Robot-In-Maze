from serial_comminication import *

__author__ = 'Danyang'

def main():
    serialCommander = SerialCommander() # shared resources
    serialThread = SerialThread("serial", serialCommander, production=False)
    serialThread.start()


if __name__=="__main__":
    main()


