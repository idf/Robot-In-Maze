import arduino
import utility
import comms
import threading
import time
import bluetooth
import string

#user define
startPosition=[4,4]
endPosition=[36,26]
#!!!!!!!!! change here!!!!!!!!!!!
#startAngle=180
startAngle=90

cmPerBox=5
safetyDist=10

#var declare
obstacles=[]
displayMap=[]
maxDist=99
runningPhrase=0
connectionWithPc = False

currentPosition=startPosition
currentAngle=startAngle
width=int(200/cmPerBox)+1
height=int(150/cmPerBox)+1
startPosition=[ int(startPosition[0]*(5/cmPerBox)) , int(startPosition[1]*(5/cmPerBox)) ]
#=[ int(endPosition[0]*(5/cmPerBox)) , int(endPosition[1]*(5/cmPerBox)) ]

btAddr = "08:60:6E:A4:CA:D6"
btPort = 4
isBluetoothConnected=False
btSock = None

phraseTwoPath=[]
returnPath=[]

GREEN = '\033[92m'
YELLOW = '\033[93m'
RED = '\033[91m'
WHITE = '\033[0m'

def init(width,height):
    global connectionWithPc
    for i in range(height):
        displayMap.append([])
        for j in range(width):
            displayMap[i].append('0')

    for i in range(height):
        for j in range( int(safetyDist/cmPerBox) ):
            displayMap[i][j]=2
            displayMap[i][width-1-j]=2
    for i in range( int(safetyDist/cmPerBox) ):
        for j in range( width ):
            displayMap[i][j]=2
            displayMap[height-1-i][j]=2
    checkInput()
    setCoordAs(startPosition[0],startPosition[1],'S')
    setCoordAs(endPosition[0],endPosition[1],'G')

    connectionWithPc = comms.startListeningFromPC()
    threading.Thread(target=arduino.listenHandler).start()
    threading.Thread(target=arduino.ifSupposeToBeStillMoving).start()
    #threading.Thread(target=bluetoothConnection).start()

    sendMessages("map initMap", 3)
    sendMessages("currentPosition "+str(currentPosition[0])+" "+str(currentPosition[1]),3)
    sendMessages("goalPosition "+str(endPosition[0])+" "+str(endPosition[1]),3)
    #sendMessages("map initMap "+str(cmPerBox)+" "+str(safetyDist)+" "+str(startPosition[0])+" "+str(startPosition[1])+" "+str(endPosition[0])+" "+str(endPosition[1]),3)
    
    setRunningPhrase(0)

def checkInput():
    if (endPosition[0]>width-(10/cmPerBox) or endPosition[1]>height-(10/cmPerBox) or endPosition[0]<(10/cmPerBox) or endPosition[1]<(10/cmPerBox)):
        print (RED+"ERR: invalid end post")
        sendMessages("msg ERR: invalid end post",3)
    if (startPosition[0]>width-(10/cmPerBox) or startPosition[1]>height-(10/cmPerBox) or startPosition[0]<(10/cmPerBox) or startPosition[1]<(10/cmPerBox)):
        print (RED+"ERR: invalid start post")
        sendMessages("msg ERR: invalid start post",3)

def setCoordAs(x,y,value):
    displayMap[y][x]=value
'''
def setObstacleLargeSensor(distance):
    #print "apppppp"
    #print distance
    x=currentPosition[0]
    y=currentPosition[1]
    n = distance/5
    if(currentAngle==0):
            y1=y-n-3
            x1=x+1
            y2=y-n-3
            x2=x-1
                
    if(currentAngle==90):
            y1=y-1
            x1=x+n+3
            y2=y+1
            x2=x+n+3
    if(currentAngle==180):
            y1=y+n+3
            x1=x-1
            y2=y+n+3
            x2=x+1


    if(currentAngle==270):
            y1=y-1
            x1=x-n-3
            y2=y+1
            x2=x-n-3
    #print "aaa"
    #print [x1,y1]
    #print [x2,y2]
    sendMessages("map setObstacle "+str(x1)+" "+str(y1),1) 
    sendMessages("map setObstacle "+str(x2)+" "+str(y2),1) 

'''

