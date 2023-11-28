int illuminationAge = 0;
double illumination = -1;

void illuminationSetup() {
#ifdef ILLUMINATION_PIN
  Serial.println(String("Illumination pin ") + ILLUMINATION_PIN);
  int raw = analogRead(ILLUMINATION_PIN);
  printIllumination(raw, mapIllumination(raw), 0);
  
#endif  
}

void checkIllumination() {
#ifdef ILLUMINATION_PIN
  illuminationMeasuredAfterDimmerAdjustment = true;
  illuminationAge++;
  int raw = analogRead(ILLUMINATION_PIN);
  double newIllumination = mapIllumination(raw);    
  double diff = fabs(newIllumination - illumination);
  if (diff > (double)0.05 || illuminationAge >= MEASUREMENT_MAX_AGE) {
    printIllumination(raw, newIllumination, illumination);    
    illumination = newIllumination;
    illuminationAge = 0;
    sendMeasurement("illumination", illumination);
  }
#endif
}

void printIllumination(int raw, double illumination, double previous) {
  double diff = fabs(illumination - previous);
  Serial.println(String("Illumination ") + illumination + " / raw "+ raw + " (prev" + previous + ", diff " + diff + ")");    
}

double mapIllumination(int raw) {
  // Map to logarithmic scale between 0 and 1  
  return log10(((double)raw) * (double)9 / (double)1024 + 1);    
}
