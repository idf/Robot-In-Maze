//
//  Ultrasound.h
//  
//
//  Created by Louis Chia on 24/2/14.
//
//

#ifndef _Ultrasound_h
#define _Ultrasound_h
#include <cstdint>
class Ultrasound
{
    public:
            Ultrasound(int URPWM, int URTRIG);
            void init();
            int dist();
    
private:
    int URPWM;
    int URTRIG;
    //uint8_t EnPwmCmd[4]
    
    
};


#endif
