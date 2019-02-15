#ifndef EEPROMUTILS_H
#define EEPROMUTILS_H

#define EEPROM_START_C1                 EEPROM_STORAGE_SPACE_START
#define EEPROM_START_C2                 EEPROM_START_C1 + 1
#define EEPROM_SCORE                    EEPROM_START_C1 + 2


/* ----------------------------------------------------------------------------
     Is the EEPROM initialised?

     Looks for the characters 'S' and 'T' in the first two bytes of the EEPROM
     memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
     it resets the settings ..
   ----------------------------------------------------------------------------
*/

void resetScore(){
    strcpy(highscores.nameOne, "BIG");
    strcpy(highscores.nameTwo, "CHU");
    strcpy(highscores.nameThree, "NGS");
    highscores.scoreThree = 0;
    highscores.scoreTwo = 0;
    highscores.scoreOne = 0;
    EEPROM.put(EEPROM_SCORE, highscores);
  }

void initEEPROM() {
  unsigned char c1 = EEPROM.read(EEPROM_START_C1);
  unsigned char c2 = EEPROM.read(EEPROM_START_C2);

  if (c1 != 'M' || c2 != 'C') {
    EEPROM.update(EEPROM_START_C1, 'M');
    EEPROM.update(EEPROM_START_C2, 'C');
  resetScore();
  }

}
#endif
