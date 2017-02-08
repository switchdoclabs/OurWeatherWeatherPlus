/*********************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers


  This example is for a 128x64 size display using I2C to communicate

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information

  Heavily modified by SDL
*********************************************************************/

// prototypes

void setDisplayLine(int lineNumber, char *value);
void writeAllDisplayLines(int DisplayMode);

char displayLines[20][28];


// WeatherPlus Text Buffer Lines

void updateDisplay(int displayMode)
{

#ifdef OLED_Present

  char buffer[40];
  char returnString[200];

  Serial.print("displayMode=");
  Serial.println(displayMode);
  String windDirection;

  RtcDateTime now = Rtc.GetDateTime();

  String currentTimeString;
  currentTimeString = returnDateTime(now);

  switch (displayMode)
  {


    case DISPLAY_UPDATING:
      Serial.println("Updating OurWeather Software");



      setDisplayLine(0,  "OurWeather Updating");
      setDisplayLine(1, "");
      setDisplayLine(2, "");
      setDisplayLine(3, "");
      setDisplayLine(4, "");
      setDisplayLine(5, "");

      break;

    case DISPLAY_UPDATE_FINISHED:
      Serial.println("Finish OurWeather Software");



      setDisplayLine(0,  "OurWeather Updated");
      setDisplayLine(1, "Wait 120 Seconds");
      setDisplayLine(2, "Unplug Power");
      setDisplayLine(3, "Wait 15 Seconds");
      setDisplayLine(4, "Plug Power In");
      setDisplayLine(5, "");

      break;

    case DISPLAY_NO_UPDATE_AVAILABLE:
      setDisplayLine(0,  "You have the most");
      setDisplayLine(1, "recent software");
      setDisplayLine(2, "");
      setDisplayLine(3, "");
      setDisplayLine(4, "");
      setDisplayLine(5, "");

      break;

    case DISPLAY_NO_UPDATE_FAILED:
      setDisplayLine(0,  "Update Failed.");
      setDisplayLine(1, "Try again later.");
      setDisplayLine(2, "");
      setDisplayLine(3, "");
      setDisplayLine(4, "");
      setDisplayLine(5, "");

      break;




    case DISPLAY_POWERUP:
      Serial.println("OurWeather Booting Up");

      strcpy(buffer, "Ver: ");
      strcat(buffer, WEATHERPLUSESP8266VERSION);

      setDisplayLine(0,  "OurWeather Booting");
      setDisplayLine(1, buffer);
      setDisplayLine(2, "");
      setDisplayLine(3, "");

      break;

    case  DISPLAY_ACCESSPOINT:
      {
        Serial.println("OurWeather Setup");




        setDisplayLine(0,  "OurWeather AP");
        buffer[0] = '\0';
        strcpy(buffer, APssid.c_str());
        setDisplayLine(1, buffer);
        buffer[0] = '\0';

        IPAddress myIp2 = WiFi.softAPIP();
        sprintf(buffer, "%d.%d.%d.%d", myIp2[0], myIp2[1], myIp2[2], myIp2[3]);
        setDisplayLine(2, buffer);
      }
      break;


    case  DISPLAY_TRYING_AP:
      {
        Serial.println("OurWeather Setup");




        setDisplayLine(0,  "Trying WiFi AP");
        buffer[0] = '\0';
        strcpy(buffer, Wssid.c_str());
        setDisplayLine(1, buffer);
        buffer[0] = '\0';
        setDisplayLine(2, "");
        setDisplayLine(3, "");

      }
      break;

    case  DISPLAY_FAILING_AP:
      {

        setDisplayLine(0,  "Failing to connect WiFi AP");
        buffer[0] = '\0';
        strcpy(buffer, Wssid.c_str());
        setDisplayLine(1, buffer);
        buffer[0] = '\0';
        setDisplayLine(2, "Restarting OurWeather");
        setDisplayLine(3, "Try again....");

      }
      break;

    case DISPLAY_IPDISPLAY:

      {
        setDisplayLine(0, "OurWeather Connected");
        setDisplayLine(1, "IP Address:");



        IPAddress myIp2 = WiFi.localIP();
        sprintf(buffer, "%d.%d.%d.%d", myIp2[0], myIp2[1], myIp2[2], myIp2[3]);
        setDisplayLine(2, buffer);
      }
      break;

    case DISPLAY_SUNAIRPLUS:

      {
        setDisplayLine(0, "Solar Readings");
       setDisplayLine(1, "----------------");

        String stringSolar;
        stringSolar = "Battery:" + String(BatteryVoltage, 2) + "V/" + String(BatteryCurrent, 1) + "mA";
        setDisplayLine(2, const_cast<char*>(stringSolar.c_str()) );

        stringSolar = "Solar:" + String(SolarPanelVoltage, 2) + "V/" + String(SolarPanelCurrent, 1) + "mA";
        setDisplayLine(3, const_cast<char*>(stringSolar.c_str()) );

        stringSolar = "Load:" + String(LoadVoltage, 2) + "V/" + String(LoadCurrent, 1) + "mA";
        setDisplayLine(4, const_cast<char*>(stringSolar.c_str()) );




      }
      break;
      
    case DISPLAY_WXLINK:

      {
        setDisplayLine(0, "WXLink Readings");
       setDisplayLine(1, "----------------");

        String stringSolar;
        stringSolar = "Battery:" + String(WXBatteryVoltage, 2) + "V/" + String(WXBatteryCurrent, 1) + "mA";
        setDisplayLine(2, const_cast<char*>(stringSolar.c_str()) );

        stringSolar = "Solar:" + String(WXSolarPanelVoltage, 2) + "V/" + String(WXSolarPanelCurrent, 1) + "mA";
        setDisplayLine(3, const_cast<char*>(stringSolar.c_str()) );

        stringSolar = "Load:" + String(WXLoadCurrent, 1) + "mA";
        setDisplayLine(4, const_cast<char*>(stringSolar.c_str()) );
        
      setDisplayLine(5, "----------------");

        stringSolar = "MessageID: " + String(WXMessageID);
        setDisplayLine(6, const_cast<char*>(stringSolar.c_str()) );


      }
      break;


    case DISPLAY_WEATHER_SMALL:

      {

        String airQual;
        airQual = "Air Qual:" + reportAirQuality(currentAirQuality);
        setDisplayLine(6, const_cast<char*>(airQual.c_str()) );

        if (EnglishOrMetric == 0)
        {
          // English Units

          char floatString[15];

          buffer[0] = '\0';
          strcat(buffer, "OT:");
          dtostrf(AM2315_Temperature * 1.8 + 32.0, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "F");
          strcat(buffer, "  OH:");
          dtostrf(AM2315_Humidity, 3, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(0, buffer);

          buffer[0] = '\0';
          strcat(buffer, "IT:");
          dtostrf(BMP180_Temperature * 1.8 + 32.0, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "F");
          strcat(buffer, " BP:");
          dtostrf((BMP180_Pressure / 1000.0) * 0.29529980165, 5, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "in");
          setDisplayLine(1, buffer);

          buffer[0] = '\0';
          strcat(buffer, "Rain Total: ");
          dtostrf(rainTotal * 0.039370, 6, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "in");
          setDisplayLine(2, buffer);

          buffer[0] = '\0';
          strcat(buffer, "WindSpeed:");
          dtostrf(currentWindSpeed * 0.621371, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mph");
          setDisplayLine(3, buffer);

          buffer[0] = '\0';
          strcat(buffer, "Wind Gust:");
          dtostrf(currentWindGust * 0.621371, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mph");
          setDisplayLine(4, buffer);

          buffer[0] = '\0';
          strcat(buffer, "Wind Dir:");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "deg");
          setDisplayLine(5, buffer);




          /*           OutTemp:  OutHum:
                       InTemp:  BP:
                       Alt:
                       WindSpeed:
                       WindGust:
                       Wind Direction:
          */
        }
        else
        {
          // Metric Units

          char floatString[15];

          buffer[0] = '\0';
          strcat(buffer, "OT: ");
          dtostrf(AM2315_Temperature, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "C");
          strcat(buffer, " OH: ");
          dtostrf(AM2315_Humidity, 3, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(0, buffer);

          buffer[0] = '\0';
          strcat(buffer, "IT: ");
          dtostrf(BMP180_Temperature, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "C");
          strcat(buffer, " BP: ");
          dtostrf(BMP180_Pressure / 100.0, 6, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kPa");
          setDisplayLine(1, buffer);

          buffer[0] = '\0';
          strcat(buffer, "Rain Total: ");
          dtostrf(rainTotal, 6, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mm");
          setDisplayLine(2, buffer);

          buffer[0] = '\0';
          strcat(buffer, "WindSpeed: ");
          dtostrf(currentWindSpeed, 6, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kph");
          setDisplayLine(3, buffer);

          buffer[0] = '\0';
          strcat(buffer, "Wind Gust: ");
          dtostrf(currentWindGust, 6, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kph");
          setDisplayLine(4, buffer);

          buffer[0] = '\0';
          strcat(buffer, "Wind Dir: ");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "deg");
          setDisplayLine(5, buffer);


        }


      }
      break;

    case DISPLAY_WEATHER_MEDIUM:

      {
        if (EnglishOrMetric == 0)
        {
          // English Units

          char floatString[15];

          buffer[0] = '\0';
          strcat(buffer, "OT:");
          dtostrf(AM2315_Temperature * 1.8 + 32.0, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "F");
          setDisplayLine(0, buffer);
          buffer[0] = '\0';

          strcat(buffer, "OH:");
          dtostrf(AM2315_Humidity, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(1, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          strcat(buffer, "IT: ");
          dtostrf(BMP180_Temperature * 1.8 + 32.0, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "F");
          setDisplayLine(2, buffer);
          buffer[0] = '\0';

          strcat(buffer, "BP:");
          dtostrf((BMP180_Pressure / 1000.0) * 0.29529980165, 5, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "in");
          setDisplayLine(3, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          strcat(buffer, "RT: ");
          dtostrf(rainTotal * 0.039370, 5, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "in");
          setDisplayLine(4, buffer);

          buffer[0] = '\0';
          strcat(buffer, "WS:");
          dtostrf(currentWindSpeed * 0.621371, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mph");
          setDisplayLine(5, buffer);

          buffer[0] = '\0';
          strcat(buffer, "WG:");
          dtostrf(currentWindGust * 0.621371, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mph");
          setDisplayLine(6, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          strcat(buffer, "WD:");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "d");
          setDisplayLine(7, buffer);




          /*           OutTemp:  OutHum:
                       InTemp:  BP:
                       Alt:
                       WindSpeed:
                       WindGust:
                       Wind Direction:
          */
        }
        else
        {
          // Metric Units

          char floatString[15];

          buffer[0] = '\0';
          strcat(buffer, "OT: ");
          dtostrf(AM2315_Temperature, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "C");
          setDisplayLine(0, buffer);
          buffer[0] = '\0';

          strcat(buffer, "OH: ");
          dtostrf(AM2315_Humidity, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(1, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          strcat(buffer, "IT: ");
          dtostrf(BMP180_Temperature, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "C");
          setDisplayLine(2, buffer);
          buffer[0] = '\0';

          strcat(buffer, "BP:");
          dtostrf(BMP180_Pressure / 100.0, 6, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kPa");
          setDisplayLine(3, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          strcat(buffer, "RT:");
          dtostrf(rainTotal, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mm");
          setDisplayLine(4, buffer);


          buffer[0] = '\0';
          strcat(buffer, "WS:");
          dtostrf(currentWindSpeed, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kph");
          setDisplayLine(5, buffer);

          buffer[0] = '\0';
          strcat(buffer, "WG:");
          dtostrf(currentWindGust, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kph");
          setDisplayLine(6, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          strcat(buffer, "WD:");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "d");
          setDisplayLine(7, buffer);


        }



        // display date and time

        buffer[0] = '\0';
        strcat(buffer, currentTimeString.c_str());
        setDisplayLine(8, buffer);
        setDisplayLine(9, "");
        String airQual;
        airQual =  reportAirQuality(currentAirQuality);
        setDisplayLine(11, const_cast<char*>(airQual.c_str()) );
        setDisplayLine(10, " Air Qual");

      }

      break;

    case DISPLAY_WEATHER_LARGE:
      {
        if (EnglishOrMetric == 0)
        {
          // English Units

          char floatString[15];

          buffer[0] = '\0';
          setDisplayLine(0, "OutTm");
          dtostrf(AM2315_Temperature * 1.8 + 32.0, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "F");
          setDisplayLine(1, buffer);
          buffer[0] = '\0';


          setDisplayLine(2, "OutHm");
          dtostrf(AM2315_Humidity, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(3, buffer);

          //updateAllWeatherVariables();
          buffer[0] = '\0';
          setDisplayLine(4,  "InTmp");
          dtostrf(BMP180_Temperature * 1.8 + 32.0, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "F");
          setDisplayLine(5, buffer);
          buffer[0] = '\0';

          setDisplayLine(6, "BPres");
          dtostrf((BMP180_Pressure / 1000.0) * 0.29529980165, 5, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "in");
          setDisplayLine(7, buffer);

          //updateAllWeatherVariables();
          buffer[0] = '\0';
          setDisplayLine(8, "RnTot");
          dtostrf(rainTotal * 0.039370, 5, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "in");
          setDisplayLine(9, buffer);

          buffer[0] = '\0';
          setDisplayLine(10, "WndSp");
          dtostrf(currentWindSpeed * 0.621371, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mph");
          setDisplayLine(11, buffer);

          buffer[0] = '\0';
          setDisplayLine(12, "WndGs");
          dtostrf(currentWindGust * 0.621371, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mph");
          setDisplayLine(13, buffer);

          //updateAllWeatherVariables();
          buffer[0] = '\0';
          setDisplayLine(14,  "WndDr");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "d");
          setDisplayLine(15, buffer);




          /*           OutTemp:  OutHum:
                       InTemp:  BP:
                       Alt:
                       WindSpeed:
                       WindGust:
                       Wind Direction:
          */
        }
        else
        {
          // Metric Units

          char floatString[15];

          buffer[0] = '\0';
          setDisplayLine(0, "OutTm");
          dtostrf(AM2315_Temperature, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "C");
          setDisplayLine(1, buffer);
          buffer[0] = '\0';

          setDisplayLine(2, "OutHm");
          dtostrf(AM2315_Humidity, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(3, buffer);

          //updateAllWeatherVariables();
          buffer[0] = '\0';
          setDisplayLine(4, "InTmp");
          dtostrf(BMP180_Temperature, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "C");
          setDisplayLine(5, buffer);
          buffer[0] = '\0';

          setDisplayLine(6, "BPres");
          dtostrf(BMP180_Pressure / 100.0, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "k");
          setDisplayLine(7, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          setDisplayLine(8, "RnTot");
          dtostrf(rainTotal, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mm");
          setDisplayLine(9, buffer);

          buffer[0] = '\0';
          setDisplayLine(10, "WndSp");
          dtostrf(currentWindSpeed, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kph");
          setDisplayLine(11, buffer);

          buffer[0] = '\0';
          setDisplayLine(12, "WndGs");
          dtostrf(currentWindGust, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kph");
          setDisplayLine(13, buffer);


          //updateAllWeatherVariables();
          buffer[0] = '\0';
          setDisplayLine(14, "WndDr");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "d");
          setDisplayLine(15, buffer);


        }
        // display date and time
        buffer[0] = '\0';
        strcat(buffer, currentTimeString.c_str());
        setDisplayLine(16, buffer);
        String airQual;
        airQual = " Air Qual " + reportAirQuality(currentAirQuality);
        setDisplayLine(17, const_cast<char*>(airQual.c_str()) );

      }
      break;

    case DISPLAY_WEATHER_DEMO:
      {
        if (EnglishOrMetric == 0)
        {
          // English Units

          char floatString[15];

          buffer[0] = '\0';
          setDisplayLine(0, "OutTm");
          dtostrf(AM2315_Temperature * 1.8 + 32.0, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "F");
          setDisplayLine(1, buffer);
          buffer[0] = '\0';


          setDisplayLine(2, "OutHm");
          dtostrf(AM2315_Humidity, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(3, buffer);


          buffer[0] = '\0';
          setDisplayLine(4, "WndSp");
          dtostrf(currentWindSpeed * 0.621371, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mph");
          setDisplayLine(5, buffer);

          buffer[0] = '\0';
          setDisplayLine(6,  "WndDr");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "d");
          setDisplayLine(7, buffer);

          buffer[0] = '\0';
          setDisplayLine(8, "RnTot");
          dtostrf(rainTotal * 0.039370, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "in");
          setDisplayLine(9, buffer);






          /*           OutTemp:  OutHum:
                       InTemp:  BP:
                       Alt:
                       WindSpeed:
                       WindGust:
                       Wind Direction:
          */
        }
        else
        {
          // Metric Units

          char floatString[15];

          buffer[0] = '\0';
          setDisplayLine(0, "OutTm");
          dtostrf(AM2315_Temperature, 6, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "C");
          setDisplayLine(1, buffer);
          buffer[0] = '\0';

          setDisplayLine(2, "OutHm");
          dtostrf(AM2315_Humidity, 6, 2, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "%");
          setDisplayLine(3, buffer);

          buffer[0] = '\0';
          setDisplayLine(4, "WndSp");
          dtostrf(currentWindSpeed, 4, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "kph");
          setDisplayLine(5, buffer);

          //updateAllWeatherVariables();
          buffer[0] = '\0';
          setDisplayLine(6, "WndDr");
          windDirection = returnDirectionFromDegrees(int(currentWindDirection));
          windDirection = windDirection + "-";
          strcat(buffer, windDirection.c_str());
          dtostrf(currentWindDirection, 3, 0, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "d");
          setDisplayLine(7, buffer);

          buffer[0] = '\0';
          setDisplayLine(8, "RnTot");
          dtostrf(rainTotal, 5, 1, floatString);
          strcat(buffer, floatString);
          strcat(buffer, "mm");
          setDisplayLine(9, buffer);

        }

        // display date and time
        buffer[0] = '\0';
        strcat(buffer, currentTimeString.c_str());
        setDisplayLine(10, buffer);
        setDisplayLine(11, "");

      }
      break;
    case DISPLAY_STATUS:

      setDisplayLine(0, "WeatherPlus Status");
      setDisplayLine(1, "# hits, etc");
      setDisplayLine(2, "");
      setDisplayLine(3, "");
      break;




    default:

      break;

  }
  writeAllDisplayLines(displayMode);


#endif

}



void setDisplayLine(int lineNumber, char *value)
{
  if (lineNumber < 32)
  {

    strncpy( displayLines[lineNumber], value, 20);

  }




}


void writeAllDisplayLines(int DisplayMode)
{
  display.clearDisplay();

  switch (DisplayMode)
  {
    case DISPLAY_POWERUP:
    case DISPLAY_UPDATE_FINISHED:
    case DISPLAY_UPDATING:
    case DISPLAY_NO_UPDATE_AVAILABLE:
    case DISPLAY_NO_UPDATE_FAILED:
    case DISPLAY_STATUS:

      {
        int textSize = 1;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 6; i++)
        {
          display.setCursor(0, (30 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }

      break;
    case DISPLAY_IPDISPLAY:
    case DISPLAY_ACCESSPOINT:
    case DISPLAY_TRYING_AP:
    case DISPLAY_FAILING_AP:


      {
        int textSize = 1;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 3; i++)
        {

          if (i == 0)
            textSize = 1;
          if (i == 1)
            textSize = 1;
          if (i == 2)
            textSize = 1;
          display.setTextSize(textSize);

          display.setCursor(0, 20 * (i));

          //display.setCursor(0, (30 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }

      break;

    case DISPLAY_SUNAIRPLUS:

      {
        int textSize = 1;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 5; i++)
        {
          display.setCursor(0, (29 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }
      break;
      
   case DISPLAY_WXLINK:
      {
        int textSize = 1;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 7; i++)
        {
          display.setCursor(0, (29 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }



      break;
    case DISPLAY_WEATHER_SMALL:
      {
        int textSize = 1;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 7; i++)
        {
          display.setCursor(0, (29 - (2 - textSize) * 20) * (i));

          display.println(displayLines[i]);
          display.display();
        }
      }



      break;

    case DISPLAY_WEATHER_MEDIUM:

      {
        int textSize = 2;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;
        for (i = 0; i < 6; i++)
        {
          display.clearDisplay();
          int j;
          for (j = 0; j < 2; j++)
          {
            display.setCursor(0, 20  * (j));

            display.println(displayLines[i * 2 + j]);


          }
          display.display();
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
          delay(1800);
        }

      }


      break;

    case DISPLAY_WEATHER_LARGE:


      {
        int textSize = 4;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;

        for (i = 0; i < 10; i++)
        {
          display.clearDisplay();
          int j;
          for (j = 0; j < 2; j++)
          {
            display.setCursor(0, 33  * (j));

            if (j == 1)
              display.setTextSize(3);
            else
              display.setTextSize(4);

            if (i == 8)
              display.setTextSize(2);

            display.println(displayLines[i * 2 + j]);


          }
          display.display();
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
          delay(1800);
        }

      }

      break;

    case DISPLAY_WEATHER_DEMO:


      {
        int textSize = 4;
        display.setTextSize(textSize);
        display.setTextColor(WHITE);
        int i;

        for (i = 0; i < 6; i++)
        {
          display.clearDisplay();
          int j;
          for (j = 0; j < 2; j++)
          {
            display.setCursor(0, 33  * (j));

            if (j == 1)
              display.setTextSize(3);
            else
              display.setTextSize(4);

            if (i == 5)
              display.setTextSize(2);

            display.println(displayLines[i * 2 + j]);


          }
          display.display();
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
          delay(900);
        }

      }

      break;

    default:
      break;

  }

}






#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

//static const unsigned char PROGMEM logo16_glcd_bmp[] =
static const unsigned char logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};



void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
    }
    display.display();
    delay(200);

    // then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i = 0; i < display.height(); i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawrect(void) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display();
  }
}

void testdrawline() {
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  delay(250);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}

#if (SSD1306_LCDHEIGHT != 64)
// #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void OLEDDisplaySetup()   {



  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(4000);

  // Clear the buffer.
  display.clearDisplay();
  /*
    // draw a single pixel
    display.drawPixel(10, 10, WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    display.display();
    delay(2000);
    display.clearDisplay();


     // draw many lines
     testdrawline();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw rectangles
     testdrawrect();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw multiple rectangles
     testfillrect();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw mulitple circles
     testdrawcircle();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw a white circle, 10 pixel radius
     display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
     display.display();
     delay(2000);
     display.clearDisplay();

     testdrawroundrect();
     delay(2000);
     display.clearDisplay();

     testfillroundrect();
     delay(2000);
     display.clearDisplay();

     testdrawtriangle();
     delay(2000);
     display.clearDisplay();

     testfilltriangle();
     delay(2000);
     display.clearDisplay();

     // draw the first ~12 characters in the font
     testdrawchar();
     display.display();
     delay(2000);
     display.clearDisplay();

     // draw scrolling text
     testscrolltext();
     delay(2000);
     display.clearDisplay();

     // text display tests
     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(0,0);
     display.println("Hello, world!");
     display.setTextColor(BLACK, WHITE); // 'inverted' text
     display.println(3.141592);
     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.print("0x"); display.println(0xDEADBEEF, HEX);
     display.display();
     delay(2000);

     // miniature bitmap display
     display.clearDisplay();
     display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
     display.display();

     // invert the display
     display.invertDisplay(true);
     delay(1000);
     display.invertDisplay(false);
     delay(1000);

     // draw a bitmap icon and 'animate' movement
     testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
  */
}

/*
  void loop() {

  }
*/

