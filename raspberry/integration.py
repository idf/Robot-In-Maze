from bluetooth_communication import *
from pc_communication import PcThread
from serial_comminication import *

__author__ = 'Danyang'

if __name__=="__main__":

    # shared resources
    serial_commander = SerialCommander()
    android_commander = AndroidCommander(serial_commander)

    # threads
    pc_thread = PcThread("pc_thread", serial_commander, android_commander, production=True)
    serial_thread = SerialExecutionThread("serial_thread", serial_commander, production=True)
    android_thread = AndroidThread("android_thread", android_commander, mode="auto", production=True)

    serial_thread.start()
    pc_thread.start()
    android_thread.start()
