void softwareProjectLabSqure() {
  for(int i=0; i<11; i++) {
    moveForward(10);
    printCounts();
  } 

  turnRight(90);
  printCounts();
  for(int i=0; i<6; i++) {
    moveForward(10);
    printCounts();
  }
  turnRight(90);
  printCounts();
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
////////////////////////////////////////////////////////////////////////////////////////////////////
void goStraightlineCheckpoint(){
 if (frontEye->output_reading_ultra()!=-1&&frontEye->output_reading_ultra()<=10){ // get_ulra_reading
  turnRight(90);
  moveForward(20);
  turnLeft(90);
  moveForward(40);
  turnLeft(90);
  moveForward(20);
  turnRight(90);
  //finish
 }
 moveForward(10);
}


void extensionCheckpoint(){
 if (frontEye->output_reading_ultra()!=-1&&frontEye->output_reading_ultra()<=10){ //get_ulra_reading
  turnRight(45);
  moveForward(20);
  turnLeft(90);
  moveForward(40);
  turnLeft(90);
  moveForward(20);
  turnRight(90);
  //finish
 }
 moveForward(10);
}
