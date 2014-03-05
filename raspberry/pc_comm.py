import socket
from serial_stub import *
import exceptions
import json


class PcInterfacing (object):
    def __init__(self, serial_commander, android_commander):
        self.tcp_ip = "192.168.15.15" # Connecting to IP address of MDPGrp 15
        self.port = 50015
        self.conn = None
        self.pcaddr = None
        self.is_connect = False

        # shared resources
        self.serial_commander = serial_commander
        self.android_commander = andorid_commander

        
    def connect(self):
        """
        TCP connection
        """
        self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Create a TCP/IP socket
        #make port reusable
        self.conn.setsockopt (socket.SOL_SOCKET, socket.SO_REUSEADDR,1) # To reuse local socket in Time-Wait state
        self.conn.setsockopt (socket.SOL_SOCKET, socket.SO_BROADCAST,1) # Sending broadcast message
        self.conn.bind((self.tcp_ip, self.port))
        self.conn.listen(1) # listening for incoming connection to the IP/Port you got with bind
        client, addr = self.conn.accept()
        print "Connected! Connection address: ", addr
        self.conn = client
        self.pcaddr = addr
        self.is_connect = True

    def disconnect(self):
        """
        TCP disconnection
        """
        self.conn.close()
        self.is_connect = False

    def __is_connected(self):
        return self.is_connect

    def write_to_pc(self, msg): # taking function code from Robot
        """
        Write response to PC
        :param msg: String
        """
        if(self.__is_connected()):
            #try:
            print "Writing to PC: %s" % msg # passing function code to PC
            self.conn.sendto(msg, self.pcaddr)
            time.sleep(0.0001) # adjusts thread scheduling and allows the socket I/O to finish

            # except Exception as e:
             #   print e.message
              #  print "Unable to write_to_pc"
              #  self.is_connect = False

    def read_from_pc(self):
        """
        Main Flow
        """
        if(self.__is_connected()):
            msg = self.conn.recvfrom(1024)
            msg = msg[0]
            print "Message received: ", msg
            msg = msg[msg.find("{"):] # cleaning data

            msg_dict = json.loads(msg)
            function_code = msg_dict["function"]
            parameter = int(msg_dict["parameter"])

            self.serial_commander.command_put(function_code, parameter) # passing information to Robot
            # TODO write to android
            while True:
                lst = self.serial_commander.response_pop() # send acknowledgement to PC
                if lst==None:
                    continue
                else:
                    print "Received acknowledgement"
                    ack, type_data, data = lst[0], lst[1], lst[2]
                    print "Acknowledgement: "
                    print ack, type_data, data
                    sending_msg = data  # TODO
                    self.write_to_pc(sending_msg)

            #except Exception as e:
             #   print e.message
              #  print "Unable to read_from_pc"
              #  self.is_connect = False
        else:
            print "not connected"

class PcThread(AbstractThread):
    @Override(AbstractThread)
    def __init__(self, name, serial_commander, android_commander, production=False):
        super(PcThread, self).__init__(name, production)
        self.pc_interfacing = PcInterfacing(serial_commander, android_commander)

    @Override(AbstractThread)
    def run(self):
        print "connecting"
        self.pc_interfacing.connect()
        

        while True:
            print "reading"
            self.pc_interfacing.read_from_pc()
            time.sleep(0.5)

if __name__ == "__main__":
    print "Executing main flow"
    serial_commander = SerialCommanderStub()
    andorid_commander = None
    pc_thread = PcThread("pc_thread", serial_commander, andorid_commander)
    pc_thread.run()
    print "Executing main flow"