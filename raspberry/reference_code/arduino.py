import serial
import time

countFrontObstacleTrue=0
countFrontObstacleFalse=0
countLeftObstacleTrue=0
countLeftObstacleFalse=0
countRightObstacleTrue=0
countRightObstacleFalse=0
countBottomObstacleTrue=0
countBottomObstacleFalse=0

countFrontDist80=0
countFrontDist70=0
countFrontDist60=0
countFrontDist50=0
countFrontDist40=0
countFrontDist30=0
countFrontDist20=0

robotMotorMoving=False

booleanGotMsgForPi=False

messageForPi=""
distanceTravelled=0
collectingObsInfo=False

waitingToMove=""
waitingToMoveVar=0

GREEN = '\033[92m'
YELLOW = '\033[93m'
RED = '\033[91m'
WHITE = '\033[0m'

########## 'Switch' Class to Enable Switch Case ###########
class switch(object):
    def __init__(self,value):
        self.value = value
        self.fall = False

    def __iter__(self):
        """Return the match method once, then stop"""
        yield self.match
        raise StopIteration

    def match(self, *args):
        """Indicate whether or not to enter a case suite"""
        if self.fall or not args:
            return True
        elif self.value in args:
            self.fall = True
            return True
        else:
            return False
########## 'Switch Class' to Enable Switch Case ###########

## Connect to selected port with 9600 baud rate
try:
    serConnect = serial.Serial('/dev/ttyACM0',9600)
except:
    serConnect = serial.Serial('/dev/ttyACM1',9600)

## Time needed for arduino to start up; requires ~1.5s
time.sleep(1.6)
serConnect.flushInput()

## Functions of actions for robot execution 
'''
def getDistanceMoved():#this doesnt work
    print "request distanceTravelled"
    serConnect.write('9/')
'''
def moveForward(distCm):
    global robotMotorMoving, waitingToMove, waitingToMoveVar
    if(not robotMotorMoving and distCm!=0):
        robotMotorMoving=True
        moveForwardCmd = '1/%d/' % distCm
        serConnect.write(moveForwardCmd)
        waitingToMove=1
        waitingToMoveVar=distCm
        print YELLOW+"MOVING FORWARD "+str(distCm)

def turnLeft(angle):
    global robotMotorMoving, waitingToMove, waitingToMoveVar
    if(not robotMotorMoving and angle!=0):
        robotMotorMoving=True
        turnLeftCmd = '2/%d/' % angle
        serConnect.write(turnLeftCmd)
        waitingToMove=2
        waitingToMoveVar=angle
        print YELLOW+"TURNING LEFT "+str(angle)
    
def turnRight(angle):
    global robotMotorMoving, waitingToMove, waitingToMoveVar
    if(not robotMotorMoving and angle!=0):
        robotMotorMoving=True
        turnRightCmd = '3/%d/' % angle
        serConnect.write(turnRightCmd)
        waitingToMove=3
        waitingToMoveVar=angle
        print YELLOW+"TURNING RIGHT "+str(angle)

def stop():
    serConnect.write('4/')

def startMotor():
    global robotMotorMoving
    if(not robotMotorMoving):
        robotMotorMoving=True
        serConnect.write('5/')

def turnHead(angle):
    turnHead = '6/%d/' % angle
    serConnect.write(turnHead)

'''
functions for messages
'''
def gotMessageForPi(msg):#set message
    global booleanGotMsgForPi,messageForPi
    messageForPi = msg
    booleanGotMsgForPi = True
    #print('gotMessageForPi: '+str(msg))

def getMsgFromArduino():#send message to core
    global booleanGotMsgForPi,messageForPi
    if(booleanGotMsgForPi):
        booleanGotMsgForPi=False
        return messageForPi

def ifSupposeToBeStillMoving():
    while True:
        time.sleep(5)
        if(robotMotorMoving):
            if(waitingToMove==1):
                print "ifSupposeToBeStillMoving FORWARD" +str(waitingToMoveVar)
                moveForward(waitingToMoveVar)
            if(waitingToMove==2):
                print "ifSupposeToBeStillMoving LEFT" +str(waitingToMoveVar)
                turnLeft(waitingToMoveVar)
            if(waitingToMove==3):
                print "ifSupposeToBeStillMoving RIGHT" +str(waitingToMoveVar)
                turnRight(waitingToMoveVar)


