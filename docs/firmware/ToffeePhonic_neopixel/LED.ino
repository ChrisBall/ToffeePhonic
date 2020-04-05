void setupLED(){
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void setLedColor(int COLORNUM){
  CRGB color;
  if(COLORNUM<0){
    color=CRGB(0,0,0);
  }else{
    color=CHSV((COLORNUM%12)*149%256,255,255);
  }

  for(int i=0;i<NUM_LEDS;i++){
    leds[i]=color;
  }
  FastLED.show();
}
