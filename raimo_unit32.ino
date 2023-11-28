#include "settings.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

long secs = 0;
long secStart = 0;
int illuminationMeasuredAfterDimmerAdjustment = false;

void setup() {
  EEPROM.begin(512);
  Serial.begin(115200);  
  Serial.println();
  Serial.println("Starting");
  //resetDevice(); // uncomment for resetting wifi settings
  loadCredentials();
  if (!hasWifiCredentials()) {
    webserverSetup();    
  }
  
  motionSetup();
  illuminationSetup();
  resetButtonSetup(); 
  statusLedSetup();
  dimmerSetup();
  relaySetup();
  temperatureHumiditySetup();
  Serial.println("Setup finished");
}

void loop() {
 eachTick();
 if (millis() - secStart > 1000) {
    secStart = millis();
    if (secs == MEASUREMENT_INTERVAL_SECS) {
      //Serial.print("Restarting loop at "); Serial.println(MEASUREMENT_INTERVAL_SECS);
      secs = 0;      
    }          
    eachSecond(secs);
    secs++;
  }
  delay(10);
}

void eachTick() {
  readInput();
  if (ILLUMINATION_FAST && isConnectedToServer()) checkIllumination();
  fadeToTarget();
  webserverLoop();
  setRelayState();
}

void eachSecond(int secs) {
  if (!hasWifiCredentials()) {
    // LED handling done in webserverLoop    
    return;
  } else if (isConnectedToServer()) {    
    checkMotion();
    if (secs == 0 || !illuminationMeasuredAfterDimmerAdjustment) {
      checkIllumination();      
    }
    if (secs == 0) {
      checkTemperatureAndHumidity();
    } else if (secs % PING_INTERVAL_SECS == 0) {
      sendPing();      
    }
    fadeoutStatusLed();
  } else {
    connStatus(CONN_SERVER_FAIL);
    delay(1000);
    connStatus(CONN_SERVER_CONNECTING);
    delay(1000);
    connectToHost();    
  }
}
