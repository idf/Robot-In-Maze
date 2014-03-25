#include "TurningDegreeMgr.h"
#include "Config.h"
TurningDegreeMgr::TurningDegreeMgr() {
  // legacy
  this->ticks_per_deg_left = Config::TICKS_PER_DEGREE_LEFT;
  this->ticks_per_deg_right = Config::TICKS_PER_DEGREE;
}
void TurningDegreeMgr::init() {

}

void TurningDegreeMgr::binary_search_ticks() {
  const int UP = 810;
  const int LOWER = 796;
  
  int min_left;
  int max_left;
  int left; 


  int comamnd_int;
  while(true) {
    // left
    for(int i=0; i<4; i++) {
      turnLeft(90); printCounts(); delay(400);
    }
    if(Serial.available()>0) {
      comamnd_int = Serial.parseInt();
      if(comamnd_int==2) {
        ;
      }
      else if (comamnd_int<2) { //less 
        min_left = left;
        left = (min_left+max_left)/2;
      }
      else { // more
        max_left = left;
        left = (min_left+max_left)/2;
      }
    }
    Serial.print(F("  "));
    // right
    
    // TODO
  }
}

double TurningDegreeMgr::get_ticks_per_deg_left() {
  return this->ticks_per_deg_left;
}
double TurningDegreeMgr::get_ticks_per_deg_right() {
  return this->ticks_per_deg_right;
}
