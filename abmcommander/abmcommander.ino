#include <Arduboy2.h>
#include <ArduboyPlaytune.h>
#include "sprites.h"
#include "objects.h"
THEHIGHSCORES highscores;
#include "epp.h"
#define MAX_MISSLE 5
#define MAX_BOMBS 10
#define CITY_COUNT 6
#define MAX_AMMO 30.0
Arduboy2 arduboy;
Sprites sprites;
ArduboyPlaytune tunes (arduboy.audio.enabled);

ABomb bombs[MAX_BOMBS] = { {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
, {0, 0, 0, 0, 0, false}
};
ACity cities[CITY_COUNT] = { {0, 51, false, false, 0}
, {20, 51, false, false, 0}
, {40, 51, false, false, 0}
, {77, 51, false, false, 0}
, {98, 51, false, false, 0}
, {118, 51, false, false, 0}
};
MCGame mcgame = { gameState::title, 0, 0, 3, 1 };
Gunner gunner = { MAX_MISSLE, 0 };
TheJet thejet = { 128, false };
AMissle theMissles[MAX_MISSLE] = { {0, 0, 0, 0, false, false, 0}
, {0, 0, 0, 0, false, false, 0}
, {0, 0, 0, 0, false, false, 0}
, {0, 0, 0, 0, false, false, 0}
, {0, 0, 0, 0, false, false, 0}
};
Crosshair crosshair = { 64, 32 };

int maxAmmo = MAX_AMMO;
int ammo = MAX_AMMO;
int warheads = 800;
gameState state = 0;
bool playMusic = false;
void
setup ()
{
  arduboy.begin ();
  arduboy.setFrameRate (120);
  randomSeed (analogRead (0));
  random (2, 128);
  random (0, 128);
  arduboy.clear ();
  tunes.initChannel (PIN_SPEAKER_1);
  tunes.initChannel (PIN_SPEAKER_2);
}


void
loop ()
{
  initEEPROM ();
  EEPROM.get (EEPROM_SCORE, highscores);
  switch (state)
    {
    case gameState::title:
      titleDisplay ();
      break;
    case gameState::gameplay:
      game ();
      break;
    case gameState::gameover:
      gameOverScreen ();
      break;
    case gameState::highscore:
      highScores ();
      break;
    case gameState::options:
      optionsScreen ();
      break;
    }
}







int bombSpeed = -1;
byte combo = 0;
unsigned long previousReload = 0;
unsigned long previousSpeedup = 0;
unsigned long previousCombo = 0;
unsigned long startTime = 0;
const long reloadInterval = 15000;
const long speedupInterval = 30000;


bool oselect = false;
char wordplay[126] =
  "It's World War 3! No one Knows how it     started. But, the    Nukes are coming and only you can stop    them. You are the....";
byte typed = 0;

void
titleDisplay ()
{
  arduboy.clear ();
  arduboy.setCursor (38, 20);
  arduboy.print ("Dan2600");
  arduboy.setCursor (35, 30);
  arduboy.print ("Presents");
  arduboy.display ();
  delay (2000);
  arduboy.clear ();
  arduboy.setCursor (0, 0);
  arduboy.setTextWrap (true);
  while (typed < 126)
    {
      arduboy.pollButtons ();
      if (arduboy.justPressed (B_BUTTON))
  {
    break;
  }
      arduboy.print (wordplay[typed]);
      typed++;
      if (wordplay[typed] != 32)
  {
    tunes.tone (175, 30);

    arduboy.display ();
    delay (100);
  }
      else
  {
    arduboy.display ();
    delay (50);
  }
    }
  delay (2700);
  arduboy.clear ();
  typed = 0;
  arduboy.setTextWrap (false);

  while (true)
    {
      arduboy.pollButtons ();
      if (arduboy.justPressed (LEFT_BUTTON)
    || arduboy.justPressed (RIGHT_BUTTON))
  oselect = !oselect;

      if (arduboy.justPressed (B_BUTTON))

  {
    if (!oselect)
      {
        state = 1;
        tunes.stopScore ();
        startTime = millis ();
        arduboy.clear ();
        previousSpeedup = millis ();
        thejet.active = false;
        for (byte i = 1; i < 4; i++)
    {
      arduboy.drawCompressed (32, 0, nukes);
      arduboy.display ();
      arduboy.invert (true);
      arduboy.digitalWriteRGB (RGB_ON, RGB_OFF, RGB_OFF);
      for (byte z = 1; z < 255; z++)
        {
          tunes.tone (z + 300, 5);
          delay (5);
        }

      arduboy.invert (false);
      arduboy.digitalWriteRGB (RGB_OFF, RGB_OFF, RGB_OFF);
      delay (150);
    }
        delay (200);

        break;
      }
    else
      {
        state = 4;
        break;
      }
  }
      arduboy.drawCompressed (0, 0, titleScreenv);
      arduboy.setCursor (22, 50);
      if (!oselect)
  arduboy.print (">");
      else
  arduboy.print (" ");
      arduboy.print ("PLAY   ");
      if (oselect)
  arduboy.print (">");
      else
  arduboy.print (" ");
      arduboy.print ("OPTIONS");
      arduboy.display ();
    }
}





void
game ()
{
  bool endGame = true;
  for (int z = 0; z < CITY_COUNT; z++)
    {
      if (!cities[z].destroyed)
  {
    endGame = false;
  }

    }
  if (endGame)
    {
      mcgame.timeAlive = millis () - startTime;
      state = 2;
    }
  unsigned long currentMillis = millis ();

  if (!arduboy.nextFrame ())
    {
      return;
    }
  if (arduboy.everyXFrames (100) && thejet.active && thejet.x > 1)
    {
      dropBomb (thejet.x, 6);
    }
  if (arduboy.everyXFrames (150))
    {
      dropBomb (0, 0);
    }
  if (currentMillis - previousReload >= reloadInterval)
    {
      ammo = maxAmmo;
      previousReload = currentMillis;
    }
  if (currentMillis - previousCombo >= 1200)
    {
      combo = 0;

    }
  if (currentMillis - previousSpeedup >= speedupInterval)
    {

      int citym = 0;
      for (int z = 0; z < CITY_COUNT; z++)
  {
    if (!cities[z].destroyed)
      citym++;
  }


      mcgame.score += 5000 * citym;

      if (bombSpeed < 9)
  {
    bombSpeed++;
  }

      thejet.x = 128;
      thejet.active = true;

      previousSpeedup = currentMillis;
    }

  if (ammo <= 0 && arduboy.everyXFrames (10))
    {
      arduboy.digitalWriteRGB (RGB_ON, RGB_OFF, RGB_OFF);
    }
  else
    {
      arduboy.digitalWriteRGB (RGB_OFF, RGB_OFF, RGB_OFF);
    }

  userInput ();
  updateObjects ();
  cCheck ();
  drawScreen ();
}

void
userInput ()
{
  arduboy.pollButtons ();
  if (arduboy.pressed (UP_BUTTON) && crosshair.y >= 0)
    --crosshair.y;
  if (arduboy.pressed (DOWN_BUTTON) && crosshair.y < 58)
    ++crosshair.y;
  if (arduboy.pressed (LEFT_BUTTON) && crosshair.x >= 0)
    --crosshair.x;
  if (arduboy.pressed (RIGHT_BUTTON) && crosshair.x <= 125)
    ++crosshair.x;
  if (arduboy.justPressed (B_BUTTON))
    {
      if (ammo <= 0)
  return;
      for (int m = 0; m < MAX_MISSLE; m++)
  {
    if (!theMissles[m].active)
      {
        theMissles[m].distance = 0;
        theMissles[m].targetx = crosshair.x - 3;
        theMissles[m].targety = crosshair.y - 3;
        theMissles[m].theta =
    atan ((theMissles[m].targety -
           59.0) / (theMissles[m].targetx - 64.0));
        if (theMissles[m].theta < 0)
    theMissles[m].theta += PI;
        theMissles[m].active = true;
        ammo--;
        break;
      }
  }
    }
}

void
cCheck ()
{
  for (int b = 0; b < MAX_BOMBS; b++)
    {
      if (bombs[b].active)
  {
    Rect bombbox =
      { bombs[b].orgx - bombs[b].distance * cos (bombs[b].theta),
 bombs[b].yOffset + bombs[b].distance * sin (bombs[b].theta), 1, 1 };
    for (int m = 0; m < MAX_MISSLE; m++)
      {
        if (theMissles[m].frame == 3)
    {

      Rect missleBox =
        { theMissles[m].targetx - 3, theMissles[m].targety - 3,
       12, 12 };

      if (m == 0 && thejet.active)
        {
          Rect jetBox = { thejet.x, 0, 9, 6 };
          if (arduboy.collide (jetBox, missleBox))
      {
        arduboy.digitalWriteRGB (RGB_ON, RGB_ON, RGB_ON);
        delayMicroseconds (700);
        arduboy.digitalWriteRGB (RGB_OFF, RGB_OFF, RGB_OFF);
        thejet.active = false;
        combo++;
        mcgame.score += 25000;
      }
        }

      if (arduboy.collide (bombbox, missleBox))
        {
          arduboy.digitalWriteRGB (RGB_ON, RGB_ON, RGB_ON);
          delayMicroseconds (100);
          arduboy.digitalWriteRGB (RGB_OFF, RGB_OFF, RGB_OFF);
          arduboy.invert (true);
          tunes.tone (random (150, 250), 10);
          tunes.tone (random (150, 250), 10);
          delay (50);
          arduboy.invert (false);
          tunes.tone (random (150, 250), 10);
          tunes.tone (random (150, 250), 10);
          delay (50);
          previousCombo = millis ();
          combo++;
          if (combo > 2)
      {
        ammo++;
      }
          mcgame.score += 1000 * combo;
          bombs[b].active = false;
        }
    }
      }
    for (int z = 0; z < CITY_COUNT; z++)
      {
        Rect citybox = { cities[z].x + 2, cities[z].y + 5, 8, 7 };
        if (arduboy.collide (bombbox, citybox) && !cities[z].exploding)
    {
      cities[z].exploding = true;
      arduboy.invert (true);
      arduboy.digitalWriteRGB (RGB_ON, RGB_ON, RGB_ON);
      tunes.tone (random (150, 250), 10);
      delay (50);
      arduboy.invert (false);
      arduboy.digitalWriteRGB (RGB_OFF, RGB_OFF, RGB_OFF);
      tunes.tone (random (150, 250), 10);
      delay (50);
      arduboy.invert (true);
      arduboy.digitalWriteRGB (RGB_ON, RGB_ON, RGB_ON);
      tunes.tone (random (150, 250), 10);
      delay (50);
      arduboy.invert (false);
      arduboy.digitalWriteRGB (RGB_OFF, RGB_OFF, RGB_OFF);
      tunes.tone (random (150, 250), 10);
      delay (5);
      tunes.tone (random (150, 250), 10);
      delay (5);
      tunes.tone (random (150, 250), 10);
      maxAmmo -= 3;
      bombs[b].active = false;

    }
      }
  }
    }
}

void
updateObjects ()
{

  if (thejet.active)
    {
      if (arduboy.everyXFrames (4))
  {
    thejet.x--;
    if (thejet.x < -6)
      {
        thejet.active = false;
        thejet.x = 128;
      }
  }
    }

  for (int m = 0; m < MAX_MISSLE; m++)
    {
      if (theMissles[m].active && !theMissles[m].exploding)
  {
    theMissles[m].distance++;
    if ((int) (64 - theMissles[m].distance * cos (theMissles[m].theta))
        <= theMissles[m].targetx
        && (int) (59 -
      theMissles[m].distance * sin (theMissles[m].theta)) <=
        theMissles[m].targety)
      {
        theMissles[m].exploding = true;
      }
  }


      if (theMissles[m].exploding)
  {
    theMissles[m].frame++;
    if (theMissles[m].frame > 6)
      {
        theMissles[m].active = false;
        theMissles[m].exploding = false;
        theMissles[m].frame = 0;
      }
  }
    }
  for (int z = 0; z < CITY_COUNT; z++)
    {
      if (cities[z].exploding && !cities[z].destroyed)
  {
    cities[z].frame++;
    if (cities[z].frame == 11)
      {
        cities[z].destroyed = true;
      }

  }
    }

  for (int b = 0; b < MAX_BOMBS; b++)
    {
      if (arduboy.everyXFrames (10 - bombSpeed))
  {
    if (bombs[b].active)
      {
        bombs[b].distance++;
      }
  }
      if (bombs[b].distance * sin (bombs[b].theta) > 64
    || bombs[b].orgx - bombs[b].distance * cos (bombs[b].theta) < 0
    || bombs[b].orgx - bombs[b].distance * cos (bombs[b].theta) > 128)
  {
    arduboy.invert (true);
    arduboy.digitalWriteRGB (RGB_ON, RGB_ON, RGB_ON);
    tunes.tone (random (150, 250), 10);
    delay (50);
    arduboy.invert (false);
    arduboy.digitalWriteRGB (RGB_OFF, RGB_OFF, RGB_OFF);
    tunes.tone (random (150, 250), 10);
    delay (50);
    tunes.tone (random (150, 250), 10);
    bombs[b].distance = 0;
    bombs[b].active = false;
  }
    }
}

void
dropBomb (int xVal, int yVal)
{
  for (int b = 0; b < MAX_BOMBS; b++)
    {

      if (!bombs[b].active)
  {
    int targeting;
    //Set X orgin and reset distance
    if (xVal == 0)
      {
        bombs[b].orgx = random (0, 128);
        targeting = random (0, 11);
      }
    else
      {
        bombs[b].orgx = xVal;
        targeting = random (0, 6);
      }
    if (targeting < 6)
      {
        if (!cities[targeting].destroyed)
    {
      bombs[b].targetx = cities[targeting].x + 5;
    }
        else
    {
      for (int x = 0; x < 6; x++)
        {
          if (!cities[x].destroyed)
      {
        bombs[b].targetx = cities[x].x + 5;
        break;
      }

        }
    }
      }
    else
      {
        bombs[b].targetx = random (0, 128);
      }
    bombs[b].distance = 0;
    bombs[b].yOffset = yVal;
    bombs[b].theta =
      atan ((63.0 - bombs[b].yOffset) / (bombs[b].orgx -
                 bombs[b].targetx));
    if (bombs[b].theta < 0)
      bombs[b].theta += PI;
    bombs[b].active = true;
    warheads--;
    break;
  }
    }
}

void
drawScreen ()
{
  arduboy.clear ();
  //Ground
  arduboy.fillRect (0, 63, 128, 2);

  //Missles
  for (int m = 0; m < MAX_MISSLE; m++)
    {
      if (theMissles[m].active && !theMissles[m].exploding)
  {
    tunes.
      tone (random
      (theMissles[m].distance * 5,
       50 + theMissles[m].distance * 5), 10);
    arduboy.fillRect (66 -
          theMissles[m].distance *
          cos (theMissles[m].theta),
          61 -
          theMissles[m].distance *
          sin (theMissles[m].theta), 2, 2);
    //sprites.drawSelfMasked(64 - theMissles[m].distance * cos(theMissles[m].theta), 59 - theMissles[m].distance * sin(theMissles[m].theta), missleS, theMissles[m].frame);
  }
      if (theMissles[m].active && theMissles[m].exploding)
  {
    tunes.tone (random (150, 250), 10);
    sprites.drawSelfMasked (63 -
          theMissles[m].distance *
          cos (theMissles[m].theta),
          57 -
          theMissles[m].distance *
          sin (theMissles[m].theta), missleS,
          theMissles[m].frame);
  }
    }
  //Cities
  for (int z = 0; z < CITY_COUNT; z++)
    {
      if (!cities[z].destroyed)
  sprites.drawSelfMasked (cities[z].x, cities[z].y, aCity,
        cities[z].frame);
    }
  //Bombs
  for (int b = 0; b < MAX_BOMBS; b++)
    {
      if (bombs[b].active)
  {
    arduboy.drawLine (bombs[b].orgx, 0 + bombs[b].yOffset,
          bombs[b].orgx -
          bombs[b].distance * cos (bombs[b].theta),
          bombs[b].yOffset +
          bombs[b].distance * sin (bombs[b].theta), WHITE);
  }
    }
  //Player Base
  arduboy.fillRect (64, 59, 2, 1);
  arduboy.fillRect (61, 60, 8, 3);
  //Crosshare
  sprites.drawSelfMasked (crosshair.x, crosshair.y, ch, 0);
  //Ammobar
  arduboy.fillRect (0, 0, ((float) ammo / MAX_AMMO) * 128, 2);
  arduboy.setCursor (0, 2);
  arduboy.print (mcgame.score);
  //Jet
  if (thejet.active)
    {
      sprites.drawSelfMasked (thejet.x, 0, jet, 0);
      tunes.tone (random (75, 150), 10);
    }
  arduboy.display ();
}
