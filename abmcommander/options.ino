//options screen

void optionsScreen() {
  bool optionSelect = true;
  while (true) {
    arduboy.clear();
    arduboy.setCursor(0, 0);
    arduboy.print("Options");
    arduboy.setCursor(0, 10);
    if (optionSelect) {
      arduboy.print(">");
    }
    else {
      arduboy.print(" ");
    }
    arduboy.print("Sound ");
    if (arduboy.audio.enabled ()) {
      arduboy.print("On");
    }
    else {
      arduboy.print("Off");
    }
    arduboy.setCursor(0, 20);
    if (!optionSelect) {
      arduboy.print(">");
    }
    else {
      arduboy.print(" ");
    }
    arduboy.print("Reset Scores");
    arduboy.setCursor(8, 40);
    arduboy.print("A Select  B Return");
    arduboy.display();
    arduboy.pollButtons();
    if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {
      optionSelect = !optionSelect;
    }
    if (arduboy.justPressed(B_BUTTON)) {
      if (optionSelect) {
        arduboy.audio.toggle();
      }
      else {
        resetScore();
        state = 0;
        EEPROM.get(EEPROM_SCORE, highscores);
        break;
      }
    }

    if (arduboy.justPressed(A_BUTTON)) {
      state = 0;
      arduboy.audio.saveOnOff();
      break;
    }
  }
}
