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