#new used!!!
def setObstacle(sensor,distance):
    '''
    print "---------"
    print sensor
    print distance
    print currentPosition
    print currentAngle
    '''
    #print "inside set obstacle"
    n=distance/cmPerBox
    #print currentAngle
    x=currentPosition[0]
    y=currentPosition[1]
    #print ("x and y  "+str([x,y]))
    if(currentAngle==0):
        if(sensor=='right'):
            y1=y-3-n
            x1=x+1
            
        if(sensor=='left'):
            y1=y-3-n
            x1=x-1
            
    if(currentAngle==90):
        if(sensor=='left'):
            y1=y-1
            x1=x+3+n
        if(sensor=='right'):
            y1=y+1
            x1=x+3+n


    if(currentAngle==180):
        if(sensor=='right'):
            y1=y+3+n
            x1=x-1
        if(sensor=='left'):
            y1=y+3+n
            x1=x+1


    if(currentAngle==270):

        if(sensor=='right'):
            y1=y-1
            x1=x-3-n
        if(sensor=='left'):
            print "aaaaa"
            y1=y+1
            x1=x-3-n
            print x1
    #print "aaa"
    #print [x1,y1]
    #print [x2,y2]
    sendMessages("map setObstacle "+str(x1)+" "+str(y1),1) 


def printMap():
    temp=""
    count=0
    for i in range(height):
        for j in range(width):
            if(displayMap[i][j]=='*'):
                count=count+1
            if(i==currentPosition[1] and j==currentPosition[0]):
                temp = temp + "C "
            elif(displayMap[i][j]=='0'):
                temp = temp + "  "
            else:
                temp = temp + str(displayMap[i][j])+" "
        temp = temp +'\n'
    print (GREEN+temp)
    print (GREEN+"Width: "+str(width) + ", Height: "+str(height)+", Total steps: "+str(count))

def setRunningPhrase(phrase):
    global runningPhrase
    runningPhrase = phrase
    sendMessages("phrase starting "+str(runningPhrase),3)
    print "phrase starting "+str(runningPhrase)
'''
bluetooth
'''
def bluetoothConnection():
    global btSock, isBluetoothConnected, btAddr

    while True:
        if not isBluetoothConnected:
            print YELLOW+"Trying to connect bluetooth"
            try:
                time.sleep(0.5)
                btSock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
                btSock.connect((btAddr, btPort))
                isBluetoothConnected=True;
                print(GREEN+"Bluetooth connection established")
                sendMessages("init", 2) 
                threading.Thread(target=listenBluetoothSocket).start()
            except Exception as err:
                print "Bluetooth error:", err
            time.sleep(3)   # Attempt to reconnect
        else:
            time.sleep(5) # Check for connection

def listenBluetoothSocket():
    global btSock, isBluetoothConnected
    while True:
        try:
            data=btSock.recv(1024)
            if len(data) != 0:
                print(GREEN+'Received "%s" from Nexus 7' % str(data))
                bluetoothHandler(data)
        except bluetooth.btcommon.BluetoothError as error:
            isBluetoothConnected=False;
            print RED+"Reciever bluetooth error"
        except Exception as err:
            print RED+"Reciever bluetooth error:", err
        time.sleep(0.1)

def sendBluetoothSocket(message):
    global btSock, isBluetoothConnected
    attempts = 0
    while True:
        try:
            btSock.send(message)
            return True
        except bluetooth.btcommon.BluetoothError as error:
            isBluetoothConnected=False;
            print RED+"Bluetooth sender error"
        except Exception as err:
            print RED+"Bluetooth sender error:", err
        if attempts == 10:
            print RED+'Failed to send message to Nexus 7 after 10 attempts'
            return False
        attempts += 1
        time.sleep(0.1)

def bluetoothHandler(msg):
    if (msg[:3] == "cmd"):
        if msg[4:] == "up":
            manualControl("up")
        if msg[4:] == "down":
            manualControl("down")
        if msg[4:] == "left":
            manualControl("left")
        if msg[4:] == "right":
            manualControl("right")
        if msg[4:] == "start":
            startCompetition()
        if msg[4:] == "return":
            startReturn()        


def sendMessages(message,towho):
    if(towho==1):
        comms.sendMsg(message)
    elif(towho==2 and isBluetoothConnected):
        sendBluetoothSocket(message)
    elif(towho==3):
        comms.sendMsg(message)
        if(isBluetoothConnected):
            sendBluetoothSocket(message)

