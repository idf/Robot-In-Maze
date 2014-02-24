ntu.sce.mdp.15
==============
TO BE THE FASTEST.  
Swifter, stronger, and more precise.  
Richard is in! --20.1.2014
Siyao is in! --22.1.2014

Communication
==============
Sending:
```json
Move forward: { "function" : "forward",
				"parameter" : "*distance*"}
Turn Right: { "function" : "right",
			  "parameter" : "*degree*"}
Turn Left: { "function" : "left",
			 "parameter" : "*degree*"}
Request for sensor data : { "function" : "sensor" }
```
Receiving:
```json
Request for sensor data : { "1" : "*data*",
							"2" : "*data*",
							"3" : "*data*"}
```