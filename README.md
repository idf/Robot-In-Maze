ntu.sce.mdp.15
==============
TO BE THE FASTEST.  
Swifter, stronger, and more precise.  

Arduino Requirements  
2 spaces as tab  

PololuWheelEncoder is using ISR(PCINT0_vect), interrupt Service Routine  
!important Avoid Serial printing in Polling

COMMUNICATION  
Json format  http://docs.python.org/2/library/json.html  
```json
{  
  "function": "function_name",  
  "parameter": parameter,  
}  
```
example:  
```json
{  
  "function": "moveForward",  
  "parameter": 10.00  
}  
```
List of functions: void moveForward(double dist); void turnRight(int angle); void turnLeft(int angle);  
