/************************************************************************************************************
 * SharpIR.h - Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK     *
 * Distance sensors                                                                                         *
 * Copyright 2014 Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)                                       *
 * Last update: 07.01.2014                                                                                  *
 ************************************************************************************************************
 
 ************************************************************************************************************
 * This library is free software; you can redistribute it and/or                                            *
 * modify it under the terms of the GNU Lesser General Public                                               *
 * License as published by the Free Software Foundation; either                                             *
 * version 2.1 of the License, or (at your option) any later version.                                       *
 *                                                                                                          *
 * This library is distributed in the hope that it will be useful,                                          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                                           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                                        *
 * Lesser General Public License for more details.                                                          *
 *                                                                                                          *
 * You should have received a copy of the GNU Lesser General Public                                         *
 * License along with this library; if not, write to the Free Software                                      *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA                               *
 ***********************************************************************************************************/


// The Sahrp IR sensors are cheap but somehow unreliable. I've found that when doing continous readings to a
// fix object, the distance given oscilates quite a bit from time to time. For example I had an object at
// 31 cm. The readings from the sensor were mainly steady at the correct distance but eventually the distance
// given dropped down to 25 cm or even 16 cm. That's quite a bit and for some applications it is quite
// unacceptable. I checked the library http://code.google.com/p/gp2y0a21yk-library/ by Jeroen Doggen
// (jeroendoggen@gmail.com) and what the author was doing is to take a bunch of readings and give an average of them

// The present library works similary. It reads a bunch of readings (avg), it checks if the current reading
// differs a lot from the previous one (tolerance) and if it doesn't differ a lot, it takes it into account
// for the mean distance.
// The distance is calculated from a formula extracted from the graphs on the sensors datasheets
// After some tests, I think that a set of 20 to 25 readings is more than enough to get an accurate distance
// Reading 25 times and return a mean distance takes 53 ms. For my application of the sensor is fast enough.
// This library has the formulas to work with the GP2Y0A21Y and the GP2Y0A02YK sensors but exanding it for
// other sensors is easy enough.


#include "Arduino.h"
#include "SharpIR.h"
#include "Servo.h"


SharpIR::SharpIR(int irPin, int avg, int tolerance, int sensorModel) {
  
    _irPin=irPin;
    _avg=avg;
    _tol=tolerance/100;
    _model=sensorModel;
    
    analogReference(DEFAULT);
 
}


// When you initialize the library object on your sketch you have to pass all the above parameters:

// irPin is obviously the pin where the IR sensor is attached
// avg is the number of readings the library does
// tolerance indicates how similar a value has to be from the last value to be taken as valid. It should be a
//    value between 0 and 100, like a %. A value of 93 would mean that one value has to be, at least, 93% to the
//    previous value to be considered as valid.
// sensorModel is a int to differentiate the two sensor models this library currently supports:
//    1080 is the int for the GP2Y0A21Y and 20150 is the int for GP2Y0A02YK. The numbers reflect the
//    distance range they are designed for (in cm)
 



int SharpIR::cm() {
    
    int raw=analogRead(_irPin);
    float voltFromRaw=map(raw, 0, 1023, 0, 5000);
    
    int puntualDistance;
    
    if (_model==1080) {
        
        puntualDistance=27.728*pow(voltFromRaw/1000, -1.2045);
        
    }else if (_model==20150){
    
        puntualDistance=61.573*pow(voltFromRaw/1000, -1.1068);
        
    }
    
    
    return puntualDistance;


}



int SharpIR::distance() {

    _p=0;
    _sum=0;

    
    for (int i=0; i<_avg; i++){
        
        int foo=cm();
        
        if (foo>=(_tol*_previousDistance)){
        
            _previousDistance=foo;
            _sum=_sum+foo;
            _p++;
            
        }
        
        
    }

    
    int accurateDistance=_sum/_p;
    
    return accurateDistance;

}








