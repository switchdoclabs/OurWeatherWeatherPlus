//
// Supports the SwitchDoc Labs WXLink Device
//

#define RXDEBUG


void resetWXLink()
{
  // assumes that WXLink Reset Pin is connected to WeatherPlus GPIO Pin #13 (also used with lightning detector)
  //

  Serial.println("Reseting WXLink");

  digitalWrite(13, 0);
  pinMode(13, OUTPUT);
  delay(100);

  pinMode(13, INPUT);
  digitalWrite(13, 1);


}


byte buffer[75];


void printBuffer(byte *buffer, int buflen)
{
  int i;
  for (i = 0; i < buflen; i++)
  {
#ifdef RXDEBUG
    Serial.print("i=");
    Serial.print(i);
    Serial.print(" | ");
    Serial.println(buffer[i], HEX);
#endif

  }

}


int convert2BytesToInt(byte *buffer, int bufferStart)
{

  union u_tag {
    byte b[2];
    int fval;
  } u;

  u.b[0] = buffer[bufferStart];
  u.b[1] = buffer[bufferStart + 1];


  return u.fval;

}

long convert4BytesToLong(byte *buffer, int bufferStart)
{

  union u_tag {
    byte b[4];
    long fval;
  } u;

  u.b[0] = buffer[bufferStart];
  u.b[1] = buffer[bufferStart + 1];
  u.b[2] = buffer[bufferStart + 2];
  u.b[3] = buffer[bufferStart + 3];

  return u.fval;

}

float convert4BytesToFloat(byte *buffer, int bufferStart)
{
  union u_tag {
    byte b[4];
    float fval;
  } u;

  u.b[0] = buffer[bufferStart];
  u.b[1] = buffer[bufferStart + 1];
  u.b[2] = buffer[bufferStart + 2];
  u.b[3] = buffer[bufferStart + 3];

  return u.fval;


}

int interpretBuffer(byte *buffer, int buflen)
{
  if (!((buffer[0] == 0xAB) && (buffer[1] == 0x66)))
  {
    // start bytes are not in buffer - reject
    return 1; // no start bytes
  }
#ifdef RXDEBUG
  Serial.println("Start Bytes Found");
#endif
  if (buflen != 64)
  {
    return 2; // buflen wrong
  }
  unsigned short checksumValue;

  // calculate checksum
  checksumValue = crc.XModemCrc(buffer, 0, 59);
#ifdef RXDEBUG
  Serial.print("crc = 0x");
  Serial.println(checksumValue, HEX);

  Serial.print("receivedChecksum=");
  Serial.print(buffer[61], HEX);
  Serial.println(buffer[62], HEX);
#endif

  if ((checksumValue >> 8) != buffer[61])
  {
    // bad checksum
    return 3;  // bad checksum

  }
  if ((checksumValue & 0xFF) != buffer[62])
  {
    // bad checksum
    return 3;  // bad checksum

  }




  //

#ifdef RXDEBUG

  Serial.println("Correct Buffer Length");

  Serial.print("Protocol=");
  Serial.println(buffer[2]);

  Serial.print("TimeSinceReboot(msec)=");
  Serial.println(convert4BytesToLong(buffer, 3));

  Serial.print("Wind Direction=");
  Serial.println(convert2BytesToInt(buffer, 7));

  Serial.print("Average Wind Speed (KPH)=");
  Serial.println(convert4BytesToFloat(buffer, 9));

  Serial.print("Wind Clicks=");
  Serial.println(convert4BytesToLong(buffer, 13));

  Serial.print("Total Rain Clicks=");
  Serial.println(convert4BytesToLong(buffer, 17));

  Serial.print("Max Wind Gust=");
  Serial.println(convert4BytesToFloat(buffer, 21));



  Serial.print("Outside Temperature=");
  Serial.println(convert4BytesToFloat(buffer, 25));

  Serial.print("OT Hex=");
  Serial.print(buffer[25], HEX);
  Serial.print(buffer[26], HEX);
  Serial.print(buffer[27], HEX);
  Serial.println(buffer[28], HEX);

  Serial.print("Outside Humidity=");
  Serial.println(convert4BytesToFloat(buffer, 29));

  Serial.print("BatteryVoltage=");
  Serial.println(convert4BytesToFloat(buffer, 33));
  Serial.print("BatteryCurrent=");
  Serial.println(convert4BytesToFloat(buffer, 37));
  Serial.print("LoadCurrent=");
  Serial.println(convert4BytesToFloat(buffer, 41));
  Serial.print("SolarPanelVoltage=");
  Serial.println(convert4BytesToFloat(buffer, 45));
  Serial.print("SolarPanelCurrent=");
  Serial.println(convert4BytesToFloat(buffer, 49));

  Serial.print("AuxA=");
  Serial.println(convert4BytesToFloat(buffer, 53));

  Serial.print("Message ID=");
  Serial.println(convert4BytesToLong(buffer, 57));


  Serial.print("Checksum High=0x");
  Serial.println(buffer[61], HEX);
  Serial.print("Checksum Low=0x");
  Serial.println(buffer[62], HEX);
#endif

  return 0;

}

void clearBufferArray(int buflen)              // function to clear buffer array
{
  for (int i = 0; i < buflen; i++)
  {
    buffer[i] = NULL; // clear all index of array with command NULL
  }
}


bool readWXLink()
{

  Wire.setClock(100000L);
  // only set variables if we read it correctly
  // request block 0




  Wire.beginTransmission(0x08);
  delay(10);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.setClockStretchLimit(1500);    // in µs

  int blockcount =   Wire.requestFrom(0x08, 32, true);
  Serial.print("Block Count Recieved=");
  Serial.println(blockcount);


  int bufferCount;
  bufferCount = 0;
  while (Wire.available())
  {

    buffer[bufferCount] = Wire.read();
    bufferCount ++;
    delay(10);


  }

  // Now request Block 1
  Wire.beginTransmission(0x08);
  delay(10);
  Wire.write(0x01);
  Wire.endTransmission();

  Wire.setClockStretchLimit(1500);    // in µs

  blockcount =   Wire.requestFrom(0x08, 32, true);
  Serial.print("Block Count Recieved=");
  Serial.println(blockcount);

  while (Wire.available())
  {

    buffer[bufferCount] = Wire.read();
    bufferCount ++;

    delay(10);


  }

#ifdef RXDEBUG
  Serial.print("bufferCount = ");
  Serial.println(bufferCount);
  //  printBuffer(buffer, bufferCount);
#endif


  int badWXLinkReads = 0;

  int interpretResult = interpretBuffer(buffer, bufferCount);


  switch (interpretResult)
  {
    case 0:
      {
        Serial.println("Good Message");
        badWXLinkReads = 0;

        return true;

      }
      break;
    case 1:
      Serial.println("Bad Message - No Start Bytes");
      // only reset on three bad reads in a row

      Serial.print("badWXLinkReads=");
      Serial.println(badWXLinkReads);
      if (badWXLinkReads == 2)
      {
        resetWXLink();
        badWXLinkReads = 0;
      }
      else
      {
        badWXLinkReads++;
      }
      return false;
      break;
    case 2:
      Serial.println("Bad Message - buffer length incorrect");
      return false;
      break;
    case 3:
      Serial.println("Bad Message - Bad Checksum");
      return false;
      break;
    default:

      Serial.print("Bad Message - Unknown Return Code =");
      Serial.println(interpretResult);
      return false;
      break;
  }





  int i;



  bufferCount = 0;
  // digitalWrite(LED, HIGH);
  //delay(100);
  //digitalWrite(LED, LOW);

  return false;


}







