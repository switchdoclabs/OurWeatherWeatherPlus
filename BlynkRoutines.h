// Blynk Routines
// December 2, 2018


void myBTimerEvent();

void startBlynk()
{



  Blynk.config(BlynkAuthCode.c_str());

  if (UseBlynk == false)
  {


    // Setup a function to be called every 10 seconds
    Btimer.setInterval(10000L, myBTimerEvent);
  }
  UseBlynk = true;


  Blynk.connect();
  writeToBlynkStatusTerminal((String)"OurWeather Version V" + (String)WEATHERPLUSESP8266VERSION + " Started From Admin Page");

}
void myBTimerInstantEvent();
/*
  void writeToBlynkStatusTerminal(String statement)
  {

  statusTerminalStatement = statement;
  Btimer.setTimeout(100, myBTimerInstantEvent);

  }
*/
void writeToBlynkStatusTerminal(String statement)
{

  RtcDateTime now = Rtc.GetDateTime();
  String currentTimeString;
  currentTimeString = returnDateTime(now);

  statusTerminal.println(currentTimeString + (String)": " + statement );
  statusTerminal.flush();   // Ensure that data was sent out of device
#ifdef DEBUGBLYNK
  Serial.println("writeToBlynkStatusTerminal - Data Sent to Blynk");
#endif

}

void writeToStatusLine(String statement)
{
  Blynk.virtualWrite(V31, statement);
}