def getSensorStatus():
    global countFrontObstacleTrue, countFrontObstacleFalse, countLeftObstacleTrue, countLeftObstacleFalse, countRightObstacleTrue, countRightObstacleFalse, countBottomObstacleTrue, countBottomObstacleFalse
    global countFrontDist80, countFrontDist70, countFrontDist60, countFrontDist50, countFrontDist40, countFrontDist30, countFrontDist20
    global collectingObsInfo

    collectingObsInfo=True
    time.sleep(0.5)
    collectingObsInfo=False

    sendFront=False
    sendLeft=False
    sendRight=False
    sendBottom=False
    #print ("andrino   "+ str(countBottomObstacleTrue)+"     " +str(countBottomObstacleFalse))
    if(countFrontObstacleTrue>=countFrontObstacleFalse):
        sendFront=True
    if(countLeftObstacleTrue>=countLeftObstacleFalse):
        sendLeft=True
    if(countRightObstacleTrue>=countRightObstacleFalse):
        sendRight=True
    if(countBottomObstacleTrue>=countBottomObstacleFalse):
        sendBottom=True

    '''
    calibrate distance
    '''
    if(sendFront):
        sendFrontDistArray = [countFrontDist80, countFrontDist70, countFrontDist60, countFrontDist50, countFrontDist40, countFrontDist30, countFrontDist20]
        sendFrontDistIndex = sendFrontDistArray.index(max(sendFrontDistArray))

        if(sendFrontDistIndex==0):
            sendFrontDist=80
        elif(sendFrontDistIndex==1):
            sendFrontDist=70
        elif(sendFrontDistIndex==2):
            sendFrontDist=60
        elif(sendFrontDistIndex==3):
            sendFrontDist=50
        elif(sendFrontDistIndex==4):
            sendFrontDist=40
        elif(sendFrontDistIndex==5):
            sendFrontDist=30
        elif(sendFrontDistIndex==6):
            sendFrontDist=20
    else:
        sendFrontDist=0

    returnValue = "front "+str(sendFront)+" "+str(sendFrontDist)+" left "+str(sendLeft)+" right "+str(sendRight)+" bottom "+str(sendBottom)

    countFrontObstacleTrue=0
    countFrontObstacleFalse=0
    countLeftObstacleTrue=0
    countLeftObstacleFalse=0
    countRightObstacleTrue=0
    countRightObstacleFalse=0
    countBottomObstacleTrue=0
    countBottomObstacleFalse=0

    countFrontDist80=0
    countFrontDist70=0
    countFrontDist60=0
    countFrontDist50=0
    countFrontDist40=0
    countFrontDist30=0
    countFrontDist20=0

    return returnValue

def waitForMsgFromArduino():#just retrun true or false
    return booleanGotMsgForPi

