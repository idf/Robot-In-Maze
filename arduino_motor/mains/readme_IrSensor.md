Accurate Range
============
15cm  - 30 cm



Code
============
```cpp
#include <SharpIR.h> 
SharpIR* sharpShort = new SharpIR(irPin, 250, 95, 1080); 
// AnalogPin, times iteartion per reading, Screening outlier 95/100, Model Number (1080 short, 20150 for long)
sharpShort->distance(); 
// get the distance, :int
```

Calibration
============
10cm:  
Ultrasound Distance: 10  
SharpIR distance long: 19  
20cm:  
Ultrasound Distance: 21  
SharpIR distance long: 21  
30cm:  
Ultrasound Distance: 31  
SharpIR distance long: 27  
40cm:  
Ultrasound Distance: 42  
SharpIR distance long: 37  
50cm:  
Ultrasound Distance: 52  
SharpIR distance long: 46  
60cm:  
Ultrasound Distance: 62  
SharpIR distance long: 56  
70cm:  
Ultrasound Distance: 73  
SharpIR distance long: 65  
80cm:  
Ultrasound Distance: 83  
SharpIR distance long: 75  
90cm:  
Ultrasound Distance: 92  
SharpIR distance long: 77  
100cm:  
Ultrasound Distance: 103  
SharpIR distance long: 91  
110cm:  
Ultrasound Distance: 113
SharpIR distance long: 92
