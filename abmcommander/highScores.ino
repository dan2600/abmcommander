//Game Over!

bool changed = false;
bool inputName = false;
char nameInput[3] = "   ";

bool scoreChecker(unsigned long scored) {
  if (changed)
  {
    return false;
  }
  if (scored < mcgame.score)
  {
    changed = true;
    inputName = true;
    return true;
  }
  return false;
}

void gameOverScreen() {
  int place = 0;
  tunes.stopScore();
  while (true) {
    if (scoreChecker(highscores.scoreOne))
    {
      highscores.scoreThree = highscores.scoreTwo;
      highscores.timeThree = highscores.timeTwo;
      strcpy(highscores.nameThree, highscores.nameTwo);
      highscores.scoreTwo = highscores.scoreOne;
      highscores.timeTwo = highscores.timeOne;
      strcpy(highscores.nameTwo, highscores.nameOne);
      highscores.scoreOne = mcgame.score;
      highscores.timeOne = mcgame.timeAlive;
      place = 1;
      break;
    }
    if (scoreChecker(highscores.scoreTwo))
    {
      highscores.scoreThree = highscores.scoreTwo;
      highscores.timeThree = highscores.timeTwo;
      strcpy(highscores.nameThree, highscores.nameTwo);
      highscores.scoreTwo = mcgame.score;
      highscores.timeTwo = mcgame.timeAlive;
      place = 2;
      break;
    }
    if (scoreChecker(highscores.scoreThree))
    {
      highscores.scoreThree = mcgame.score;
      highscores.timeThree = mcgame.timeAlive;
      place = 3;
      break;
    }
    break;
  }

  arduboy.clear();
  tunes.stopScore();
  arduboy.drawCompressed(0, 0, nukeover);
  arduboy.display();
  arduboy.invert(true);
  for (int x = 0; x < 500; x++)
  {
    if(x % 10)
    {
       arduboy.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);
    }
    else
    {
      arduboy.digitalWriteRGB(RGB_ON,RGB_ON,RGB_ON);
       
    }
    tunes.tone(random(135, 175), 4);
    delay(4);
  }
  arduboy.invert(false);
  delay(500);

  arduboy.setTextBackground(BLACK);
  arduboy.setCursor(10, 5);
  arduboy.setTextSize(2);
  arduboy.print("GAME OVER");
  arduboy.setTextSize(1);
  arduboy.setCursor(2, 40);
  arduboy.print("Score: ");
  arduboy.print(mcgame.score);
  arduboy.setCursor(2, 50);
  arduboy.print("Time Alive: ");
  arduboy.print(mcgame.timeAlive / 1000 / 60);
  arduboy.print(":");
  if (mcgame.timeAlive / 1000 % 60 < 10) arduboy.print("0");
  arduboy.print(mcgame.timeAlive / 1000 % 60);
  arduboy.display();
  while (true) {
    arduboy.pollButtons();
    if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
      break;
    }
  }
  if (place == 0) {
    state = gameState::highscore;
    return;
  }
  else
  {
    bool tester = true;
    byte cursLoc = 0;
    nameInput[cursLoc] = 65;
    while (tester) {
      arduboy.clear();
      switch (place) {
        case 0:
          arduboy.setCursor(2, 20);
          arduboy.print("ERROR");
          break;
        case 1:
          arduboy.setCursor(12, 20);
          arduboy.print("1ST PLACE");
          break;
        case 2:
          arduboy.setCursor(31, 20);
          arduboy.print("2ND PLACE");
          break;
        case 3:
          arduboy.setCursor(31, 20);
          arduboy.print("3RD PLACE");
          break;
      }
      arduboy.setCursor(19, 56);
      arduboy.print("INPUT INTIALS");
      arduboy.setTextSize(2);
      arduboy.setCursor(39, 34);
      arduboy.print(nameInput[0]);
      arduboy.setCursor(55, 34);
      arduboy.print(nameInput[1]);
      arduboy.setCursor(71, 34);
      arduboy.print(nameInput[2]);
      arduboy.setTextSize(1);
      switch (cursLoc) {
        case 0:
          arduboy.fillRect(39, 50, 10, 2, WHITE);
          break;
        case 1:
          arduboy.fillRect(55, 50, 10, 2, WHITE);
          break;
        case 2:
          arduboy.fillRect(71, 50, 10, 2, WHITE);
          break;
      }
      arduboy.pollButtons();
      if (arduboy.justPressed(LEFT_BUTTON))
      {
        if (cursLoc == 0) cursLoc = 3;
        cursLoc--;
      }
      if (arduboy.justPressed(RIGHT_BUTTON))
      {

        if (cursLoc == 2) cursLoc = 0;
        else cursLoc++;
      }
      if (arduboy.justPressed(A_BUTTON))
      {
        nameInput[cursLoc] = 32;
        if (cursLoc > 0) --cursLoc;
      }
      if (arduboy.justPressed(B_BUTTON))
      {
        if (cursLoc == 2) {
          switch (place) {
            case 1:
              strcpy(highscores.nameOne, nameInput);
              highscores.scoreOne = mcgame.score;
              break;
            case 2:
              strcpy(highscores.nameTwo, nameInput);
              highscores.scoreTwo = mcgame.score;
              break;
            case 3:
              strcpy(highscores.nameThree, nameInput);
              highscores.scoreThree = mcgame.score;
              break;
          }
          EEPROM.put(EEPROM_SCORE, highscores);
          //flashScreen(5, 100);
          changed = false;
          inputName = false;
          nameInput[0] = 32;
          nameInput[1] = 32;
          nameInput[2] = 32;
          delay(200);
          state = gameState::highscore;
          tester = false;
        }
        else {
          ++cursLoc;
          nameInput[cursLoc] = 65;
        }
      }
      if (arduboy.pressed(UP_BUTTON))
      {
        switch (nameInput[cursLoc])
        {
          case 32:
            nameInput[cursLoc] = 65;
            break;
          case 90:
            nameInput[cursLoc] = 32;
            break;
          default:
            ++nameInput[cursLoc];
        }
        delay(100);
      }
      if (arduboy.pressed(DOWN_BUTTON))
      {
        switch (nameInput[cursLoc])
        {
          case 32:
            nameInput[cursLoc] = 90;
            break;
          case 65:
            nameInput[cursLoc] = 32;
            break;
          default:
            --nameInput[cursLoc];
        }
        delay(100);
      }
      arduboy.display();
    }
    // tunes.stopScore();
  }
}









