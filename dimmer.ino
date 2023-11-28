int brightness = 0;
int targetBrightness = 0;

#define EEPROM_ADDRESS 400

void dimmerSetup() {
#ifdef DIMMER_LIGHT_PIN
  Serial.println(String("Dimmer pin ") + DIMMER_LIGHT_PIN);
  ledcSetup(DIMMER_PWM, 1000, 8);
  ledcAttachPin(DIMMER_LIGHT_PIN, DIMMER_PWM);
  EEPROM.begin(512);  
  EEPROM.get(EEPROM_ADDRESS, targetBrightness);
  if (targetBrightness >= 0 && targetBrightness <= 255) {
    Serial.println(String("Read targetBrightness from EEPROM: ") + targetBrightness);
  } else {
    targetBrightness = 0;
  }
  setBrightness(brightness);    
#endif
}

void setTargetBrightness(int t) {
  targetBrightness = t;
  Serial.println(String("New target brightness ") + targetBrightness);
  EEPROM.put(EEPROM_ADDRESS, targetBrightness);
  EEPROM.commit();
}

void fadeToTarget() {
  int diff = targetBrightness - brightness;
  if (diff != 0) {
    int distance = abs(diff);
    int dir = diff / distance;
    int increment = distance >= 4 ? dir * 4 : diff;
    brightness += increment;
    setBrightness(brightness);      
    illuminationMeasuredAfterDimmerAdjustment = false;
  }
}

// Range: 0-255
void setBrightness(int b) {
#ifdef DIMMER_LIGHT_PIN  
  ledcWrite(DIMMER_PWM, b);
#endif
}
