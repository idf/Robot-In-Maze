import socket
from abstract import print_msg
from serial_stub import *
import json
import time

class PcInterfacing (object):
    def __init__(self, serial_commander, android_commander):
        self.tcp_ip = "192.168.15.15" # Connecting to IP address of MDPGrp 15
        self.port = 50015
        self.conn = None
        self.pc_addr = None
        self.is_connect = False
        self.name = "Pc Interfacing"

        # shared resources
        self.serial_commander = serial_commander
        self.android_commander = android_commander

        
    def connect(self):
        """
        TCP connection
        """
        self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Create a TCP/IP socket
        #make port reusable
        self.conn.setsockopt (socket.SOL_SOCKET, socket.SO_REUSEADDR,1) # To reuse local socket in Time-Wait state
        self.conn.setsockopt (socket.SOL_SOCKET, socket.SO_BROADCAST,1) # Sending broadcast message
        self.conn.bind((self.tcp_ip, self.port))
        self.conn.listen(1)  # listening for incoming connection to the IP/Port you got with bind
        client, addr = self.conn.accept()
        print_msg(self.name, "Connected! Connection address: "+str(addr))
        self.conn = client
        self.pc_addr = addr
        self.is_connect = True

    def disconnect(self):
        """
        TCP disconnection
        """
        self.conn.close()
        self.is_connect = False

    def __is_connected(self):
        return self.is_connect

    def __response_to_pc(self, msg):  # taking function code from Robot
        """
        Write response to PC
        :param msg: String
        """
        msg += "\0"  # talking to C thus adding \0
        if self.__is_connected():
            #try:
            print_msg(self.name, "Writing to PC: %s" % msg)  # passing function code to PC
            self.conn.sendto(msg, self.pc_addr)
            time.sleep(0.0001)  # adjusts thread scheduling and allows the socket I/O to finish FLUSH

    def read_from_pc(self):
        """
        Main Flow
        """
        if self.__is_connected():
            msg = self.conn.recvfrom(1024)
            msg = msg[0]
            print_msg(self.name, "Message received: %s"%msg)
            msg = msg[msg.find("{"):] # cleaning data

            msg_dict = json.loads(msg)
            function_code = int(msg_dict["function"])
            parameter = int(msg_dict["parameter"])

            self.serial_commander.command_put(function_code, parameter)  # passing information to Robot
            while True:
                lst = self.serial_commander.response_pop() # send acknowledgement to PC
                if lst==None:
                    time.sleep(0.05) # 50 ms
                    continue
                else:
                    print_msg(self.name, "Received acknowledgement")
                    ack, type_data, data = lst[0], lst[1], lst[2]
                    print_msg(self.name, "Acknowledgement: "+str(ack)+str(type_data)+str(data))

                    sending_msg = data
                    self.__response_to_pc(sending_msg)

                    if self.android_commander!=None:
                        # TODO write map to android
                        pass

                    if ack:
                        break
        else:
            print_msg(self.name, "Not connected")

class PcThread(AbstractThread):
    @Override(AbstractThread)
    def __init__(self, name, serial_commander, android_commander, production):
        super(PcThread, self).__init__(name, production)
        self.pc_interfacing = PcInterfacing(serial_commander, android_commander)

    @Override(AbstractThread)
    def run(self):
        self.print_msg("connecting")
        self.pc_interfacing.connect()
        
        while True:
            self.print_msg("Reading...")
            self.pc_interfacing.read_from_pc()
            time.sleep(0.05)


if __name__=="__main__":
    # stub testing
    print "Starting main flow"
    serial_commander = SerialCommanderStub()
    android_commander = None
    pc_thread = PcThread("pc_thread", serial_commander, android_commander, production=False)
    pc_thread.start()
    print "Existing main flow"