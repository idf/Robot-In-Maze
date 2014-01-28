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
    Serial.begin(115200); // data rate in bits per second 
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
  
    md.setM2Speed(SPEED);
    md.setM1Speed(SPEED);
    
      /*
    accelerateTo(md, 0, SPEED, DELAY);
    delay(1000);
    accelerateTo(md, SPEED, 0, DELAY);
    accelerateTo(md, 0, -SPEED, DELAY);
    delay(1000);
    accelerateTo(md, -SPEED, 0, DELAY);
    */
    
    //M1 & M2
    /*
    for (int i = 0; i <= 400; i++)
    { 
        md.setM1Speed(i); stopIfFault(); 
        md.setM2Speed(i); stopIfFault(); 
        if (i%200 == 100)
        {
            Serial.print("M1 current: ");
            Serial.println(md.getM1CurrentMilliamps());
        } 
        if (i%200 == 100)
        {
            Serial.print("M2 current: ");
            Serial.println(md.getM2CurrentMilliamps());
        } 
        delay(2);
    }
    
    for (int i = 400; i >= -400; i--)
    { 
        md.setM1Speed(i); stopIfFault(); 
        md.setM2Speed(i); stopIfFault(); 
        if (i%200 == 100)
        {
            Serial.print("M1 current: ");
            Serial.println(md.getM1CurrentMilliamps());
        } 
        if (i%200 == 100)
        {
            Serial.print("M2 current: ");
            Serial.println(md.getM2CurrentMilliamps());
        } 
        delay(2);
    }
    for (int i = -400; i <= 0; i++)
    { 
        md.setM1Speed(i); stopIfFault(); 
        md.setM2Speed(i); stopIfFault(); 
        
        if (i%200 == 100)
        {
            Serial.print("M1 current: ");
            Serial.println(md.getM1CurrentMilliamps());
        } 
        if (i%200 == 100)
        {
            Serial.print("M2 current: ");
            Serial.println(md.getM2CurrentMilliamps());
        } 
        delay(2);
    }*/
    
}