''' 
arduino functions
'''
def moveFromCurrentPositionTo(toLocation):
    global currentPosition
    angleToTurn = utility.getAngle(currentPosition, toLocation, currentAngle) 
    distanceToMove = utility.getDistance(currentPosition, toLocation, cmPerBox)
    turn(angleToTurn)
    while arduino.getRobotMotorMovingStatus():
        pass
    move(distanceToMove)
    while arduino.getRobotMotorMovingStatus():
        pass
    print WHITE+"turn: "+str(angleToTurn)+" distance: "+str(distanceToMove)
    currentPosition=toLocation

def manualControl(updownleftright):
    global currentPosition
    sendMessages("msg manualControl: "+updownleftright,2)
    if(runningPhrase==0):
        if(updownleftright=="up"):
            if(currentAngle<180):
                angle = 0 - currentAngle
            else:
                angle = 360 - currentAngle
        if(updownleftright=="right"):
            if(currentAngle<270):
                angle = 180 - (currentAngle + 90)
            else:
                angle = 450 - currentAngle
        if(updownleftright=="down"):
            angle = 180 - currentAngle
        if(updownleftright=="left"):
            if(currentAngle<90):
                angle = -currentAngle - 90
            else:
                angle = -currentAngle + 270

        turn(angle)
        while arduino.getRobotMotorMovingStatus():
            pass
        move(10)

def turn(angle):
    global currentAngle
    currentAngle = int(currentAngle)
    if(angle<-180):
        arduino.turnRight(-angle-180)
    elif(angle>180):
        arduino.turnLeft(angle-180)
    elif(angle<0):
        arduino.turnLeft(-angle)
    elif(angle>=0):
        arduino.turnRight(angle)

    currentAngle=(currentAngle+angle)%360
    sendMessages("currentAngle "+str(currentAngle),3)
    print GREEN+"currentAngle "+str(currentAngle)

def move(distance):
    distance=int(distance)
    arduino.moveForward(distance)
    sendMessages("currentPosition "+str(currentPosition[0])+" "+str(currentPosition[1]),3)
    print GREEN+"currentPosition "+str(currentPosition[0])+" "+str(currentPosition[1])

    if (currentAngle==0):
        currentPosition[1]=currentPosition[1]-(distance/cmPerBox)
    elif (currentAngle==90):
        currentPosition[0]=currentPosition[0]+(distance/cmPerBox)
    elif (currentAngle==180):
        currentPosition[1]=currentPosition[1]+(distance/cmPerBox)
    elif (currentAngle==270):
        currentPosition[0]=currentPosition[0]-(distance/cmPerBox)












