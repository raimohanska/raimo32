int relayState = 0;

void relaySetup() {
#ifdef RELAY_LIGHT_PIN
  pinMode(RELAY_LIGHT_PIN, OUTPUT);
  digitalWrite(RELAY_LIGHT_PIN, relayState);
  Serial.println(String("Relay pin ") + RELAY_LIGHT_PIN);
#endif
}

void setRelayState() {
#ifdef RELAY_LIGHT_PIN  
  Serial.println(String("Set relay state ") + relayState);
  digitalWrite(RELAY_LIGHT_PIN, relayState);
#endif
}

void setRelayTargetState(int targetState) {
    relayState = targetState;
}