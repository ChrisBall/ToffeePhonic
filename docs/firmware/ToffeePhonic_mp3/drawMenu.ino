void drawMenu() {
  u8g2.clearBuffer();         // clear the internal memory
  draw();
  u8g2.sendBuffer();
}

void draw() {
  if (bigMenu) {
    u8g2.setFont(u8g2_font_inr21_mr);
    u8g2.setCursor(1, 30);
    u8g2.print(sm.getFundamentalName());
    u8g2.setCursor(1, 60);
    u8g2.print(sm.getScaleName());
  } else {
    u8g2.setFont(u8g2_font_6x13_tr);

    u8g2.setCursor(1, 10);
    u8g2.print("Scale: ");
    u8g2.print(sm.getFundamentalName());
    u8g2.print(" ");
    u8g2.print(sm.getScaleName());

    u8g2.setCursor(1, 20);
    u8g2.print("Notes: "); u8g2.print(scaleRange);

    u8g2.setCursor(1, 30);
    u8g2.print("Min Dist(cm): "); u8g2.print(minRange / 10);

    u8g2.setCursor(1, 40);
    u8g2.print("Max Dist(cm): "); u8g2.print(maxRange / 10);

    u8g2.setCursor(1, 50);
    u8g2.print("Inst: "); u8g2.print(instrument);

    u8g2.setCursor(1, 60);
    u8g2.print("Volume: "); u8g2.print(midiVolume);

    u8g2.drawFrame(0, activeMenuInput * 10, 127, 11);
  }
}
