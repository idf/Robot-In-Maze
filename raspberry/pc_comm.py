import socket
from serial_stub import *
import exceptions
import json
FUNCTION_CODE_MAPPING= {
    "forward": 0,
    "left": 1,
    "right": 2,
    "sensor": 10
}

class PcInterfacing (object):
    def __init__(self, serial_commander, android_commander):
        self.tcp_ip = "192.168.15.15" # Connecting to IP address of MDPGrp 15
        self.port = 50015
        self.ip_sock = None
        self.pcaddr = None
        self.is_connect = False

        self.serial_commander = serial_commander

        
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
        return self.is_connect

    def write_to_pc(self, msg): # taking function code from Robot
        if(self.is_connected()):
            try:
                print "Writing to PC: %d" % msg # passing function code to PC
                self.ip_sock.sendto (msg, self.pcaddr)

            except Exception as e:
                print e.message
                print "Unable to write_to_pc"
                self.is_connect = False

    def read_from_pc(self):
        """
        Main Flow
        """
        if(self.is_connected()):
            try:
                msg = self.ip_sock.recvfrom (1024)
                msg_dict = json.loads(msg)
                function_code = FUNCTION_CODE_MAPPING[msg_dict["function"]]
                parameter = int(msg_dict["paramter"])
                print "Read from PC: %d %d" %(function_code, parameter)
                self.serial_commander.command_put(function_code, parameter) # passing information to Robot
                # TODO write to android
                while True:
                    lst = self.serial_commander.response_pop() # send acknowledgement to PC
                    if lst==None:
                        continue
                    else:
                        print "Received acknowlegement"
                        ack, type_data, data = lst[0], lst[1], lst[2]
                        print "Acknowledgement: "
                        print ack, type_data, data

                        self.write_to_pc([ack, type_data, data])

            except Exception as e:
                print e.message
                print "Unable to read_from_pc"
                self.is_connect = False

class PcThread(AbstractThread):
    @Override(AbstractThread)
    def __init__(self, name, serial_commander, android_commander, production=False):
        super(PcThread, self).__init__(name, production)
        self.pc_interfacing = PcInterfacing(serial_commander, android_commander)

    @Override(AbstractThread)
    def run(self):
        while not self.pc_interfacing.is_connected():
            print "connecting"
            self.pc_interfacing.connect()
            time.sleep(0.5)

        while True:
            self.pc_interfacing.read_from_pc()
            time.sleep(0.5)

if __name__ == "__main__":
    print "Executing main flow"
    serial_commander = SerialCommanderStub()
    andorid_commander = None
    pc_thread = PcThread("pc_thread", serial_commander, andorid_commander)
    pc_thread.run()



