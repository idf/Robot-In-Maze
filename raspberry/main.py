from bluetooth_communication import androidThread
from serial_comminication import SerialCommander, SerialExecutionThread

__author__ = 'Yanl56'



if __name__=="__main__":
    print "Executing main flow"
    serial_commander = SerialCommander()
    androidT = androidThread("android", serial_commander)
    serialExecutionThread = SerialExecutionThread("serial", serial_commander, production=True)

    serialExecutionThread.start()
    androidT.start()




