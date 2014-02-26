import math
import time

startTime = 0


def getAngle(fromPoint, toPoint, facingAngle):
    deltaX = toPoint[0] - fromPoint[0]
    deltaY = toPoint[1] - fromPoint[1]
    angle = (math.atan2(deltaY, deltaX) * 180 / math.pi)
    angle = angle + 90 - facingAngle
    if (angle>180):
        angle = angle - 360
    return int(angle)


def getAngleOne(fromPoint, toPoint, facingAngle):
    direction = [0, 0]
    direction[0] = toPoint[0] - fromPoint[0]
    direction[1] = toPoint[1] - fromPoint[1]
    print ("angle one" + str(direction))
    if (direction[1]>0):
        angle = 180
    if (direction[0]>0):
        angle = 90
    if (direction[1]<0):
        angle = 0
    if (direction[0]<0):
        angle = 270

    turning = angle - facingAngle

    return turning


def getDistance(fromPoint, toPoint, cmPerBox):
    a2 = math.pow((toPoint[0] - fromPoint[0]) * cmPerBox, 2)
    b2 = math.pow((toPoint[1] - fromPoint[1]) * cmPerBox, 2)
    return int(round(math.sqrt(a2 + b2)))

#return int( math.hypot( toPoint[0]-fromPoint[0] , toPoint[1]-fromPoint[1] ) ) * cmPerBox #<--- old

def startTimer():
    global startTime
    startTime = time.time()


def getTime():
    return int(time.time() - startTime)


def parseBoolString(theString):
    return theString[0].upper()=='T'