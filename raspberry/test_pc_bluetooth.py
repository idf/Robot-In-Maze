from bluetooth_communication import AndroidAPI, AndroidThread, AndroidExploreRunThread
from pc_communication import PcThread, PcExploreRunThread
from serial_stub import SerialAPIStub

__author__ = 'Danyang'

if __name__=="__main__":
    print "Executing main flow"
    serial_api = SerialAPIStub()
    android_api = AndroidAPI(serial_api)

    android_thread = AndroidThread("android", android_api, mode="auto", production=True)
    explore_run_thread = AndroidExploreRunThread("explore_run_bluetooth", android_thread.android_api)
    pc_thread = PcThread("pc_thread", serial_api, android_api)
    pc_explore_run_thread = PcExploreRunThread("explore_run_pc", pc_thread.pc_api)

    android_thread.start()
    explore_run_thread.start()
    pc_thread.start()
    pc_explore_run_thread.start()