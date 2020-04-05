void rangeUpdate() {   //updates the value stored in int range and outputScaleNote
  TOFSensor.read();
  if (TOFSensor.ranging_data.range_status<2) {
    range = TOFSensor.ranging_data.range_mm;
  }else{
    range=0;
  }

  /*
   * 0=RANGE DATA OK
   * 1=HIGH SIGMA (HIGH STANDARD DEVIATION)
   * 2=LOW SIGNAL LEVEL (HIGH NOISE OR HIGH ABSORPTION AT TARGET)
   * 3=TARGET TOO CLOSE
   * 4=TARGET TOO FAR/NOTHING DETECTED
   * 5=HardwareFail
   * 6=RangeValidNoWrapCheckFail
   * 7=WrapTargetFail            
   * 8=ProcessingFail            
   * 9=XtalkSignalFail           
   * 10=SynchronizationInt        
   * 11=RangeValid MergedPulse    
   * 12=TargetPresentLackOfSignal 
   * 13=MinRangeFail              
   * 14=RangeInvalid              
   * 15=None = 255,
   */
}

//interrupt function: counts PWM width in microseconds and saves value in "range"
//void rangePWM() {
//  long unsigned int us = micros();
//  range = (us - startTime)>5000 ? range : us - startTime; //to get rid of erroneous high values
//  startTime = us;
//}
