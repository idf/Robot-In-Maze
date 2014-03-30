void softwareProjectLabSqure() {
  pidMgr->restore();

  pidMgr->setScale(1.75);
  moveForward(160);
  pidMgr->restore();

  turnRight(90);

  pidMgr->setScale(1.75);
  moveForward(110);
  pidMgr->restore();
  turnRight(90);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void test_calibeate() {
  calibrator->calibrate(6);
  turnLeft(90);
  for(int i=0; i<16; i++) {moveForward(10); delay(200);}
  turnRight(90);
  delay(200);
  calibrator->calibrate(4);
  turnRight(90);
  delay(200);
  for(int i=0; i<16; i++)  {moveForward(10); delay(200);}
  turnLeft(90);
  delay(200);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void obstacle_checkpoint() {
  turnLeft(90);
  moveForward(10);

  turnLeft(90);
  moveForward(10);
  turnLeft(90);
  moveForward(10);
  turnRight(90);
}
