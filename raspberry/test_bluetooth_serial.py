from bluetooth_communication import AndroidThread
from serial_comminication import SerialAPI, SerialExecutionThread
from serial_stub import SerialAPIStub

__author__ = 'Yanl56'


# test bluetooth and serial
if __name__=="__main__":
    print "Executing main flow"
    serial_api = SerialAPI()
    androidThread = AndroidThread("android", serial_api, mode="control", production=False)
    # serialExecutionThread = SerialExecutionThread("serial", serial_api, production=True)

    # serialExecutionThread.start()
    androidThread.start()




