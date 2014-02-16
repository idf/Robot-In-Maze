ntu.sce.mdp.15
==============
TO BE THE FASTEST.  
Swifter, stronger, and more precise.  

Arduino Requirements  
2 spaces as tab  

PololuWheelEncoder is using ISR(PCINT0_vect), interrupt Service Routine  
!important Avoid Serial printing in Polling
abs() in Arduino has adverse effect; thus do not use it on function's return value  

NAMING CONVENTION  
method_name  
functionName  

COMMUNICATION  
Json format  http://docs.python.org/2/library/json.html  
```json
{  
  "function": function_code,  
  "parameter": parameter  
}  
```
example:  
```json
{  
  "function": 0,  
  "parameter": 5  
}  
```
NOTICE: In python each json serial command at very end must end with 
```python
'\n'
some_serial_write_function(json.dumps(json_command)+'\n')
```
The example commands Arduino to moveForwad by 5 cm.
List of functions:  
0: void moveForward(double dist);  
1: void turnRight(double angle);  
2: void turnLeft(double angle);  

Feedback:
```json
{  
  "function": function_code,  
  "parameter": status_code  
}  
```
status_code is the HTTP status code as in http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html  
The most common status code is 200 (i.e. OK).
