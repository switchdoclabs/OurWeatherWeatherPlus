


// EEPROM Preferences


void readEEPROMState();

#define READ_CODE 165

void invalidateEEPROMState()
{

  for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();

}

void writeEEPROMState()
{
  EEPROM.write(0, READ_CODE);
  int i;

  for (i = 1; i < Wssid.length() + 1; i++)
  {
    EEPROM.write(i, Wssid[i - 1]);

  }
  EEPROM.write(i, '\0');

  for (i = 33; i < WPassword.length() + 33; i++)
  {
    EEPROM.write(i, WPassword[i - 33]);


  }
  EEPROM.write(i, '\0');

  EEPROM.write(97, EnglishOrMetric);

  EEPROM.write(98, WeatherDisplayMode);

  for (i = 99; i < stationName.length() + 99; i++)
  {
    EEPROM.write(i, stationName[i - 99]);


  }
  EEPROM.write(i, '\0');

  // now put alititude in

  int tempAltitude;
  tempAltitude = altitude_meters * 10.0;
  Serial.print("tempAltitude>>8=");
  Serial.println(tempAltitude >> 8);

  EEPROM.write(99 + 34 + 1, tempAltitude >> 8);
  Serial.print("tempAltitud& 0xFFe=");
  Serial.println(tempAltitude & 0xFF);

  EEPROM.write(99 + 34 + 2, tempAltitude & 0xFF);


  for (i = 136; i < adminPassword.length() + 136; i++)
  {
    EEPROM.write(i, adminPassword[i - 136]);


  }
  EEPROM.write(i, '\0');


  for (i = 200; i < WeatherUnderground_StationID.length() + 200; i++)
  {
    EEPROM.write(i, WeatherUnderground_StationID[i - 200]);


  }
  EEPROM.write(i, '\0');




  for (i = 215; i < WeatherUnderground_StationKey.length() + 215; i++)
  {
    EEPROM.write(i, WeatherUnderground_StationKey[i - 215]);


  }
  EEPROM.write(i, '\0');

  // Now PubNub

  EEPROM.write(231, pubNubEnabled);



  for (i = 232; i < SDL2PubNubCode.length() + 232; i++)
  {
    EEPROM.write(i, SDL2PubNubCode[i - 232]);


  }
  EEPROM.write(i, '\0');

  for (i = 275; i < SDL2PubNubCode_Sub.length() + 275; i++)
  {
    EEPROM.write(i, SDL2PubNubCode_Sub[i - 275]);


  }
  EEPROM.write(i, '\0');

  // setup AS3935 parameter string  -31 characters assumed

  for (i = 318; i < as3935_Params.length() + 318; i++)
  {
    EEPROM.write(i, as3935_Params[i - 318]);


  }
  EEPROM.write(i, '\0');

  // write out BlynkAuthCode

  for (i = 349; i < BlynkAuthCode.length() + 349; i++)
  {
    EEPROM.write(i, BlynkAuthCode[i - 349]);


  }
  EEPROM.write(i, '\0');


  EEPROM.commit();

  Serial.println("Writing EEPROM");
  Serial.print("Wssid=");
  Serial.println(Wssid);
  Serial.print("Wssid Length =");
  Serial.println(Wssid.length());
  Serial.print("WPassword");
  Serial.println(WPassword);
  Serial.print("WPLength=");
  Serial.println(WPassword.length());

  Serial.print("SSID:");
  Serial.println(Wssid);
  Serial.print("PASS:");
  Serial.println(WPassword);
  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("WeatherDisplayMode:");
  Serial.println(WeatherDisplayMode);
  Serial.print("Station Name:");
  Serial.println(stationName);
  Serial.print("Altitude:");
  Serial.println(altitude_meters);
  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");
  Serial.print("WeatherUnderground Station ID:");
  Serial.println(WeatherUnderground_StationID);
  Serial.print("WeatherUnderground Station KEY:");
  Serial.println(WeatherUnderground_StationKey.substring(0, 2) + "******");

  Serial.print("pubNubEnabled=");
  Serial.println(pubNubEnabled);
  Serial.print("SDL2PubNubCode=");
  Serial.println(SDL2PubNubCode);
  Serial.print("SDL2PubNubCode_Sub=");
  Serial.println(SDL2PubNubCode_Sub);
  Serial.print("as3935_Params=");
  Serial.println(as3935_Params);
  Serial.print("BlynkAuthCode=");
  Serial.println(BlynkAuthCode);

}



