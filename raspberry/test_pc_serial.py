from pc_communication import PcThread
from serial_comminication import *

__author__ = 'Danyang'


# PC and Serial
if __name__=="__main__":
    print "Starting main flow"
    # shared resources
    serial_api = SerialAPI()
    android_api = None

    # threads
    pc_thread = PcThread("pc_thread", serial_api, android_api, production=True)
    serial_thread = SerialExecutionThread("serial", serial_api, production=True)


    serial_thread.start()
    pc_thread.start()
    print "Existing main flow"
