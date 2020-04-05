void setupSM() {
  sm.setCurrentScale(1); //sets the currently used scale to Major
  sm.setFundamental(60); //sets the fundamental to C5 (this is already the default, but included for clarity)
}

void setupPins() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void setupTOF() {
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  TOFSensor.setTimeout(500);
  if (!TOFSensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  TOFSensor.setDistanceMode(VL53L1X::Medium);
  TOFSensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  TOFSensor.startContinuous(50);

}

void setupMidi() {
  Serial1.begin(31250);
  resetMIDI();
}
