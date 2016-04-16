/***************************************************
  This is a library for the AM2315 Humidity & Temp Sensor

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
extern long AM2315BadCount;
#ifdef DEBUGPRINT
extern long AM2315TotalCount;
extern unsigned char FirstBadReply[10];

#endif

#include "ESG_AM2315.h"
//#include <util/delay.h>

ESG_AM2315::ESG_AM2315() {
}
int delayByCPU(long delaycount);

int I2C_ClearBus();

boolean ESG_AM2315::readData(float *dataArray) {
  uint8_t reply[10];



  //ETS_UART_INTR_DISABLE();
  noInterrupts();
  ETS_INTR_LOCK();

  Wire.begin(5, 4);   // some ESP8266 devices require 5, 4 instead of 4,5

  Wire.setClock(400000L);

  //Wire.beginTransmission(AM2315_I2CADDR);
  //delay(3);
  //Wire.endTransmission();

  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.write(AM2315_READREG);
  //Wire.write(0x00);  // start at address 0x0
  //Wire.write(4);  // request 4 bytes data
  Wire.endTransmission();

  //delay(50);
  delayByCPU(50);



  // for reasons unknown we have to send the data twice :/
  // whats the bug here?
  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.write(AM2315_READREG);
  Wire.write(0x00);  // start at address 0x0
  Wire.write(4);  // request 4 bytes data
  Wire.endTransmission();

  //delay(50);
  delayByCPU(50);

  Wire.requestFrom(AM2315_I2CADDR, 8);
  for (uint8_t i = 0; i < 8; i++) {
    reply[i] = Wire.read();
    //Serial.println(reply[i], HEX);
  }


  interrupts();
  ETS_UART_INTR_ENABLE();
  //ETS_INTR_UNLOCK();

  yield();


  if ((reply[0] == AM2315_READREG) &&
      (reply[1] == 4))  {

    humidity = reply[2];
    humidity *= 256;
    humidity += reply[3];
    humidity /= 10;

    dataArray[0] = humidity;

    temp = reply[4];
    temp *= 256;
    temp += reply[5];
    temp /= 10;

    // leave in C
    //  dataArray[1] = (temp * 1.8)+32;
    dataArray[1] = temp;
#ifdef DEBUGPRINT
    AM2315TotalCount++;
    AM2315BadCount = -1;
#endif
    //Serial.println("End of AM2315 acquire");
    return true;
  }

  else  {

    dataArray[0] = NAN;
    dataArray[1] = NAN;
#ifdef DEBUGPRINT
    if (   AM2315BadCount == -1)
      AM2315BadCount = AM2315TotalCount;
    for (uint8_t i = 0; i < 10; i++)
      FirstBadReply[i] = reply[i];
#endif
    return false;
  }

}

int delayByCPU(long delaycount)
{
  unsigned long startMillis;
  unsigned long endMillis;

#define COUNT 100000



  startMillis = millis();

  long index;
  float test;
  long i;
  long j;

  for (i = 0; i < delaycount * 166; i++)
  {
    //Serial.println("outside loop");
    for (j = 0; j < 1000; j++)
    {
      for (index = 0; index < COUNT; index++)
      {

        test = 30.4 + i;
        test = test / 50.0;


      }
      test = test + j;
    }
  }

  endMillis = millis();



  return int(test);
}






/*********************************************************************/
