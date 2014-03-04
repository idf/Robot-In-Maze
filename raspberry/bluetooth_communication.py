import threading
from bluetooth import *
from serial_stub import *
from serial_comminication import *

def goodbye(client_sock, server_sock):
    if client_sock:
        print "Closing bluetooth client"
        client_sock.close()
    if server_sock:
        print "Closing bluetooth server"
        server_sock.close()

class androidCommander(object):
    def __init__(self, serial_commander):
        self.client_sock = None
        self.server_sock = None
        self.is_connected = False
        #self.serial_commander = SerialCommanderStub()
        self.serial_commander = serial_commander

    def is_connect(self):
        return self.is_connected

    def init_bluetooth(self, btport = 4):
        """
        manual connect to nexus
        channel port: 4
        nexus address "08:60:6E:A5:90:50" #mac number
        """
        self.server_sock = BluetoothSocket( RFCOMM )
        self.server_sock.bind(("", btport))
        self.server_sock.listen(1)
        port = self.server_sock.getsockname()[1]
        uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"
        advertise_service(self.server_sock, "SampleServer",
                          service_id=uuid,
                          service_classes=[uuid, SERIAL_PORT_CLASS],
                          profiles=[SERIAL_PORT_PROFILE],
        )
        import atexit
        atexit.register(goodbye, None, self.server_sock)
        print "atexit registered 1"
        print "waiting for connection on RFCOMM channel %d" % port
        self.client_sock, client_info = self.server_sock.accept() # blocking
        atexit.register(goodbye, self.client_sock, self.server_sock)
        print "atexit registered 2"
        # print "Accepted connection from ", client_info
        self.is_connected = True


    def disconnect(self):
        print "disconnect"
        self.server_sock.close()

    def connect(self):
        """
        auto connect to nexus
        """
        print "Connecting..."
        self.client_sock = BluetoothSocket( RFCOMM )
        uuid = "00001101-0000-1000-8000-00805F9B34FB"
        btaddr = "08:60:6E:A5:90:50"
        service_match = find_service(uuid = uuid, address = btaddr)
        while len(service_match) ==  0:
                service_match = find_service(uuid = uuid, address = btaddr)

        first_match = service_match[0]
        port = first_match["port"]
        host = first_match["host"]
        self.client_sock.connect((host,port))
        print "Connected to ",host
        self.is_connected = True

    def write(self,msg):
        """
        write to nexus
        :status + msg
        """
        try:
            while self.is_connect():
            #while self.is_connect():
                self.client_sock.send("status "+str(msg))
                print "Write to Android: %s" % msg
                return True

        except IOError:
            print "disconnected"
            self.is_connected = False
            self.client_sock.close()
            self.disconnect()




    def read(self):
        """
        listen from nexus
        """
        try:
            if self.is_connect():
                print "Receiving socket package"
                b_data = self.client_sock.recv(1024)
                print "Received from Android: %s" % b_data
                if len(b_data) != 0:
                    print "decoding"
                    message =  self.decode_n_execute(b_data)
                    self.write(message)
        except IOError:
            print "disconnected"
            self.is_connected = False
            self.client_sock.close()
            self.disconnect()

    def __execute_msg(self, function_code, parameter):
        #self.write("Forward")
        self.serial_commander.command_put(function_code, parameter)
        while True:
            lst = self.serial_commander.response_pop()
            print "Waiting for response"
            print lst
            if lst==None:
                time.sleep(2)
                continue
            else:
                ack, type_data, data = lst[0], lst[1], lst[2]
                print ack, type_data, data
                if ack==True:
                    break
                else:
                    continue


    def decode_n_execute(self, msg):
        if msg == "w":
            self.__execute_msg(0, 10)
            return "Forward"
        elif msg == "a":
            self.__execute_msg(1, 90)
            return "Turn Left"
        elif msg == "d":
            self.__execute_msg(2, 90)
            return "Turn Right"
        elif msg == "run":#shortest path
            #self.write("run")
            return ""
        elif msg == "explore":#explore maze
            #self.write("explore")
            return ""




class androidThread(AbstractThread):
    @Override(AbstractThread)
    def __init__(self, name, serial_commander):
        super(androidThread, self).__init__()
        self.commander = androidCommander(serial_commander)
        self.name = name


    @Override(AbstractThread)
    def run(self):
        self.print_msg("Starting")
        while True:
            while True:
                self.commander.init_bluetooth()
                if self.commander.is_connect:
                    break
                time.sleep(1)
            self.commander.read()
            #self.commander.write()
        self.print_msg("Ending")







