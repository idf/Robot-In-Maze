ntu.sce.mdp.15
==============
TO BE THE FASTEST.  
Swifter, stronger, and more precise.  

Arduino Requirements  
2 spaces as tab  
PID_v1.h install from https://github.com/br3ttb/Arduino-PID-Library/  
Max PWM duty cycle: 400.  

PololuWheelEncoder is using ISR(PCINT0_vect), interrupt Service Routine  
!important Avoid Serial printing in Polling

COMMUNICATION  
Json format  http://docs.python.org/2/library/json.html  
{  
  "function": "function_name",  
  "parameter": parameter,  
}  
example:  
{
  "function": "moveForward",  
  "parameter": 10.00  
}  