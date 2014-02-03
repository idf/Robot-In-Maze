#include "DualVNH5019MotorShield.h" 
DualVNH5019MotorShield md;
void stopIfFault()
{ 
    if (md.getM1Fault())
    {
        Serial.println("M1 fault"); while(1);
    } 
    if (md.getM2Fault())
    {
        Serial.println("M2 fault"); while(1);
    }
}
int const SPEED = 400;
int const DELAY = SPEED/80;
void setup()
{
    Serial.begin(9600); // data rate in bits per second 
    Serial.println("Dual VNH5019 Motor Shield");
    md.init();
}
void accelerateTo(DualVNH5019MotorShield md,int initialSpeed, int targetSpeed, int delayTime)
{ 
    for (int i = initialSpeed; i != targetSpeed ; )
    { 
        if(initialSpeed > targetSpeed)
        {
            i--;
        }
        else
        {
            i++;
        }
        md.setSpeeds(i, i); stopIfFault();  
        delay(delayTime);
    }
}

void loop()
{
    accelerateTo(md, 0, SPEED, DELAY);
    delay(1000);
    accelerateTo(md, SPEED, 0, DELAY);
    accelerateTo(md, 0, -SPEED, DELAY);
    delay(1000);
    accelerateTo(md, -SPEED, 0, DELAY);

}
