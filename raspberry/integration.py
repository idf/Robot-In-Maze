from bluetooth_communication import *
from pc_communication import PcThread, PcExploreRunThread
from serial_comminication import *

__author__ = 'Danyang'

if __name__=="__main__":

    # shared resources
    serial_api = SerialAPI()
    android_api = AndroidAPI(serial_api)

    # threads
    android_thread = AndroidThread("android", android_api, mode="auto", production=True)
    bluetooth_explore_run_thread = AndroidExploreRunThread("explore_run_bluetooth", android_thread.android_api)
    pc_thread = PcThread("pc_thread", serial_api, android_api)
    pc_explore_run_thread = PcExploreRunThread("explore_run_pc", pc_thread.pc_api)
    serial_exe_thread = SerialExecutionThread("serial", serial_api, production=True)

    serial_exe_thread.start()
    android_thread.start()
    bluetooth_explore_run_thread.start()
    pc_thread.start()
    pc_explore_run_thread.start()