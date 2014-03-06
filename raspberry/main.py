from bluetooth_communication import androidThread
from serial_comminication import SerialCommander, SerialExecutionThread
from serial_stub import SerialCommanderStub

__author__ = 'Yanl56'



if __name__=="__main__":
    print "Executing main flow"
    #serial_commander = SerialCommander()
    serial_commander = SerialCommanderStub()
    androidT = androidThread("android", serial_commander, mode="auto", production=False)
    # serialExecutionThread = SerialExecutionThread("serial", serial_commander, production=True)

    # serialExecutionThread.start()
    androidT.start()




