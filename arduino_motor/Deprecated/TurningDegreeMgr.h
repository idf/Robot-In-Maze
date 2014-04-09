#ifndef TURNINGDEGREEMGR_H
#define TURNINGDEGREEMGR_H
class TurningDegreeMgr {
public:
  TurningDegreeMgr();
  void init();
  void binary_search_ticks();
  double get_ticks_per_deg_left();
  double get_ticks_per_deg_right();
private: 
  double ticks_per_deg_left;
  double ticks_per_deg_right;
};
#endif