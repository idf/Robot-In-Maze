from pc_comm import PcThread
from serial_comminication import *

__author__ = 'Danyang'


if __name__=="__main__":
    print "Starting main flow"
    # shared resources
    serial_commander = SerialCommander()
    android_commander = None

    # threads
    pc_thread = PcThread("pc_thread", serial_commander, android_commander, production=True)
    serial_thread = SerialExecutionThread("serial", serial_commander, production=True)


    serial_thread.start()
    pc_thread.start()
    print "Existing main flow"