//High Score Screen


void drawScore(unsigned long timeAlive, unsigned long score, byte y1, byte y2) {
  arduboy.setCursor(39, y1);
  arduboy.print("Score ");
  arduboy.print(score);
  arduboy.setCursor(39, y2);
  arduboy.print("Time Alive ");
  arduboy.print(timeAlive / 1000 / 60);
  arduboy.print(":");
  if (timeAlive / 1000 % 60 < 10) arduboy.print("0");
  arduboy.print(timeAlive / 1000 % 60);
}

void highScores() {
  arduboy.pollButtons();
  arduboy.clear();
  arduboy.setCursor(49, 1);
  arduboy.print("HIGH SCORES");

  arduboy.setTextSize(2);
  for (byte z = 0; z < 3; z++) {
    arduboy.setCursor(0 + (12 * z), 11);
    arduboy.print(highscores.nameOne[z]);
    arduboy.setCursor(0 + (12 * z), 29);
    arduboy.print(highscores.nameTwo[z]);
    arduboy.setCursor(0 + (12 * z), 48);
    arduboy.print(highscores.nameThree[z]);
  }
  arduboy.setTextSize(1);
  drawScore(highscores.timeOne, highscores.scoreOne, 11, 19);
  drawScore(highscores.timeTwo, highscores.scoreTwo, 29, 37);
  drawScore(highscores.timeThree, highscores.scoreThree, 48, 56);
  arduboy.display();
  if (arduboy.justPressed(B_BUTTON) || arduboy.justPressed(A_BUTTON)) {
    state = 0;
    mcgame.score = 0;
    warheads = 800;
    maxAmmo = MAX_AMMO;
    ammo = MAX_AMMO;
    bombSpeed = -1;
    for (int z = 0; z < CITY_COUNT; z++)
    {
      cities[z].exploding = false;
      cities[z].destroyed = false;
      cities[z].frame = 0;
    }
    for (int b = 0; b < MAX_BOMBS; b++)
    {
      bombs[b].active = false;
    }
  }
}