void readEEPROMState()
{
  int readCode;
  char myChar;
  readCode = EEPROM.read(0);


  if (readCode == READ_CODE)
  {

    Wssid = "";
    WPassword = "";

    for (int i = 1; i < 33; ++i)
    {
      myChar = EEPROM.read(i);
      if (myChar == 0)
        break;
      Wssid += myChar;
    }


    for (int i = 33; i < 97; ++i)
    {

      myChar = EEPROM.read(i);
      if (myChar == 0)
        break;

      WPassword += char(myChar);
    }
    Serial.print("Wssid=");
    Serial.println(Wssid);
    Serial.print("Wssid Length =");
    Serial.println(Wssid.length());
    Serial.print("WPassword");
    Serial.println(WPassword);
    Serial.print("WPLength=");
    Serial.println(WPassword.length());

    EnglishOrMetric = EEPROM.read(97);

    WeatherDisplayMode = EEPROM.read(98);
    for (int i = 99; i < 99 + 34; i++)
    {
      myChar = EEPROM.read(i);
      if (myChar == 0)
        break;

      stationName += char(myChar);

    }

    stationName = "";
    int i;
    for (i = 99; i < 99 + 34; ++i)
    {
      myChar = EEPROM.read(i);
      if (myChar == 0)
        break;
      stationName += myChar;
    }

    int tempAltitude;
    tempAltitude = EEPROM.read(99 + 34 + 1) * 256;
    tempAltitude = tempAltitude + EEPROM.read(99 + 34 + 2);

    altitude_meters = float(tempAltitude) / 10.0;

    //EEPROM.write(i + 1, tempAltitude >> 8);
    //EEPROM.write(i + 2, tempAltitude && 0xFF);

    adminPassword = "";
    for (i = 136; i < 136 + 34; ++i)
    {
      myChar = EEPROM.read(i);
      if (myChar == 0)
        break;
      adminPassword += myChar;
    }

    WeatherUnderground_StationID = "";
    for (i = 200; i < 200 + 15; ++i)
    {
      myChar = EEPROM.read(i);
      if (myChar == 0)
        break;
      WeatherUnderground_StationID += myChar;
    }

    WeatherUnderground_StationKey = "";
    for (i = 215; i < 215 + 15; ++i)
    {
      myChar = EEPROM.read(i);
      if (myChar == 0)
        break;
      WeatherUnderground_StationKey += myChar;
    }

    pubNubEnabled = EEPROM.read(231);

    if (pubNubEnabled != 1)
    {
      pubNubEnabled = 0;
    }

    SDL2PubNubCode = "";
    if (pubNubEnabled == 1)
    {
      for (i = 232; i < 232 + 43; ++i)
      {
        myChar = EEPROM.read(i);
        if (myChar == 0)
          break;
        SDL2PubNubCode += myChar;
      }
    }
    else
      SDL2PubNubCode = "XX";

    SDL2PubNubCode_Sub = "";
    if (pubNubEnabled == 1)
    {
      for (i = 275; i < 275 + 43; ++i)
      {
        myChar = EEPROM.read(i);
        if (myChar == 0)
          break;
        SDL2PubNubCode_Sub += myChar;
      }
    }
    else
      SDL2PubNubCode_Sub = "XX";

    // now read params list for AS3935
    String tempString;
    tempString = "";
    for (i = 318; i < 318 + 31; i++)
    {
      myChar = EEPROM.read(i);

      if (myChar == 0)
        break;
      tempString += myChar;
    }


    if (tempString.length() > 0)
      as3935_Params = tempString;

    // now read Blynk Authorization Code
    BlynkAuthCode = "";

    for (i = 349; i < 349 + 43; i++)
    {
      myChar = EEPROM.read(i);

      if (myChar == 0)
        break;
      BlynkAuthCode += myChar;
    }

    if (BlynkAuthCode.length() == 0)
      UseBlynk = false;
    else
      UseBlynk = true;






  }
  else
  {
    Serial.println("EEPROM not Initialized");
    Wssid = "XXX";
    WPassword = "XXX";
    EnglishOrMetric = 0;
    WeatherDisplayMode = DISPLAY_WEATHER_LARGE;
    stationName = "";
    altitude_meters = 637.0;  // default to 611

    adminPassword = "admin";
    WeatherUnderground_StationID = "";
    WeatherUnderground_StationKey = "";
    pubNubEnabled = 0;
    SDL2PubNubCode = "XX";
    SDL2PubNubCode_Sub = "XX";
    BlynkAuthCode = "";
    writeEEPROMState();



  }
  Serial.println("Reading EEPROM");
  Serial.println("EEPROM");
  Serial.print("readCode:");
  Serial.println(readCode);
  Serial.print("SSID:");
  Serial.println(Wssid);
  Serial.print("PASS:");
  Serial.println(WPassword);
  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("WeatherDisplayMode:");
  Serial.println(WeatherDisplayMode);
  Serial.print("Station Name:");
  Serial.println(stationName);
  Serial.print("Altitude:");
  Serial.println(altitude_meters);
  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");
  Serial.print("WeatherUnderground Station ID:");
  Serial.println(WeatherUnderground_StationID);
  Serial.print("WeatherUnderground Station KEY:");
  Serial.println(WeatherUnderground_StationKey.substring(0, 2) + "******");
  Serial.print("pubNubEnabled=");
  Serial.println(pubNubEnabled);
  Serial.print("SDL2PubNubCode=");
  Serial.println(SDL2PubNubCode);
  Serial.print("SDL2PubNubCode_Sub=");
  Serial.println(SDL2PubNubCode_Sub);
  Serial.print("as3935_Params=");
  Serial.println(as3935_Params);
  Serial.print("BlynkAuthCode=");
  Serial.println(BlynkAuthCode);
  Serial.print("UseBlynk=");
  Serial.println(UseBlynk);
}


