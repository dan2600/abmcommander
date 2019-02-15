#include <Arduino.h>
#define MAX_MISSLE 5
#define MAX_BOMBS 10
#define CITY_COUNT 6
#define MAX_AMMO 25.0


enum gameState {
  title,
  gameplay,
  gameover,
  highscore,
  options,
};

struct THEHIGHSCORES {
  char nameOne[3];
  unsigned long scoreOne;
  unsigned long timeOne;
  char nameTwo[3];
  unsigned long scoreTwo;
  unsigned long timeTwo;
  char nameThree[3];
  unsigned long scoreThree;
  unsigned long timeThree;
};



struct MCGame {
  gameState state;
  unsigned long score;
  unsigned long timeAlive;
  byte lives;
  byte level;
};

struct Gunner {
  byte rounds;
  float theta;
};

struct AMissle {
  int targetx;
  int targety;

  int distance;
  float theta;
  bool active;
  bool exploding;
  byte frame;
};

struct ACity {
  const int x;
  const int y;
  bool exploding;
  bool destroyed;
  byte frame;
};

struct ABomb {
  int orgx;
  int yOffset;
  int targetx;
  int distance;
  float theta;
  bool active;
};

struct TheJet {
  int x;
  bool active;
};

struct Crosshair {
  int x;
  int y;
};
