from Queue import Queue
import threading
import serial
import json
import time
import sys
from abstract import AbstractThread, print_msg
from utils.decorators import *
from settings import *

__author__ = 'Danyang'
FUNCTION = 0
SENSOR = 1

class SerialAPI(object):
    def __init__(self, port=None, data_rate=9600,production=True):
        self.name = "SerialAPI"
        self.ready=False
        self.ser = None

        if production:
            self._init_serial(port, data_rate)

        self.commands_outgoing = Queue()  # synchronized, queue of [function_code, parameter]
        self.responses_outgoing = Queue()  # queue of [ack, type_data, data]

        # stop n wait
        self.outstanding_command_pair = None




    def disconnect(self, *args):
        """
        Waiting for the Progress Terminate signal to close the port
        Not used yet
        :param args:
        """
        print_msg(self.name, "Closing serial")
        if self.ser.isOpen():
            self.ser.close()



    def _init_serial(self, port, data_rate):
        """
        if the port is specified as None, it automatically tries to find a serial port
        if the port is specified (usually through command line argument), it open the serial at that port
        :param port: path to the serial port
        :param data_rate: int
        :return: void
        """
        # Serial port: /dev/ttyACM0
        # The Raspberry Pi may not provide enough power to drive an Arduino, so you might need external power.
        self.ser = serial.Serial()
        self.ser.baudrate = data_rate
        self.ser.timeout = 1

        if port!=None:
            self.ser.port = port
        else:
            port = "/dev/ttyACM0"
            print_msg(self.name, "Automatically find serial port")
            for i in range(20):
                try:
                    port_auto_find = port[:-1] + str(i)
                    self.ser.port = port_auto_find
                    print_msg(self.name, "Finding port at %s"%port_auto_find)
                    self.ser.open()

                    return
                except serial.SerialException as e:
                    print e.message
                    continue

            print_msg(self.name, "Automatically find port fails. Try to reboot the OS")
            sys.exit(-1)



    def _convert_to_machine_code(self, function_code, parameter):
        """
        Convert function code and parameter to the Arduino readable code
        :param function_code: int
        :param parameter: double
        :return: machine code string
        """
        function_code_str = '0000000' + str(function_code)
        function_code_str = function_code_str[-2:]
        parameter_str = '0000000' + str(int(parameter * 100))
        parameter_str = parameter_str[-5:]
        return function_code_str + parameter_str

    def _parse_function_status(self, receive_data_dict):
        """
        parse {"function": 0, "status": 200}
        :param receive_data_dict:
        :return:
        """
        function_code = receive_data_dict["function"]
        status_code = receive_data_dict["status"]

        return {int(function_code): status_code}

    def _parse_sensor_readings(self, receive_data_dict):
        """
        parse {"sensors":[{"sensor":0,"value":100},{"sensor":1,"value":30},{"sensor":2,"value":30},{"sensor":10,"value":30},{"sensor":11,"value":30}]}
        :param receive_data_dict:
        :return:
        """
        sensor_lst = receive_data_dict["sensors"]
        reading_dic =  {}
        for element in sensor_lst:
            reading_dic[element["sensor"]] = element["value"]

        return reading_dic

    ########################################################################################################################
    def write(self, function_code, parameter):
        """
        write_to_pc to the serial
        :param function_code: int
        :param parameter: double
        """
        # if self.ready==True:
        self.ser.write(self._convert_to_machine_code(function_code, parameter))

    def read(self):
        """
        Read from the serial
        Guaranteed to get json response
        :return: parsed json
        """
        receive_data = ""
        while True:  # keep fetching until found json
            data = self.ser.readline() # waits for the arduino to send a serial and will not continue unless it fetches a serial

            if "{" in data: # only check for starting "{" # naive type checking
                receive_data = data[data.find("{"): ]
                break
            else:
                # if no json, wait
                continue

        print_msg(self.name, "received serial data: %s"%receive_data)
        try:
            receive_data_dict = json.loads(receive_data)
            if "sensors" in receive_data_dict:
                return SENSOR, receive_data  # return raw data
            elif "function" in receive_data_dict:
                return FUNCTION, receive_data  # return raw data
            else:
                return None, None
        except ValueError:
            print_msg(self.name, "json mal-formatted")
            return None, None


    @Deprecated
    def is_ready(self):
        """
        Ready when receiving {"function": 99, "status": 200}
        :return:
        """
        indicator, dic = self.read()
        if indicator!=FUNCTION:
            return

        if dic.get(99, None)==200:
            print_msg(self.name, "robot ready")
            self.ready = True

    ########################################################################################################################
    def command_pop_n_exe(self):
        if not self.commands_outgoing.empty():
            self.ack = False
            command_pair = self.commands_outgoing.get()
            self.write(command_pair[0], command_pair[1])
            self.outstanding_command_pair = command_pair
            print_msg(self.name, "Executing command"+str(command_pair))

    def command_put(self, function, parameter):
        self.commands_outgoing.put([function, parameter])

    def is_command_empty(self):
        return self.commands_outgoing.empty()

    ########################################################################################################################
    def response(self):
        """
        Parse the response from the serial
        :return: ack:bool, type_data: int, data:dict
        """
        print_msg(self.name, "waiting for ack")
        type_data, data = self.read()


        # sensor data
        if type_data==SENSOR:
            return False, type_data, data
        data_dict = json.loads(data)
        data_parsed = self._parse_function_status(data_dict)
        if data_parsed.get(self.outstanding_command_pair[0], None)==200:  # use the function_code to get the status
            self.outstanding_command_pair = None
            return True, type_data, data

        return None, None, None


    def response_put(self, ack, type_data, data):
        """
        :param ack: bool
        :param type_data: int
        :param data: String
        """
        self.responses_outgoing.put([ack, type_data, data])

    def response_pop(self):
        """
        blocking, pop out one of the item from responses_outgoing queue
        if empty, wait until the Queue is written by other threads
        :return: [ack, type_data, data]
        """
        if self.responses_outgoing.empty():
            return None
        return self.responses_outgoing.get()



# serial_api is the shared resources
class SerialExecutionThread(AbstractThread):
    @Override(AbstractThread)
    def __init__(self, name, serial_api, production):
        super(SerialExecutionThread, self).__init__(name, production)

        self.serial_api = serial_api
        # daemon thread
        self.setDaemon(True)

    @Override(AbstractThread)
    def run(self):
        self.print_msg("Starting")
        while True:
            if self.serial_api.is_command_empty():
                # self.print_msg("Waiting for enqueuing command")
                if self.production:
                    # waiting for request_command to be called
                    time.sleep(0.05)
                    continue
                else:
                    function_code = int(raw_input("function code: "))
                    parameter = float(raw_input("parameter: "))
                    self.serial_api.command_put(function_code, parameter)
            else:
                self.serial_api.command_pop_n_exe()
                # stop and wait for ack
                while True:
                    ack, type_data, data = self.serial_api.response()
                    if ack!=None:
                        self.serial_api.response_put(ack, type_data, data)

                    print data
                    if ack:
                        break

        self.print_msg("Exiting")