String returnDirectionFromDegrees(int degrees)
{

  if (degrees == 0)
    return "N";
  if (degrees == 22)
    return "NNE";
  if (degrees == 45)
    return "NE";
  if (degrees == 67)
    return "ENE";
  if (degrees == 90)
    return "E";
  if (degrees == 112)
    return "ESE";
  if (degrees == 135)
    return "SE";
  if (degrees == 157)
    return "SSE";
  if (degrees == 180)
    return "S";
  if (degrees == 202)
    return "SSW";
  if (degrees == 225)
    return "SW";
  if (degrees == 247)
    return "WSW";
  if (degrees == 270)
    return "W";
  if (degrees == 292)
    return "WNW";
  if (degrees == 315)
    return "NW";
  if (degrees == 337)
    return "NNW";


  return "XX";  // return previous value if not found

}



void updateAllWeatherVariables()
{



  heapSize = ESP.getFreeHeap();

  if (AM2315_Present)
  {

  AOK = am2315.readData(dataAM2315);
  AM2315_Temperature = dataAM2315[1];
  AM2315_Humidity = dataAM2315[0];
  }

  if (SHT30_Present)
  {
          int sht30_success;
          sht30_success = sht30.get();
          Serial.print("sht30_success=");
          Serial.println(sht30_success);
          if (sht30_success == 0)
          {

            // Now set the old AM2315 variables
            
            AM2315_Temperature = sht30.cTemp;
            AM2315_Humidity = sht30.humidity;
            dewpoint =  AM2315_Temperature - ((100.0 - AM2315_Humidity) / 5.0);
          }
    
  }

  if (BMP180Found)
  {


    /* Display the results (barometric pressure is measure in hPa) */
    //BMP180_Pressure = bmp.readPressure();
    // Put Alitude in Meters
    BMP180_Pressure = bmp.readSealevelPressure(altitude_meters);
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(BMP180_Pressure / 100.0);
    Serial.println(" kPa");



    /* Calculating altitude with reasonable accuracy requires pressure    *
       sea level pressure for your position at the moment the data is
       converted, as well as the ambient temperature in degress
       celcius.  If you don't have these values, a 'generic' value of
       1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA
       in sensors.h), but this isn't ideal and will give variable
       results from one day to the next.
     *                                                                    *
       You can usually find the current SLP value by looking at weather
       websites or from environmental information centers near any major
       airport.
     *                                                                    *
       For example, for Paris, France you can check the current mean
       pressure and sea level at: http://bit.ly/16Au8ol                   */

    /* First we get the current temperature from the BMP085 */
    float temperature;
    temperature = bmp.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");


    BMP180_Temperature = temperature;




    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    float altitude;
    altitude = bmp.readAltitude(BMP180_Pressure);
    Serial.print("Altitude:    ");
    Serial.print(altitude);

    BMP180_Altitude = altitude;
    Serial.println(" m");

  }

  if (BMP280Found)
  {

    /* Display the results (barometric pressure is measure in hPa) */
    //BMP180_Pressure = bmp.readPressure();
    // Put Alitude in Meters

    BMP180_Pressure = bme.readSealevelPressure(altitude_meters);

    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(BMP180_Pressure / 100.0);
    Serial.println(" hPa");



    /* Calculating altitude with reasonable accuracy requires pressure    *
       sea level pressure for your position at the moment the data is
       converted, as well as the ambient temperature in degress
       celcius.  If you don't have these values, a 'generic' value of
       1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA
       in sensors.h), but this isn't ideal and will give variable
       results from one day to the next.
     *                                                                    *
       You can usually find the current SLP value by looking at weather
       websites or from environmental information centers near any major
       airport.
     *                                                                    *
       For example, for Paris, France you can check the current mean
       pressure and sea level at: http://bit.ly/16Au8ol                   */

    /* First we get the current temperature from the BMP085 */
    float temperature;
    temperature = bme.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");


    BMP180_Temperature = temperature;




    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    float altitude;
    altitude = bme.readAltitude(SENSORS_PRESSURE_SEALEVELHPA);
    Serial.print("Altitude:    ");
    Serial.print(altitude);

    BMP180_Altitude = altitude;
    Serial.println(" m");

  }



  currentWindSpeed = weatherStation.current_wind_speed();
  currentWindGust = weatherStation.get_wind_gust();

  currentWindDirection = weatherStation.current_wind_direction();

  float oldRain = rainTotal;
  rainTotal = rainTotal + weatherStation.get_current_rain_total();



  windSpeedGraph.add_value(currentWindSpeed);
  windGustGraph.add_value(currentWindGust);
  windDirectionGraph.add_value(currentWindDirection);

  windSpeedGraph.getRasPiString(windSpeedBuffer, windSpeedBuffer);
  windGustGraph.getRasPiString(windGustBuffer, windGustBuffer);
  windDirectionGraph.getRasPiString(windDirectionBuffer, windDirectionBuffer);

  windSpeedMin = windSpeedGraph.returnMinValue();
  windSpeedMax = windSpeedGraph.returnMaxValue();
  windGustMin = windGustGraph.returnMinValue();
  windGustMax = windGustGraph.returnMaxValue();
  windDirectionMin = windDirectionGraph.returnMinValue();
  windDirectionMax = windDirectionGraph.returnMaxValue();



  // Handle REST calls
  WiFiClient client = server.available();
  if (client)
  {

    while (!client.available()) {
      delay(1);
    }
    if (client.available())
    {




      rest.handle(client);

    }
  }






}


