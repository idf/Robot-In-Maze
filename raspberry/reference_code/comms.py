'''
this is at PI
'''
from socket import *
import threading
import time

PI_port = 8300
#PC_IP = ''
#PC_IP = '192.168.14.9'
PC_IP = '192.168.14.7'
PC_port = 8400

stillWaitingForMsg = False
retrievedData = ""

GREEN = '\033[92m'
YELLOW = '\033[93m'
RED = '\033[91m'
WHITE = '\033[0m'

def listenerHandler():
	global stillWaitingForMsg,retrievedData
	s = socket(AF_INET, SOCK_STREAM)
	s.bind(('', PI_port))
	s.listen(1)
	try:
		while True:
			conn, addr = s.accept()
			data = conn.recv(1024)

			retrievedData = str(data)
			stillWaitingForMsg = False

			conn.close
			time.sleep(0.1)
	except Exception as err:
		print RED+'Error listenerHandler:', err
		s.close()

def startListeningFromPC():
	threading.Thread(target=listenerHandler).start()
	time.sleep(0.5)
	sendMsg("PIready")
	if (getMsg()=="PCready"):
	    return True

def sendMsg(message):
	global lastMsgToSend, stillWaitingForMsg
	try:
		s = socket()
		s.connect((PC_IP, PC_port))
		s.send(message)
		s.close()
		stillWaitingForMsg=True
	except Exception as err:
		print RED+'Error sendMsg: ', err	

def getMsg():
	while (stillWaitingForMsg):
		time.sleep(0.1)
	return retrievedData
