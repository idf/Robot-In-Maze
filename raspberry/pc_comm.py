import socket
from serial_stub import *
import exceptions

class pc_interface (object):
    def __init__(self):
        self.tcp_ip = "192.168.15.15" # Connecting to IP address of MDPGrp 15
        self.port = 50015
        self.ip_sock = None
        self.pcaddr = None
        self.is_connect = False

        self.commander = SerialCommanderStub()
        
    def connect(self):
        self.ip_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Create a TCP/IP socket
        #make port reusable
        self.ip_sock.setsockopt (socket.SOL_SOCKET, socket.SO_REUSEADDR,1) # To reuse local socket in Time-Wait state
        self.ip_sock.setsockopt (socket.SOL_SOCKET, socket.SO_BROADCAST,1) # Sending broadcast message
        self.ip_sock.bind((self.tcp_ip, self.port))
        self.ip_sock.listen(1) # listening for incoming connection to the IP/Port you got with bind
        conn, addr = self.ip_sock.accept()
        print "Connected! Connection address: ", addr
        self.pcaddr = addr
        self.is_connect = True

    def disconnect(self):
        self.ip_sock.close()
        self.is_connect = False

    def is_connected(self):
        return self.is_connect;

    def write(self,msg): # taking function code from Robot
        if(self.is_connected()):
            try:
                self.ip_sock.sendto (msg, self.pcaddr)
                print "Write to PC: %d" % msg # passing function code to PC

            except Exception:
                print "Unable to write"
                self.is_connect = False

    def read(self, function, parameter):
        if(self.is_connected()):
            try:
                function, parameter, address = self.ip_sock.recvfrom (1024)
                print "Read from PC: %d %d" %(function, parameter)
                self.commander.command_put(function, parameter) # passing information to Robot
                while True:
                    lst = self.commander.response_pop() # send acknowledgement to PC
                    if lst==None:
                        continue
                    else:
                        ack, type_data, data = lst[0], lst[1], lst[2]
                        print ack, type_data, data

            except Exception:
                print "Unable to read"
                self.is_connect = False

if __name__ == "__main__":
    print "Executing main flow"
    pc_comm = pc_interface()
    pc_comm.__init__()
    pc_comm.connect()