void myBTimerEvent()
{

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
#ifdef DEBUGBLYNK
  Serial.println("myBTimerEvent - Starting");
#endif
  Blynk.connect();
#ifdef DEBUGBLYNK
  if (Blynk.connected())
  {
    Serial.println("Blynk Connected");

  }
  else
  {
    Serial.println("Blynk NOT Connected");

  }
#endif

  // only 10 values a second
  RtcDateTime now = Rtc.GetDateTime();
  String currentTimeString;
  currentTimeString = returnDateTime(now);
  Blynk.virtualWrite(V44, currentTimeString);

  Blynk.virtualWrite(V1, String(AM2315_Humidity) + "%");
  Blynk.virtualWrite(V4, String(AM2315_Humidity));


  char buffer[20];
  if (EnglishOrMetric == 0)
  {
    // English Units


    char floatString[15];
    float FTemp;
    FTemp = AM2315_Temperature * 1.8 + 32.0;

    buffer[0] = '\0';
    dtostrf(FTemp, 5, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "F");
    Blynk.virtualWrite(V0, buffer);
    Blynk.virtualWrite(V10, FTemp );
  }
  else
  {
    // English Units


    char floatString[15];
    float FTemp;
    FTemp = AM2315_Temperature;

    buffer[0] = '\0';
    dtostrf(FTemp, 5, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "C");
    Blynk.virtualWrite(V0, buffer);
    Blynk.virtualWrite(V10, FTemp );

  }

  if (EnglishOrMetric == 0)
  {
    // English Units


    char floatString[15];
    float FTemp;
    FTemp = dewpoint * 1.8 + 32.0;

    buffer[0] = '\0';
    dtostrf(FTemp, 5, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "F");
    Blynk.virtualWrite(V70, buffer);

  }
  else
  {
    // English Units


    char floatString[15];
    float FTemp;
    FTemp = dewpoint;

    buffer[0] = '\0';
    dtostrf(FTemp, 5, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "C");
    Blynk.virtualWrite(V70, buffer);


  }


  char floatString[15];
  String windDirection;


  // Wind variables
  buffer[0] = '\0';
  dtostrf(currentWindDirection, 3, 0, floatString);
  strcat(buffer, floatString);
  windDirection =   "/" + returnDirectionFromDegrees(int(currentWindDirection));
  strcat(buffer, windDirection.c_str());
  Blynk.virtualWrite(V2, buffer);


  if (EnglishOrMetric == 0)
  {
    // English Units


    buffer[0] = '\0';
    dtostrf(currentWindSpeed * 0.621371, 5, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "mph");
    Blynk.virtualWrite(V9, buffer );


  }
  else
  {
    buffer[0] = '\0';
    dtostrf(currentWindSpeed, 5, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "kph");
    Blynk.virtualWrite(V9, buffer );

  }

  // Rain

  if (EnglishOrMetric == 0)
  {
    // English Units


    buffer[0] = '\0';
    dtostrf(rainTotal * 0.039370, 6, 2, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "in");
    Blynk.virtualWrite(V3, buffer );


  }
  else
  {

    buffer[0] = '\0';
    dtostrf(rainTotal, 6, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "mm");
    Blynk.virtualWrite(V3, buffer );

  }


  // baro pressure

  if (EnglishOrMetric == 0)
  {
    // English Units
    buffer[0] = '\0';
    dtostrf((BMP180_Pressure / 1000.0) * 0.29529980165, 5, 2, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "in");
    Blynk.virtualWrite(V40, buffer );
    Blynk.virtualWrite(V41, (BMP180_Pressure / 1000.0) * 0.29529980165 );

  }

  else
  {
    buffer[0] = '\0';
    dtostrf(BMP180_Pressure / 100.0, 6, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "kPa");
    Blynk.virtualWrite(V40, buffer );
    Blynk.virtualWrite(V41, (BMP180_Pressure / 1000.0)  );
  }


  Blynk.virtualWrite(V7, reportAirQuality(currentAirQuality)  );



  // inside temperature
  if (EnglishOrMetric == 0)
  {
    // English Units
    buffer[0] = '\0';

    dtostrf(BMP180_Temperature * 1.8 + 32.0, 5, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "F");
    Blynk.virtualWrite(V21, buffer );
  }
  else
  {


    buffer[0] = '\0';

    dtostrf(BMP180_Temperature, 4, 1, floatString);
    strcat(buffer, floatString);
    strcat(buffer, "C");
    Blynk.virtualWrite(V21, buffer );
  }

  // Air Quality if Present

  if (AirQualityPresent) {

    Blynk.virtualWrite(V19, INTcurrentAirQualitySensor );
  }

  // Now solar Variables if present
  delay(500); // push variables into next second - avoid flood

  if (SunAirPlus_Present)
  {


    Blynk.virtualWrite(V56, String(returnPercentLeftInBattery(BatteryVoltage , 4.19)) );
    Blynk.virtualWrite(V57, String(returnPercentLeftInBattery(BatteryVoltage , 4.19)) + "%" );
    //
    // Now do the graphs and results on the solar page
    delay(1000); // push variables into next second - avoid flood

    dtostrf(SolarPanelVoltage, 5, 2, floatString);
    Blynk.virtualWrite(V80,  atof(floatString));
    Blynk.virtualWrite(V50,  String(floatString) + "V");

    dtostrf(SolarPanelCurrent, 5, 1, floatString);
    Blynk.virtualWrite(V81, atof(floatString ));
    Blynk.virtualWrite(V51, String(floatString ) + "mA");

    dtostrf(BatteryVoltage, 5, 2, floatString);
    Blynk.virtualWrite(V82,  atof(floatString));
    Blynk.virtualWrite(V52,  String(floatString) + "V");

    dtostrf(BatteryCurrent, 5, 1, floatString);
    Blynk.virtualWrite(V83, atof(floatString) );
    Blynk.virtualWrite(V53, String(floatString)  + "mA");



    dtostrf(LoadVoltage, 5, 2, floatString);
    Blynk.virtualWrite(V84,  atof(floatString));
    Blynk.virtualWrite(V54, String(floatString)  + "V");

  
    
    dtostrf(LoadCurrent, 5, 1, floatString);
    Blynk.virtualWrite(V85, atof(floatString) );
    Blynk.virtualWrite(V55, String(floatString)  + "mA");


    // Power

    dtostrf((BatteryVoltage * BatteryCurrent ) / 1000.0, 5, 2, floatString);
    Blynk.virtualWrite(V60, String(floatString) + "W");

    dtostrf((SolarPanelVoltage * SolarPanelCurrent) / 1000.0, 5, 2, floatString);
    Blynk.virtualWrite(V61, String(floatString) + "W" );

    dtostrf((LoadVoltage * LoadCurrent) / 1000.0, 5, 2, floatString);
    Blynk.virtualWrite(V62,  String(floatString) + "W");



  }


#ifdef DEBUGBLYNK
  Serial.println("myBTimerEvent - Data Sent to Blynk");
#endif
}
/*

  BLYNK_WRITE(V8)
  {
  int pinValue = param.asInt(); // assigning incoming value from pin V6 to a variable

  #ifdef DEBUGBLYNK
  // process received value
  Serial.print("V8 pinValue=");
  Serial.println(pinValue);
  if (pinValue == 0)
  {
    if (EnglishOrMetric == 1)
    {
      writeToBlynkStatusTerminal("Units set to English");
      EnglishOrMetric = 0;
    }
  }
  else
  {
    if (EnglishOrMetric == 0)
    {
      writeToBlynkStatusTerminal("Units set to Metric");
      EnglishOrMetric = 1;
    }
  }

  #endif

  }
*/
/*
  // This function will run every time Blynk connection is established
  BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  //Blynk.syncAll();

  // You can also update individual virtual pins like this:
  Blynk.syncVirtual(V8);

  }
*/

