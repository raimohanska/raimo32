int currentMotionStatus = 0;
int motionCounter = 0;
int countSinceSendMotion = 0;
#define SLEEP_SECS_AFTER_POSITIVE 60

void motionSetup() {
#ifdef MOTION_PIN
  pinMode(MOTION_PIN, INPUT);
  Serial.println(String("Motion pin ") + MOTION_PIN);
#endif
}

void checkMotion() {
#ifdef MOTION_PIN
  int newStatus = digitalRead(MOTION_PIN);
  if (newStatus != currentMotionStatus) {
    //Serial.println(String("New motion status ") + newStatus);
    int secondsToWait = (currentMotionStatus ? SLEEP_SECS_AFTER_POSITIVE : 0);
    if (motionCounter >= secondsToWait) {
      motionCounter = 0;
      countSinceSendMotion = 0;
      currentMotionStatus = newStatus;     
      sendMeasurement("motion", newStatus);
     } else {
      //Serial.println(String("Throttling ") + (secondsToWait - motionCounter));
    }
  } else {
    motionCounter = 0;
  }
  if (motionCounter < 10000) { // overflow prevention
    motionCounter++;
    countSinceSendMotion++;
  }  
#endif
}