def detectObstacle(sensor,detected,distanceOfObstacle):
    global countFrontObstacleTrue, countFrontObstacleFalse, countLeftObstacleTrue, countLeftObstacleFalse, countRightObstacleTrue, countRightObstacleFalse, countBottomObstacleTrue, countBottomObstacleFalse
    global countFrontDist80, countFrontDist70, countFrontDist60, countFrontDist50, countFrontDist40, countFrontDist30, countFrontDist20
    global collectingObsInfo

    if(collectingObsInfo):
        if(sensor=="front"):
            try:
                distanceOfObstacle = int(distanceOfObstacle)
                if(distanceOfObstacle<=85 and distanceOfObstacle>75):
                    countFrontDist80 = countFrontDist80 + 1
                elif(distanceOfObstacle<=75 and distanceOfObstacle>65):
                    countFrontDist70 = countFrontDist70 + 1
                elif(distanceOfObstacle<=65 and distanceOfObstacle>55):
                    countFrontDist60 = countFrontDist60 + 1
                elif(distanceOfObstacle<=55 and distanceOfObstacle>45):
                    countFrontDist50 = countFrontDist50 + 1
                elif(distanceOfObstacle<=45 and distanceOfObstacle>35):
                    countFrontDist40 = countFrontDist40 + 1
                elif(distanceOfObstacle<=35 and distanceOfObstacle>25):
                    countFrontDist30 = countFrontDist30 + 1
                elif(distanceOfObstacle<=25):
                    countFrontDist20 = countFrontDist20 + 1
            except:
                pass
            
            if(detected==True):
                countFrontObstacleTrue=countFrontObstacleTrue+1
            elif(detected==False):
                countFrontObstacleFalse=countFrontObstacleFalse+1

        elif(sensor=="left"):
            if(detected==True):
                countLeftObstacleTrue=countLeftObstacleTrue+1
            elif(detected==False):
                countLeftObstacleFalse=countLeftObstacleFalse+1

        elif(sensor=="right"):
            if(detected==True):
                countRightObstacleTrue=countRightObstacleTrue+1
            elif(detected==False):
                countRightObstacleFalse=countRightObstacleFalse+1

        elif(sensor=="bottom"):
            if(detected==True):
                countBottomObstacleTrue=countBottomObstacleTrue+1
            elif(detected==False):
                countBottomObstacleFalse=countBottomObstacleFalse+1
'''
def getDistanceTravelled():#not working, so not using it yet
    global distanceTravelled
    if(distanceTravelled==0):
        getDistanceMoved()
        return False
    else:
        print "ooooooooooooooooooooooooooooooooooooooooooooooooo"+distanceTravelled
        distT=distanceTravelled
        distanceTravelled=0
        return distT

def setDistanceTravelled(dist):
    global distanceTravelled
    distanceTravelled = dist
'''
def getRobotMotorMovingStatus():
    time.sleep(0.1)
    return robotMotorMoving

def listenHandler():
    global robotMotorMoving
    while True:
        if (serConnect.inWaiting() > 0):

            try:
                receivedInput=""
                notComplete=True
                while notComplete:
                    incoming = str(serConnect.read(1))
                    if(incoming!=";"):
                        receivedInput = receivedInput + incoming
                    else:
                        notComplete=False

                receivedInputSplit = receivedInput.split("/")
                caseNum = receivedInputSplit[0]

                try:
                    value = receivedInputSplit[1]
                except:
                    pass

                for case in switch(caseNum):
                    if case('51'):
                        if(waitingToMove==1):
                            robotMotorMoving=False
                            #print "finshed moving FORWARD"
                        break
                    if case('52'):
                        if(waitingToMove==2):
                            robotMotorMoving=False
                            #print "finshed moving LEFT"
                        break
                    if case('53'):
                        if(waitingToMove==3):
                            robotMotorMoving=False
                            #print "finshed moving RIGHT"
                        break
                    if case('1'):
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",False,value)
                        break
                    if case('2'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",False,value)
                        break
                    if case('3'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",False,value)
                        break
                    if case('4'):
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",False,value)
                        break
                    if case('5'):                  
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",False,value)
                        break
                    if case('6'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",False,value)
                        break
                    if case('7'):
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",False,value)
                        break
                    if case('8'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",True,value)
                        break
                    if case('9'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",True,value)
                        break
                    if case('10'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",True,value)
                        break
                    if case('11'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",True,value)
                        break
                    if case('12'):
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",True,value)
                        break
                    if case('13'):
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",True,value)
                        break
                    if case('14'):
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",True,value)
                        break
                    if case('15'):
                        detectObstacle("bottom",True,value)
                        detectObstacle("left",True,value)
                        detectObstacle("right",True,value)
                        detectObstacle("front",True,value)
                        break
                    if case('16'):
                        detectObstacle("bottom",False,value)
                        detectObstacle("left",False,value)
                        detectObstacle("right",False,value)
                        detectObstacle("front",False,value)
                        break
                    '''
                    if case('17'):#not in use
                        setDistanceTravelled(value)
                        print "dist travelled received "+value
                        break
                    '''
                        
                    if case():
                        #if(robotMotorMoving):
                            #ifSupposeToBeStillMoving()
                        break
            except:
                pass