def phraseOne():
    setRunningPhrase(1)
    threading.Thread(target=utility.startTimer).start()
    path = []
    goBack=False
    getPath=True
    #getPath = True

    #turn 90 at very first

    while True:

        #to return on the same path used
        isPath=True

        time.sleep(1)
        if(endPosition==currentPosition):
            sendMessages("Reached EndPosition in "+str(utility.getTime())+" seconds",3)
            #print "=========================>>>>> reach endpoint"
            #phraseOneCheckIfAtGoal()
            break

        try:
            if(currentPosition==path[0]):
                path.pop(0)
        except:
            pass
        #print "dddd"
        #if getPath is true, get optimal path (90degrees)
        if(getPath or len(path)==0):
            getPath = False
            sendMessages("currentAngle "+str(currentAngle),3)
            sendMessages("currentPosition "+str(currentPosition[0])+" "+str(currentPosition[1]),3)
            sendMessages("phraseOneFindOptimalPath",1)
            returnMsgInString = comms.getMsg()
            #print "aaaaaa"
            #print returnMsgInString
            returnMsgInString = returnMsgInString.split(';')
            pathInString = returnMsgInString[0]
            pathInString = pathInString.translate(string.maketrans('', ''), '[] ')
            pathInString = pathInString.split(',')
            
            
            changeSafetyDistance = utility.parseBoolString(returnMsgInString[1]);

            #print "bbbb"
            #print changeSafetyDistance
            #print pathInString

            '''
            if (pathInString=='[]'):
                goBack=True
                continue
            '''

            
            
            
            path=[]
            i=0
            #print "======== "+str(pathInString)

            
            while i<len(pathInString):
                path.append([int(pathInString[i]),int(pathInString[i+1])])
                i=i+2
            print "get new path:::"
            print path

        #base on path, get direction to face
        directionToFace = utility.getAngle(currentPosition, path[0], currentAngle)
        distanceToNextPoint = utility.getDistance(currentPosition, path[0], cmPerBox)
        #print ("direction  "+str(directionToFace))
        #print ("distance   "+str(distanceToNextPoint)) 
        print "currentPosition = "+str(currentPosition)
        if(directionToFace!=0):
            if(currentPosition==startPosition):
                isPath=False
            elif(len(returnPath)>0):
                if(returnPath[0]==currentPosition):
                    isPath=False
            if(isPath):
                newPoint=[0,0]
                newPoint[0]=currentPosition[0]
                newPoint[1]=currentPosition[1]
                returnPath.insert(0,newPoint)
            turn(directionToFace)
            while arduino.getRobotMotorMovingStatus():
                pass
        #print (WHITE+"current position  "+str(currentPosition))
        #print (WHITE+"current angle  "+str(currentAngle))
        #print (WHITE+"disatnce to next point"+str(distanceToNextPoint))
        #check left and right sensor for obs
        sensorValues = arduino.getSensorStatus()
        #print WHITE+"sensor values"
        #print WHITE+sensorValues
        sensorValues = sensorValues.split()
        
        isLeftObstacle=utility.parseBoolString(sensorValues[4])
        isRightObstacle=utility.parseBoolString(sensorValues[6])

        print "sensor values:::::"
        print sensorValues
        if (currentPosition[0]>=34 and currentPosition[1]>=24 and (isRightObstacle or isLeftObstacle)):
            sendMessages("Reached EndPosition in "+str(utility.getTime())+" seconds",3)
            print "not very accurate, but regard as reached"
            #print "=========================>>>>> reach endpoint"
            #phraseOneCheckIfAtGoal()
            break


        if(isRightObstacle):#if right sensor is true
            #print ("current "+str(currentPosition))
            #print "currentPosition = "+str(currentPosition)
            #print "left obstacle"
            setObstacle("right",10)
            obsTrueCoords = comms.getMsg()
            sendMessages("map setObstacle "+obsTrueCoords,2)
            getPath=True

        #bottom sensor is for front left
        if(isLeftObstacle):#if bottom sensor is true, front obs within 10cm
            #print ("current "+str(currentPosition))
            #print "currentPosition = "+str(currentPosition)
            #print "right obstacle"
            setObstacle("left",10)
            obsTrueCoords = comms.getMsg()
            sendMessages("map setObstacle "+obsTrueCoords,2)
            getPath=True
        
        if ((not getPath) or changeSafetyDistance):
            move(10)
            changeSafetyDistance=False
        else:
            continue


    
        while arduino.getRobotMotorMovingStatus():
            pass
        continue
    endingCheck()
'''

def phraseOnePartTwo():
    pass

#MAYBE DONT NEED TO CHECK!!!!!
#!!!!!!!
#!!!!!!!

def phraseOneCheckIfAtGoal():
    if(currentAngle==0):
        turn(90)
    elif(currentAngle==180):
        turn(-90)
    elif(currentAngle==270):
        turn(180)

    while arduino.getRobotMotorMovingStatus():
        pass

    sensorValues = arduino.getSensorStatus()
    print WHITE+"phraseOneCheckIfAtGoal "+sensorValues
    sensorValues = sensorValues.split()

    if(utility.parseBoolString(sensorValues[6]) and utility.parseBoolString(sensorValues[8])):
        sendMessages("Reached EndPosition in "+str(utility.getTime())+" seconds",3)
    elif(utility.parseBoolString(sensorValues[6])):
        arrayScan=doThreeHeadScan()
        distOfFrontObs=min(arrayScan)
        move(distOfFrontObs-10)
        while arduino.getRobotMotorMovingStatus():
            pass
        sendMessages("Reached EndPosition in "+str(utility.getTime())+" seconds",3)
    elif(utility.parseBoolString(sensorValues[8])):
        turn(90)
        while arduino.getRobotMotorMovingStatus():
            pass
        sensorValues = arduino.getSensorStatus()
        print "phraseOneCheckIfAtGoal"+sensorValues
        sensorValues = sensorValues.split()

        if(utility.parseBoolString(sensorValues[8])):
            sendMessages("Reached EndPosition in "+str(utility.getTime())+" seconds",3)
        else:
            arrayScan=doThreeHeadScan()
            distOfFrontObs=min(arrayScan)
            move(distOfFrontObs-10)
            while arduino.getRobotMotorMovingStatus():
                pass
            sendMessages("Reached EndPosition in "+str(utility.getTime())+" seconds",3)
'''

