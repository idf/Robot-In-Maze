from pc_comm import PcThread
from serial_comminication import *

__author__ = 'Danyang'


if __name__=="__main__":
    print "Executing main flow"
    serial_commander = SerialCommander()
    andorid_commander = None
    pc_thread = PcThread("pc_thread", serial_commander, andorid_commander)
    serial_thread = SerialExecutionThread("serial", serial_commander, production = True)


    serial_thread.start()
    pc_thread.run()
    print "Executing main flow"
