/***************************************************
  Heavily modified by SwitchDoc Labs for reliablity
  February 2018 and in 2017


  This is a library for the AM2315 Humidity Pressure & Temp Sensor

  Designed specifically to work with the AM2315 sensor from Adafruit
  ----> https://www.adafruit.com/products/1293

  These displays use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.

  Additional improvements added by Daniel Sandoval/EasternStarGeek
  8 July, 2013.  (Parent Library: Adafruit_AM2315)

  BSD license, all text above must be included in any redistribution
 ****************************************************/

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Wire.h"

#define AM2315_I2CADDR       0x5C
#define AM2315_READREG       0x03

class SDL_ESP8266_HR_AM2315 {
  public:
    SDL_ESP8266_HR_AM2315();

    boolean readData(float *dataArray);

    boolean badReadFound();

    void printStatistics();

    // AM2315 Statistics
    unsigned long goodReads, badReads, highBadReadCount;
    unsigned long badCRCReads;
    unsigned long badSpikeReads;
    unsigned long badMessageReads;
    unsigned long oldDataReads;

    float  highTemp, lowTemp;
    float lastTemp;

    int lastBadRead[10];

    boolean firstGoodRead;


  private:

    int internalReadData(float *dataArray);
    float humidity, temp;

    boolean badReadPresent;

    float lastGoodTemp;

    // check CRC
    uint16_t calcCRC;
    uint16_t sentCRC;



};