def phraseTwo():
    global phraseTwoPath
    print "phase two begin"
    global returnPath, phaseTwoPath
    returnPath.append(startPosition)
    print "RETURN PATH:   "+str(returnPath)
    setRunningPhrase(2)
    utility.startTimer()
    
    pathTwo = phaseTwoPath
    #pathTwo=[]
    print "==========="
    print pathTwo
    if(pathTwo==[]):
        print "no optimal path find so go back on the same path as phase one"
        pathTwo=returnPath
    
    while True:
        time.sleep(0.01)

        if(len(pathTwo)!=0):
            print "aaaaaaaaaaaa"
            moveFromCurrentPositionTo(pathTwo[0])
            pathTwo.pop(0)
        elif(currentPosition==startPosition):
            sendMessages("Reached StartPosition, total running time: "+str(utility.getTime())+" seconds",3)
            break

    startingCheck()
    

def startingCheck():
    sensorValues = arduino.getSensorStatus()

    sensorValues = sensorValues.split()
        
    isLeftObstacle=utility.parseBoolString(sensorValues[4])
    isRightObstacle=utility.parseBoolString(sensorValues[6])
    print ("==================================")
    print sensorValues

    if (not(isLeftObstacle or isRightObstacle)):
        move(5)
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())

    if(currentAngle==0):
        turn(-90)
        while arduino.getRobotMotorMovingStatus():
                print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    if(currentAngle==270):
        turn(90)
        while arduino.getRobotMotorMovingStatus():
                print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    if (not(isLeftObstacle or isRightObstacle)):
        move(5)
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())

def endingCheck():
    sensorValues = arduino.getSensorStatus()

    sensorValues = sensorValues.split()
        
    isLeftObstacle=utility.parseBoolString(sensorValues[4])
    isRightObstacle=utility.parseBoolString(sensorValues[6])
    print ("==================================")
    print sensorValues

    if (not(isLeftObstacle or isRightObstacle)):
        move(5)
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())

    if(currentAngle==90):
        turn(90)
        while arduino.getRobotMotorMovingStatus():
                print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    if(currentAngle==180):
        turn(-90)
        while arduino.getRobotMotorMovingStatus():
                print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    if (not(isLeftObstacle or isRightObstacle)):
        move(5)
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())


def getPhraseTwoPath():
    global phraseTwoPath

    sendMessages("currentAngle "+str(currentAngle),3)
    sendMessages("currentPosition "+str(currentPosition[0])+" "+str(currentPosition[1]),3)

    sendMessages("phraseTwoFindOptimalPath",1)
    #print "bbbb"
    pathInString = comms.getMsg()
    #print "aaaa"

    if pathInString=='[]':
        return []

    pathInString = pathInString.translate(string.maketrans('', ''), '[] ')
    #print "bbbb"
    
    pathInString = pathInString.split(',')
    phraseTwoPath=[]
    i=0
    while i<len(pathInString):
        phraseTwoPath.append([int(pathInString[i]),int(pathInString[i+1])])
        i=i+2
    sendMessages("phraseTwoPath: "+str(phraseTwoPath),3)
    return phraseTwoPath

def startCompetition():

    global phaseTwoPath
    phraseOne()
    phaseTwoPath=getPhraseTwoPath()
    #print "aaaa"
    #getPhraseTwoPath()

    #to be remove
    time.sleep(5)
    phraseTwo()

def startReturn():
    phraseTwo()

def testTurning():
    for i in range (10):
        turn(90)
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    
        move(30)
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
            
    time.sleep(10) 
    for i in range (10):
        turn(-90)
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
        move(30)        
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    