void blinkLED(int timesToBlink, int delayBetweenBlinks)
{

  int i;
  for (i = 0; i < timesToBlink; i++)
  {
    digitalWrite(blinkPin, 0);
    delay(delayBetweenBlinks / 2);
    digitalWrite(blinkPin, 1);
    delay(delayBetweenBlinks / 2);
  }

}


void blinkIPAddress()
{

  Serial.print("Blinking local Address =");

  Serial.println(WiFi.localIP());
  IPAddress LIP;
  LIP = WiFi.localIP();

  int lowOct;
  lowOct = LIP[3];

  // now do the blink

  int check100;
  int check10;
  int check1;
  delay(1000);

  check100 = lowOct / 100;

  Serial.println(check100);
  if (check100 != 0)
  {
    blinkLED(check100, 250);
  }

  delay(1000);

  check10 = (lowOct - check100 * 100) / 10;
  Serial.println(check10);
  if (check10 != 0)
  {
    blinkLED(check10, 500);
  }
  else
  {
    blinkLED(10, 100); // blink fast is 0

  }
  delay(1000);

  check1 = ( lowOct - check100 * 100 - check10 * 10);
  Serial.println(check1);
  if (check1 != 0)
  {
    blinkLED(check1, 500);
  }
  else
  {
    blinkLED(10, 100); // blink fast is 0

  }

}

