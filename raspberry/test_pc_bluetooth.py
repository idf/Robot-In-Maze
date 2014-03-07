from bluetooth_communication import AndroidCommander, AndroidThread, AndroidExploreRunThread
from pc_communication import PcThread, PcExploreRunThread
from serial_stub import SerialCommanderStub

__author__ = 'Danyang'

if __name__=="__main__":
    print "Executing main flow"
    serial_commander = SerialCommanderStub()
    android_commander = AndroidCommander(serial_commander)

    android_thread = AndroidThread("android", android_commander, mode="auto", production=True)
    explore_run_thread = AndroidExploreRunThread("explore_run_bluetooth", android_thread.android_commander)
    pc_thread = PcThread("pc_thread", serial_commander, android_commander)
    pc_explore_run_thread = PcExploreRunThread("explore_run_pc", pc_thread.pc_interfacing)

    android_thread.start()
    explore_run_thread.start()
    pc_thread.start()
    pc_explore_run_thread.start()