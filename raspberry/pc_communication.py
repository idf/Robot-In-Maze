import socket
from abstract import print_msg
from serial_stub import *
import json
import time

class PcAPI (object):
    def __init__(self, serial_api, android_api):
        self.tcp_ip = "192.168.15.15" # Connecting to IP address of MDPGrp 15
        self.port = 50015
        self.conn = None
        self.pc_addr = None
        self.is_connect = False
        self.name = "Pc Interfacing"

        # shared resources
        self.serial_api = serial_api
        self.android_api = android_api

        # explore start signal
        self.__explore_sent = False
        self.__run_sent = False
        
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

    def _is_connected(self):
        return self.is_connect

    def __response_to_pc(self, msg):  # taking function code from Robot
        """
        Write response to PC
        :param msg: String
        """
        msg += "\0"  # talking to C thus adding \0
        if self._is_connected():
            #try:
            print_msg(self.name, "Writing to PC: %s" % msg)  # passing function code to PC
            self.conn.sendto(msg, self.pc_addr)
            time.sleep(0.0001)  # adjusts thread scheduling and allows the socket I/O to finish FLUSH

    def __index_json_msg(self, msg):
        import re
        return [m.start() for m in re.finditer(r"{", msg)]


    def __read(self):
        """
        Read from the socket (json needed)
        :return: dictionary
        """
        msg = self.conn.recvfrom(1024)
        msg = msg[0]
        print_msg(self.name, "Message received: %s" % msg)
        
        msg = msg[msg.find("{"):] # cleaning data
        msg = msg.replace("}\n{", ",") # in case of combined socket  

        msg_dict = json.loads(msg)
        return msg_dict

    def communicate_with_pc(self):
        """
        Main Flow
        """
        if self._is_connected():
            msg_dict = self.__read()
            if "map" in msg_dict:
                if self.android_api!=None:
                    print_msg(self.name, "Updating Android's map")
                    self.android_api.map_put(msg_dict["map"], msg_dict["location"])

            if "function" in msg_dict:
                # executing function with parameter
                function_code = int(msg_dict["function"])
                parameter = int(msg_dict["parameter"])
                print_msg(self.name, "Executing robot command")
                self.serial_api.command_put(function_code, parameter)  # passing information to Robot

                if (function_code/10!=0): # ack for commands except for 0 1 2 command
                    # waiting for ack
                    while True:
                        lst = self.serial_api.response_pop() # send acknowledgement to PC
                        if lst==None:
                            time.sleep(0.05) # 50 ms
                            continue
                        else:
                            print_msg(self.name, "Received acknowledgement")
                            ack, type_data, data = lst[0], lst[1], lst[2]
                            print_msg(self.name, "Acknowledgement: "+str(ack)+str(type_data)+str(data))

                            try:
                                if json.loads(data)["function"]/10==0: # avoid 0 1 2
                                    continue
                            except KeyError as e:
                                pass 

                            sending_msg = data
                            self.__response_to_pc(sending_msg)
                            if ack:
                                break
                else:
                    sending_msg = json.dumps({"function": function_code, "status": 200}) # manual 0 1 2
                    self.__response_to_pc(sending_msg)


        else:
            print_msg(self.name, "Not connected")

    def explore_run_signal(self):
        if not self.__explore_sent:
            if self.android_api.explore_start:
                print_msg(self.name, "Sending \"explore\" signal")
                self.__response_to_pc("explore")
                self.__explore_sent = True

        if not self.__run_sent:
            if self.android_api.run_start:
                print_msg(self.name, "Sending \"start\" signal")
                self.__response_to_pc("start")
                self.__run_sent = True


class PcThread(AbstractThread):
    @Override(AbstractThread)
    def __init__(self, name, serial_api, android_api):
        super(PcThread, self).__init__(name, production=True)
        self.pc_api = PcAPI(serial_api, android_api)

    @Override(AbstractThread)
    def run(self):
        self.print_msg("Connecting")
        self.pc_api.connect()
        
        while True:
            self.print_msg("Reading...")
            self.pc_api.communicate_with_pc()
            time.sleep(0.05)


class PcExploreRunThread(AbstractThread):
    @Override(AbstractThread)
    def __init__(self, name, pc_api):
        super(PcExploreRunThread, self).__init__(name, True)
        self.pc_api = pc_api

    @Override(AbstractThread)
    def run(self):
        while not self.pc_api._is_connected():
            time.sleep(0.05)

        while True:
            self.pc_api.explore_run_signal()
            time.sleep(0.05)

if __name__=="__main__":
    # stub testing
    serial_api = SerialAPIStub()
    android_api = None
    pc_thread = PcThread("pc_thread", serial_api, android_api)
    pc_thread.start()


