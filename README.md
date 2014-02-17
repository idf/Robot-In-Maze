ntu.sce.mdp.15
==============
TO BE THE FASTEST.  
Swifter, stronger, and more precise.  

Arduino Requirements
--------------
2 spaces as tab  

PololuWheelEncoder is using ISR(PCINT0_vect), interrupt Service Routine  
!important Avoid Serial printing in Polling
abs() in Arduino has adverse effect; thus do not use it on function's return value  

NAMING CONVENTION  
method_name  
functionName  

COMMUNICATION
--------------
Command to be executed is using 7-digit machine code since it saves memory on Arduino;
```python
machine_code = "xxxxxxx"
```
machine_code[0:2] constructs 2-digit function codes  
machine_code[2:7] constructs parameter with 2 decimal number  
example:  
```python
"0012312"
```
This code is translated into function code 00 with parameter 123.12  
The example commands Arduino to moveForwad by 5 cm.
List of function codes:  
00: void moveForward(double dist);  
01: void turnRight(double angle);  
02: void turnLeft(double angle);  

NOTICE: In python each json serial command at very end must end with 
```python
'\n'
```

Feedback:
Json format  http://docs.python.org/2/library/json.html  
```json
{  
  "function": function_code,  
  "parameter": status_code  
}  
```
status_code is the HTTP status code as in http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html  
The most common status code is 200 (i.e. OK).
