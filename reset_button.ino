void resetButtonSetup() {
  #ifdef RESET_BUTTON_PIN
    pinMode(RESET_BUTTON_PIN, INPUT);
    pinMode(RESET_SIGNAL_PIN, OUTPUT);
    digitalWrite(RESET_SIGNAL_PIN, HIGH);
  #endif  
}

void checkResetButton() {
  #ifdef RESET_BUTTON_PIN
    if (digitalRead(RESET_BUTTON_PIN) == RESET_BUTTON_STATE) {
      while (digitalRead(RESET_BUTTON_PIN) == RESET_BUTTON_STATE) {
        coolLedEffect();
        delay(100);
      }
      Serial.println("Reset button pressed. Reseting!");
      resetDevice();
    }
  #endif
}

void resetDevice() {
  Serial.println("Reseting saved settings");
  resetCredentials();
  saveCredentials();
  delay(1000);
  Serial.println("Restarting device...");
  digitalWrite(RESET_SIGNAL_PIN, LOW);
  while(true) {
    delay(1000);
  }
}