def testMotorMovingStatus():
    time.sleep(3)
    time.sleep(0.1)
    print 1
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    print 2
    move(30)
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    print 3

def testSensor():
    for i in range(3):
        time.sleep(0.1)
        print WHITE+"testSensor: "+arduino.getSensorStatus()
    turn(90)
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    for i in range(3):
        time.sleep(0.1)
        print WHITE+"testSensor: "+arduino.getSensorStatus()
    turn(-180)
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    for i in range(3):
        time.sleep(0.1)
        print WHITE+"testSensor: "+arduino.getSensorStatus()
    turn(90)
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    for i in range(3):
        time.sleep(0.1)
        print WHITE+"testSensor: "+arduino.getSensorStatus()

def testSensorSimple():
    for i in range(3):
        time.sleep(0.1)
        print WHITE+"testSensor: "+arduino.getSensorStatus()
    turn(90)
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
    for i in range(3):
        time.sleep(0.1)
        print WHITE+"testSensor: "+arduino.getSensorStatus()
    turn(-90)
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())

def testHead():
    for i in range(3):
        time.sleep(2)
        firstScan=turnHeadAndReturnDist(80)
        secondScan=turnHeadAndReturnDist(90)
        thirdScan=turnHeadAndReturnDist(100)
        print WHITE+str(firstScan)+" "+str(secondScan)+" "+str(thirdScan)

def testSeriesOfCoords():
    pathTwo=[[6,4],[6,6],[4,4],[2,3],[4,4]]
    for i in range(len(pathTwo)):
        moveFromCurrentPositionTo(pathTwo[i])
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())

def testSensorInf():
    while(1):
        time.sleep(0.1)
        print WHITE+"testSensor: "+arduino.getSensorStatus()

def motorSensorChecks():
    pathTwo=[[6,4],[6,6],[6,4],[3,4],[4,4]]
    for i in range(len(pathTwo)):
        time.sleep(1)
        moveFromCurrentPositionTo(pathTwo[i])
        while arduino.getRobotMotorMovingStatus():
            print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
        print WHITE+arduino.getSensorStatus()
    testHead()
'''
def testHeadAll():
    scan1=turnHeadAndReturnDist(10)
    scan2=turnHeadAndReturnDist(20)
    scan3=turnHeadAndReturnDist(30)
    scan4=turnHeadAndReturnDist(40)
    scan5=turnHeadAndReturnDist(50)
    scan6=turnHeadAndReturnDist(60)
    scan7=turnHeadAndReturnDist(70)
    scan8=turnHeadAndReturnDist(80)
    scan9=turnHeadAndReturnDist(90)
    scan10=turnHeadAndReturnDist(100)
    scan11=turnHeadAndReturnDist(110)
    scan12=turnHeadAndReturnDist(120)
    scan13=turnHeadAndReturnDist(130)
    scan14=turnHeadAndReturnDist(140)
    scan15=turnHeadAndReturnDist(150)
    scan16=turnHeadAndReturnDist(160)
    scan17=turnHeadAndReturnDist(170)
    print str(scan1)+" "+str(scan2)+" "+str(scan3)+" "+str(scan4)+" "+str(scan5)+" "+str(scan6)+" "+str(scan7)+" "+str(scan8)+" "+str(scan9)+" "+str(scan10)+" "+str(scan11)+" "+str(scan12)+" "+str(scan13)+" "+str(scan14)+" "+str(scan15)+" "+str(scan16)+" "+str(scan17)
    time.sleep(5)

'''




    

'''init start'''
init(width,height)
#testTurning()
#testTurning()

#time.sleep(15)

'''
for i in range(15):
    move(10)
    while arduino.getRobotMotorMovingStatus():
        print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())
'''
testSensorSimple()

startCompetition()
#motorSensorChecks()
#testSensorInf()

'''
print "move50"
move(50)

while arduino.getRobotMotorMovingStatus():
    print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())

print "turn90"
turn(90)


while arduino.getRobotMotorMovingStatus():
    print WHITE+"arduino.getRobotMotorMovingStatus "+str(arduino.getRobotMotorMovingStatus())

print "turn90"
turn(-90)
'''
'''
testHead()
'''


