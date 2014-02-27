from serial_comminication import SerialThread

__author__ = 'Danyang'

if __name__=="__main__":
    print "Executing main flow"
    serialThread = SerialThread("serial")
    serialThread.start()
