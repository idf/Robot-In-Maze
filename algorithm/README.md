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
Sending Map information and Robot location to Android: { "map" : "000211002003000040002" , "location" : "x,y" }
```
// 0: free, 1: obstacle, 2: start, 3: goal, 4: unexplored
// data of map: row by row
Receiving:
```json
Request for sensor data : { "1" : "*data*",
							"2" : "*data*",
							"3" : "*data*"}
```