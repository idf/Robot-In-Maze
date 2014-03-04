double driftToTarget(int isLeftForward, int isRightForward, double target_tick) {
  resetPololuTicks(); // since we only cares about the difference, the starting point better set to 0 to avoid overflow
  double avgTicksForAngleOrDist = 0;
  long firstLeftCount = leftCnt;//leftCnt();
  long firstRightCount = rightCnt;//rightCnt();
  
  while (target_tick - avgTicksForAngleOrDist > Config::MIN_SPEED*2) { //target_tick - change to 'angle' for other formula // 200 is tested
  // the tolerance value affect the turning errors

    double leftTicksForAngleOrDist = leftCnt;
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = rightCnt;
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); 

    avgTicksForAngleOrDist = (leftTicksForAngleOrDist - rightTicksForAngleOrDist) / 2; // both forward
    configureMotor(isLeftForward, isRightForward);

    /* IMPORTANT */
    //if(isLeftForward*isRightForward>0)
      delay(Config::MIN_SPEED*0.5); // long distance problem
  }
  // fading
  setScale(0.35); // small, increase accuracy, too small, cannot move (torque)
  //Serial.println(F("fading"));
  while (target_tick - avgTicksForAngleOrDist > 0) { // tolerance
  // the tolerance value affect the turning errors

    double leftTicksForAngleOrDist = leftCnt;
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = rightCnt;
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); 
    

    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2; // turn right
    configureMotor(isLeftForward, isRightForward);
  }
  setScale(1/0.35);
  Serial.print(F("Ticks statistics: ")); Serial.print(avgTicksForAngleOrDist); Serial.print(F(" / ")); Serial.println(target_tick);
  motorShield.setBrakes(Config::DESIGNED_MAX_SPEED, Config::DESIGNED_MAX_SPEED);
  delay(300);
  return avgTicksForAngleOrDist;
}
