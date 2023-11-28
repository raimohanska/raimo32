int currentConnStatus;
int connStatusConfirmed = false;

void statusLedSetup() {
  #ifdef GREEN_LED_PIN
    ledcSetup(RED_LED_PWM, 4000, 8);
    ledcSetup(GREEN_LED_PWM, 4000, 8);
    ledcAttachPin(RED_LED_PIN, RED_LED_PWM);
    ledcAttachPin(GREEN_LED_PIN, GREEN_LED_PWM);
    connStatus(CONN_WIFI_FAIL);
  #endif
}

void connStatus(int status) {
  if (status != currentConnStatus) {
    currentConnStatus = status;    
    connStatusConfirmed = false;
  }
#ifdef GREEN_LED_PIN
  Serial.print("Conn status "); Serial.println(status);
  switch(status) {
    case CONN_WIFI_FAIL: 
    case CONN_SERVER_FAIL: 
      statusRed();
      break;
    case CONN_WIFI_CONNECTING: 
    case CONN_SERVER_CONNECTING: 
      statusYellow();
      break;
    case CONN_WIFI_CONNECTED: 
    case CONN_SERVER_CONNECTED: 
      statusGreen();
      break;
  }  
#endif    
}

void fadeoutStatusLed() {
  if (!connStatusConfirmed) {
#ifdef GREEN_LED_PIN    
    ledcWrite(RED_LED_PWM, 0);
    for (int i = 255; i >= 0; i--) {
      ledcWrite(GREEN_LED_PWM, i);
      delay(1);
    }
#endif    
    connStatusConfirmed = true;    
  }
}

void showColor(int red, int green) {
  ledcWrite(RED_LED_PWM, red);
  ledcWrite(GREEN_LED_PWM, green);
}

void statusRed() {
  showColor(255, 0);
}

void statusYellow() {
  showColor(255, 255);
}

void statusGreen() {
  showColor(0, 255);
}

void statusOff() {
  showColor(0, 0);
}