void printDigits(int digits) {
  // utility function for digital clock display: prints an leading 0

  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}


void return2Digits(char returnString[], char *buffer2, int digits)
{
  if (digits < 10)
    sprintf(returnString, "0%i", digits);
  else
    sprintf(returnString, "%i", digits);

  strcpy(returnString, buffer2);


}

void buildTimeString(char returnString[], char *buffer2, tmElements_t convertTime)
{



  char myBuffer[5];
  sprintf(myBuffer, "%i-", tmYearToCalendar(convertTime.Year));
  strcat(returnString, myBuffer);

  return2Digits(myBuffer, myBuffer, convertTime.Month);
  strcat(returnString, myBuffer);
  strcat(returnString, "-");

  return2Digits(myBuffer, myBuffer, convertTime.Day);
  strcat(returnString, myBuffer);
  strcat(returnString, " ");

  return2Digits(myBuffer, myBuffer, convertTime.Hour);
  strcat(returnString, myBuffer);
  strcat(returnString, ":");

  return2Digits(myBuffer, myBuffer, convertTime.Minute);
  strcat(returnString, myBuffer);
  strcat(returnString, ":");

  return2Digits(myBuffer, myBuffer, convertTime.Second);
  strcat(returnString, myBuffer);



}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             "%02u/%02u/%04u %02u:%02u:%02u",
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.print(datestring);
}

String returnDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             "%04u-%02u-%02u %02u:%02u:%02u",
             dt.Year(),
             dt.Month(),
             dt.Day(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  return String(datestring);
}


float returnPercentLeftInBattery(float currentVoltage, float maxVolt)
{

  float returnPercent;

  float scaledVolts = currentVoltage / maxVolt;

  if (scaledVolts > 1.0)
    scaledVolts = 1.0;


  if (scaledVolts > .9686)
  {
    returnPercent = 10 * (1 - (1.0 - scaledVolts) / (1.0 - .9686)) + 90;
    return returnPercent;
  }

  if (scaledVolts > 0.9374)
  {
    returnPercent = 10 * (1 - (0.9686 - scaledVolts) / (0.9686 - 0.9374)) + 80;
    return returnPercent;
  }


  if (scaledVolts > 0.9063)
  {
    returnPercent = 30 * (1 - (0.9374 - scaledVolts) / (0.9374 - 0.9063)) + 50;
    return returnPercent;

  }

  if (scaledVolts > 0.8749)
  {
    returnPercent = 20 * (1 - (0.8749 - scaledVolts) / (0.9063 - 0.8749)) + 11;

    return returnPercent;
  }


  if (scaledVolts > 0.8437)
  {
    returnPercent = 15 * (1 - (0.8437 - scaledVolts) / (0.8749 - 0.8437)) + 1;
    return returnPercent;

  }

  if (scaledVolts > 0.8126)
  {
    returnPercent = 7 * (1 - (0.8126 - scaledVolts) / (0.8437 - 0.8126)) + 2;
    return returnPercent;
  }


  if (scaledVolts > 0.7812)
  {
    returnPercent = 4 * (1 - (0.7812 - scaledVolts) / (0.8126 - 0.7812)) + 1;
    return returnPercent;
  }

  return 0;
}
