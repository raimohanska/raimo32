int temperatureAge = 0;
float temperature = NAN;
float humidity = NAN;
#include "DHT.h"
#ifdef DHT_PIN
DHT dht(DHT_PIN, DHT22);
#endif

void temperatureHumiditySetup() {
  #ifdef DHT_PIN
  Serial.println(String("DHT pin ") + DHT_PIN);
#ifdef DHT_ENABLE_PIN
  pinMode(DHT_ENABLE_PIN, OUTPUT);
  Serial.println(String("DHT enable pin ") + DHT_ENABLE_PIN);
  digitalWrite(DHT_ENABLE_PIN, DHT_ENABLE_LEVEL);
#endif
  dht.begin();    
  delay(2000); // Warm up the DHT sensor
#endif
}

void checkTemperatureAndHumidity() {
  #ifdef DHT_PIN
  temperatureAge++;
  Serial.println("Measuring temperature and humidity");
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.print("Temperature: "); Serial.println(t);
  Serial.print("Humidity: "); Serial.println(h);
  int shouldSend = false;
  if (t != temperature) {
    if (isnan(temperature) || (abs(t - temperature) > CHANGE_THRESHOLD)) {
      shouldSend = true;
      temperature = t;
    }
  }

  if (h != humidity) {
    if (isnan(humidity) || (abs(h - humidity) > CHANGE_THRESHOLD)) {
      shouldSend = true;
      humidity = h;
    }
  }

  int sent = 0;
  if (shouldSend || temperatureAge >= MEASUREMENT_MAX_AGE) {
    temperatureAge = 0;
    if (!isnan(t)) sent += sendMeasurement("temperature", t);
    if (!isnan(h)) sent += sendMeasurement("humidity", h);    
  }
  #ifdef DHT_ENABLE_PIN
    if (isnan(t)) {
      Serial.println("DHT measurement doesn't seem to work. Booting the sensor");
      digitalWrite(DHT_ENABLE_PIN, !DHT_ENABLE_LEVEL);
      delay(2000);
      digitalWrite(DHT_ENABLE_PIN, DHT_ENABLE_LEVEL);
    }
  #endif  
  #endif
}
