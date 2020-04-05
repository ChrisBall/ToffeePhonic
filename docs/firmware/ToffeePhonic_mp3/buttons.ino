//this function manages 4-state buttons (pressed, held, released, off)
int scrollTime = 200;
long unsigned int heldTimer = 0;
long unsigned int menuTimer = 0;

void buttonUpdate() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    boolean buttonState = digitalRead(buttonPins[i]);
    if (buttonStates[i] == 3) {
      buttonStates[i] = 2;
    }
    if ((buttonState == LOW) && (buttonStates[i] == 0)) {
      buttonStates[i] = 3;
      heldTimer = millis();
      menuTimer = millis();
      bigMenu = false;
    }
    if (buttonStates[i] == 1) {
      buttonStates[i] = 0;
    }
    if ((buttonState == HIGH) && (buttonStates[i] == 2)) {
      buttonStates[i] = 1;
    }
  }
  //0-off
  //3-momentary on
  //2-on
  //1-momentary off
}

//this function acts upon button presses
void buttonActions() {
  if (buttonStates[UP] == 3) {
    activeMenuInput--;
  }

  if (buttonStates[DOWN] == 3) {
    activeMenuInput++;
  }

  if (buttonStates[LEFT] == 3) {
    switch (activeMenuInput) {
      case 0:
        sm.fundamental = constrain(sm.fundamental - 1, 0, 127);
        break;
      case 1:
        scaleRange = constrain(scaleRange - 1, 1, 36);
        break;
      case 2:
        minRange = constrain(minRange - 10, minRangeLimit, maxRange - 10);
        break;
      case 3:
        maxRange = constrain(maxRange - 10, minRange + 10, maxRangeLimit);
        break;
      case 4:
        instrument = constrain(instrument - 1, 0, 127);
        midiSetInstrument(0, instrument);
        break;
      case 5:
        midiVolume--;
        if (volume > 127) {
          volume = 127;
        }
        midiSetChannelVolume(0, volume);
        break;
    }
  }

  if (buttonStates[CENTRE] == 3) {
    switch (activeMenuInput) {
      case 0:
        sm.currentScale = (sm.currentScale + 1) % 6;
        break;
      case 4:
        midiBank = (midiBank + 1) % 2;
        midiSetInstrument(0, instrument);
        midiSetInstrument(0, instrument);
        break;
    }
  }

  if (buttonStates[RIGHT] == 3) {
    switch (activeMenuInput) {
      case 0:
        sm.fundamental=constrain(sm.fundamental + 1, 0, 127);
        break;
      case 1:
        scaleRange = constrain(scaleRange + 1, 1, 36);
        break;
      case 2:
        minRange = constrain(minRange + 10, minRangeLimit, maxRange - 10);
        break;
      case 3:
        maxRange = constrain(maxRange + 10, minRange + 10, maxRangeLimit);
        break;
      case 4:
        instrument = constrain(instrument + 1, 0, 127);
        break;
      case 5:
        volume++;
        if (volume > 254) {
          volume = 0;
        }
        break;
    }
  }

  if (activeMenuInput < 0) {
    activeMenuInput = menuItems - 1; //menu looping
  } else if (activeMenuInput > (menuItems - 1)) {
    activeMenuInput = 0;
  }

  for (int i = 0; i < NUM_BUTTONS; i++) {
    if ((buttonStates[i] == 2) && ((millis() - heldTimer) > scrollTime)) { //if a button has been held for enough time
      buttonStates[i] = 0;
      updateDisplayFlag = true;
      //saveState();
      return;
    } else if (buttonStates[i] == 1) { //if a button has been released
      updateDisplayFlag = true;
      return;
    }
  }

  if ((millis() - menuTimer) > 10000 && bigMenu == false) {
    bigMenu = true;
    updateDisplayFlag = true;
  }
}
