from bluetooth_communication import AndroidThread
from serial_comminication import SerialCommander, SerialExecutionThread
from serial_stub import SerialCommanderStub

__author__ = 'Yanl56'


# test bluetooth and serial
if __name__=="__main__":
    print "Executing main flow"
    serial_commander = SerialCommander()
    androidT = AndroidThread("android", serial_commander, mode="control", production=False)
    # serialExecutionThread = SerialExecutionThread("serial", serial_commander, production=True)

    # serialExecutionThread.start()
    androidT.start()




