//takes the range value and works out which MIDI note should be played
int currentOutputScaleNote = -1;

void midiUpdate() {
  outputScaleNote = map(range, minRange, maxRange, 0, scaleRange);

  if ((outputScaleNote >= 0) && (outputScaleNote < scaleRange)) {
    notePlaying = true;
  } else {
    notePlaying = false;
  }

  currentMidiNote = sm.getScaleNote(outputScaleNote);
  if (notePlaying) { //if trigger input is 'on'
    if (currentMidiNote != activeMidiNote) {
      midiNoteOff(0, activeMidiNote, midiVolume); //turn off last note
      midiNoteOn(0, currentMidiNote, midiVolume); //turn on last note
      activeMidiNote = currentMidiNote;
    }
  } else {
    if (activeMidiNote != 128) {
      midiNoteOff(0, activeMidiNote, volume); //turn off last note
      activeMidiNote = 128;
    }
  }
}

void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15) return;
  inst --; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127) return;

  //Serial1.write(MIDI_CHAN_PROGRAM | chan);
  //Serial1.write(inst);
}


void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;

  //Serial1.write(MIDI_CHAN_MSG | chan);
  //Serial1.write(MIDI_CHAN_VOLUME);
  //Serial1.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;

  //Serial1.write(MIDI_CHAN_MSG | chan);
  //Serial1.write((uint8_t)MIDI_CHAN_BANK);
  //Serial1.write(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;

  midiMessage(MIDI_NOTE_ON | chan, n, vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;

  midiMessage(MIDI_NOTE_OFF | chan, n, vel);
}

void midiMessage(uint8_t STATUS, uint8_t PAR1, uint8_t PAR2) {
  //send MIDI to VS1053 synth
  //Serial1.write(STATUS);
  //Serial1.write(PAR1);
  //Serial1.write(PAR2);

  //then to other ports, if enabled
  if (MIDIMODE) {
    if (HAIRLESS) {
      //Hairless Midi
      Serial.write(STATUS);
      Serial.write(PAR1);
      Serial.write(PAR2);
    } else {
      //USB HID midi
      midiEventPacket_t msg = {STATUS >> 4, STATUS, PAR1, PAR2};
      MidiUSB.sendMIDI(msg);
      MidiUSB.flush();
    }
  }
}
