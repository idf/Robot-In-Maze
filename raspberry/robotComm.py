import serial
import threading
import json
from utils.decorators import Deprecated

#-----------------for arduino connection
ser = serial.Serial("/dev/ttyACM0", 9600)


#receive_data = '{"function": 0, "status": 200}' # sample : to be read from serial
global robot_status
robot_status = False
#-------------------Arduino
data = ser.readline()

@Deprecated
def robot_read():
    global data, robot_status
    while True:
        data = ser.readline()
        print data

    print data[data.find("{")+1 : data.find("}")]
    receive_data = "{"+data[data.find("{")+1 : data.find("}")]+"}"
    receive_data_dict = json.loads(receive_data)
    print receive_data_dict
    function_code = receive_data_dict["function"]
    status_code = receive_data_dict["status"]
    print function_code
    print status_code
    if status_code == "200":
        print "robot ready"
        robot_status = True


@Deprecated
def robot_write():
    if robot_status == True:
        ser.write(convert_to_machine_code(1, 90.00))


@Deprecated
def convert_to_machine_code(function_code, parameter):
    """
    function_code is int
    parameter is double
    """
    function_code_string = '000000'+str(function_code)
    function_code_string = function_code_string[-2:]
    parameter_string = '000000'+str(int(parameter*100))
    parameter_string = parameter_string[-5:]
    return function_code_string+parameter_string





#converting = threading.Thread(target = convert_to_machine_code(1, 90.00))
#converting.start()


arduino_Read = threading.Thread(target = robot_read())
arduino_Read.start()

arduino_Send  = threading.Thread(target = robot_write)
arduino_Send.start()

#converting.join()
arduino_Read.join()
arduino_Send.join()
