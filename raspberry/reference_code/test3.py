import utility


def turn(angle):
    currentAngle = int(90)

    if (angle<-180):
        print "turnRight" + str(-angle - 180)

    elif (angle>180):
        print "turnLeft" + str(angle - 180)
    elif (angle<0):
        print "turnLeft" + str(-angle)

    elif (angle>=0):
        print "turnRight" + str(angle)

    currentAngle = (currentAngle + angle) % 360


angleToTurn = utility.getAngle([7, 7], [7, 3], 270)

print angleToTurn

turn(-270)


