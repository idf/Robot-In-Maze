from bluetooth_communication import AndroidThread, AndroidAPI
from serial_comminication import SerialAPI, SerialExecutionThread
from serial_stub import SerialAPIStub

__author__ = 'Yanl56'


# test bluetooth and serial
if __name__=="__main__":
    print "Executing main flow"
    serial_api = SerialAPI()
    android_api = AndroidAPI(serial_api)
    androidThread = AndroidThread("android", android_api, mode="control", production=False)
    # serialExecutionThread = SerialExecutionThread("serial", serial_api, production=True)

    # serialExecutionThread.start()
    androidThread.start()




