from bluetooth_communication import *
from pc_communication import PcThread, PcExploreRunThread
from serial_comminication import *

__author__ = 'Danyang'

if __name__=="__main__":

    # shared resources
    serial_commander = SerialCommander()
    android_commander = AndroidCommander(serial_commander)

    # threads
    android_thread = AndroidThread("android", android_commander, mode="auto", production=True)
    explore_run_thread = AndroidExploreRunThread("explore_run_bluetooth", android_thread.android_commander)
    pc_thread = PcThread("pc_thread", serial_commander, android_commander)
    pc_explore_run_thread = PcExploreRunThread("explore_run_pc", pc_thread.pc_interfacing)
    serial_exe_thread = SerialExecutionThread("serial", serial_commander, production=True)

    serial_exe_thread.start()
    android_thread.start()
    explore_run_thread.start()
    pc_thread.start()
    pc_explore_run_thread.start()