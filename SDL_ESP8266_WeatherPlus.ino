// Filename WeatherPlus.ino
// Version 1.18 September 2016
// SwitchDoc Labs, LLC
//

//
// Started From:
// Filename ArduinoConnectServer.ino
// Version 1.5 08/15/13 RV MiloCreek
//


#define WEATHERPLUSESP8266VERSION "018"

// define DEBUGPRINT to print out lots of debugging information for WeatherPlus.
#undef DEBUGPRINT


// Change this to undef if you don't have the OLED present0
#define OLED_Present

// BOF preprocessor bug prevent - insert on top of your arduino-code
#if 1
__asm volatile ("nop");
#endif

// Board options

#pragma GCC diagnostic ignored "-Wwrite-strings"

extern "C" {
#include "user_interface.h"
}
//#include "Time/TimeLib.h"
#include "TimeLib.h"

#include <ESP8266WiFi.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>



#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

//gets called when WiFiManager enters configuration mode


//void configModeCallback (WiFiManager *myWiFiManager)
void configModeCallback ()
{

  Serial.println("Entered config mode");

  Serial.println(WiFi.softAPIP());

}

// OTA updated
#include <ESP8266WiFiMulti.h>



#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <EEPROM.h>


#include "config.h"




#include <Wire.h>

#include <Arduino.h> //needed for Serial.println

// debug the REST library

#define DEBUG_MODE 1

#include "aREST.h"

#include <String.h>

// display modes
#define DISPLAY_POWERUP 0
#define DISPLAY_IPDISPLAY 1
#define DISPLAY_WEATHER_SMALL 2
#define DISPLAY_WEATHER_MEDIUM 3
#define DISPLAY_WEATHER_LARGE 4
#define DISPLAY_STATUS 5
#define DISPLAY_ACCESSPOINT 6
#define DISPLAY_WEATHER_DEMO 7
#define DISPLAY_TRYING_AP 8
#define DISPLAY_FAILING_AP 9
#define DISPLAY_DATETIME 10
#define DISPLAY_UPDATING 11
#define DISPLAY_NO_UPDATE_AVAILABLE 12
#define DISPLAY_NO_UPDATE_FAILED 13
#define DISPLAY_UPDATE_FINISHED 14

#define DEBUG


// Rest Interface


#define PREFIX ""

String RestTimeStamp;
String RestDataString;
String Version;

//----------------------------------------------------------------------
//Local WiFi


int WiFiSetupFlag = 0;

String APssid;

String Wssid;
String WPassword;

WiFiServer server(WEB_SERVER_PORT);



//----------------------------------------------------------------------


int blinkPin = 0;                // pin to blink led at each reading
// Create an instance of the server


// Create aREST instance
aREST rest = aREST();
// commands are functions that get called by the webserver framework
// they can read any posted data from client, and they output to server




#include "elapsedMillis.h"

elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop

elapsedMillis timeElapsed300Seconds; //declare global if you don't want it reset every time loop

// BMP180 / BMP280 Sensor
// Both are stored in BMP180 variables
//

#include "MAdafruit_BMP280.h"
#include "MAdafruit_BMP085.h"
Adafruit_BMP280 bme;

Adafruit_BMP085 bmp;
#define SENSORS_PRESSURE_SEALEVELHPA 1015.00
float altitude_meters;
float BMP180_Temperature;
float BMP180_Pressure;
float BMP180_Altitude;

bool BMP180Found;
bool BMP280Found;


int EnglishOrMetric;   // 0 = English units, 1 = Metric

int WeatherDisplayMode;

// DS3231 Library functions

#include "RtcDS3231.h"

RtcDS3231 Rtc;


// AM2315

float AM2315_Temperature;
float AM2315_Humidity;

#include "SDL_ESP8266_HR_AM2315.h"


SDL_ESP8266_HR_AM2315 am2315;
float dataAM2315[2];  //Array to hold data returned by sensor.  [0,1] => [Humidity, Temperature]


boolean AOK;  // 1=successful read

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


// Station Name

String stationName;
String adminPassword;

// Health Indications for WeatherPlus

int heapSize;

// WeatherUnderground

String WeatherUnderground_StationID;
String WeatherUnderground_StationKey;



// WeatherRack


float windSpeedMin;
float windSpeedMax;
float windGustMin;
float windGustMax;
float windDirectionMin;
float windDirectionMax;



float currentWindSpeed;
float currentWindGust;
float currentWindDirection;

float rainTotal;


float rainCalendarDay;
int lastDay;

float startOfDayRain;

#include "SDL_RasPiGraphLibrary.h"
// setup the RasPiConnect Graph Arrays
SDL_RasPiGraphLibrary windSpeedGraph(10, SDL_MODE_LABELS);
SDL_RasPiGraphLibrary windGustGraph(10, SDL_MODE_LABELS);
SDL_RasPiGraphLibrary windDirectionGraph(10, SDL_MODE_LABELS);


char windSpeedBuffer[150];  // wind speed graph
char windGustBuffer[150];  // wind speed graph
char windDirectionBuffer[150];  // wind speed graph





// WeatherRack

// LED connected to digital GPIO 0
int WpinLED = 0;
// Anenometer connected to GPIO 14
int pinAnem = 14;
// Rain Bucket connected to  GPIO 12
int pinRain = 12;


#include "OWMAdafruit_ADS1015.h"


Adafruit_ADS1015 ads1015(0x49);

int current_quality = -1;
Adafruit_ADS1115 adsAirQuality(0x48);



long currentAirQuality;
long currentAirQualitySensor;
int INTcurrentAirQualitySensor;
bool AirQualityPresent = false;

#include "AirQualitySensor.h"

#include "SDL_Weather_80422.h"


SDL_Weather_80422 weatherStation(pinAnem, pinRain, 0, 0, A0, SDL_MODE_INTERNAL_AD );

// SDL_MODE_I2C_ADS1015
//

// RasPiConnect








long messageCount;

static uint8_t mac[] = LOCALMAC;
static uint8_t ip[] = LOCALIP;

// this is our current command object structure.  It is only valid inside void jsonCmd
typedef struct {
  char ObjectID[40];
  char ObjectFlags[40];
  char ObjectAction[40];
  char ObjectName[40];
  char ObjectServerID[40];
  char Password[40];
  char ObjectType[40];
  char Validate[40];
} currentObjectStructure;

char *md5str;

char ST1Text[40];   // used in ST-1 Send text control


char bubbleStatus[40];   // What to send to the Bubble status


#include "RainFunctions.h"

float lastRain;
#include "WeatherUnderground.h"


#include "Utils.h"





// OLED Constants


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// aREST functions

#include "aRestFunctions.h"

// OLED Display

#include "OWMAdafruit_GFX.h"
#include "ESP_SSD1306.h"


#define min(a,b) ((a)<(b)?(a):(b))

#define OLED_RESET 4



ESP_SSD1306 display(OLED_RESET);

#include "OLEDDisplay.h"



//
//
// setup()
//
//
#ifdef DEBUGPRINT
long AM2315BadCount;
long AM2315TotalCount;
byte FirstBadReply[10];
#endif


RtcDateTime lastBoot;
void setup() {


  // WiFi reset loop fix - erase the WiFi saved area

  WiFi.persistent(false);


#ifdef DEBUGPRINT
  AM2315BadCount = -1;
  AM2315TotalCount = 0;

  for (uint8_t i = 0; i < 10; i++)
    FirstBadReply[i] = 0;
#endif

  BMP180Found = false;
  BMP280Found = false;
  stationName = "";



  WeatherUnderground_StationID = "XXXX";
  WeatherUnderground_StationKey = "YYYY";

  adminPassword = "admin";
  altitude_meters = 637.0;  // default to 611

  pinMode(blinkPin, OUTPUT);        // pin that will blink every reading
  digitalWrite(blinkPin, HIGH);  // High of this pin is LED OFF


  Serial.begin(115200);           // set up Serial library at 9600 bps

  // Setup DS3231 RTC

  //--------RTC SETUP ------------
  Rtc.Begin();
#if defined(ESP8266)
  Wire.begin(5, 4);
#endif

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Serial.println("--------");
  printDateTime(compiled);
  Serial.println("--------");
  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
    // Common Cuases:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");

    // following line sets the RTC to the date & time this sketch was compiled
    // it will also reset the valid flag internally unless the Rtc device is
    // having an issue

    Rtc.SetDateTime(compiled);
  }

  RtcDateTime now = Rtc.GetDateTime();

  lastBoot = now;

  rainCalendarDay = 0.0;
  startOfDayRain = 0.0;

  lastDay = now.Day();

  String currentTimeString;
  currentTimeString = returnDateTime(now);

  Serial.print("now fromRTC =");
  Serial.println(currentTimeString);

  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);


  EEPROM.begin(512);

#ifdef OLED_Present
  OLEDDisplaySetup();
  updateDisplay(DISPLAY_POWERUP);
#endif

  delay(2000);

  if (digitalRead(0) == 0)
  {
    Serial.println("GPIO0 button down - Invalidating EEPROM");
    invalidateEEPROMState();

  }




  readEEPROMState();

  const char APpassphrase[] = "OurWeather";

  // Append the last two bytes of the MAC (HEX'd) to string to make unique
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  APssid = "OurWeather - " + macID;


  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(true);
  //reset saved settings
  //wifiManager.resetSettings();


  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  wifiManager.setTimeout(600);
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect(APssid.c_str())) {
    Serial.println("failed to connect and hit timeout");
    blinkLED(4, 300);  // blink 4, failed to connect
    //reset and try again, or maybe put it to deep sleep
    //ESP.reset();
    //delay(1000);
  }



  writeEEPROMState();


  blinkLED(2, 300);  // blink twice - OK!
  heapSize = ESP.getFreeHeap();

  RestTimeStamp = "";
  RestDataString = "";
  Version =   WEATHERPLUSESP8266VERSION;
  server.begin();

  rest.variable("OurWeatherTime", &RestTimeStamp);
  rest.variable("FullDataString", &RestDataString);
  rest.variable("FirmwareVersion", &Version);

  rest.variable("IndoorTemperature", &BMP180_Temperature);
  rest.variable("BarometricPressure", &BMP180_Pressure);
  rest.variable("Altitude", &BMP180_Altitude);

  rest.variable("OutdoorTemperature", &AM2315_Temperature);
  rest.variable("OutdoorHumidity", &AM2315_Humidity);

  rest.variable("CurrentWindSpeed", &currentWindSpeed);
  rest.variable("CurrentWindGust", &currentWindGust);

  rest.variable("CurrentWindDirection", &currentWindDirection);


  rest.variable("EnglishOrMetric", &EnglishOrMetric);

  rest.variable("RainTotal", &rainTotal);
  rest.variable("WindSpeedMin", &windSpeedMin);
  rest.variable("WindSpeedMax", &windSpeedMax);
  rest.variable("WindGustMin", &windGustMin);
  rest.variable("WindGustMax", &windGustMax);
  rest.variable("WindDirectionMin", &windDirectionMin);
  rest.variable("WindDirectionMax", &windDirectionMax);
  rest.variable("AirQualitySensor", &INTcurrentAirQualitySensor);

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
  // health indications for device
  rest.variable("ESP8266HeapSize", &heapSize);






  // Function to be exposed

  rest.function("arduino", jsonCmd); // For RasPiConnect - ArduinoConnect

  // auxillary functions
  rest.function("led", ledControl);
  rest.function("setID", setWeatherPlusIDControl);
  rest.function("resetOurWeather",   resetOurWeather);

  rest.function("setWUSID",   setWUSID);
  rest.function("setWUKEY",   setWUKEY);

  rest.function("setAdminPassword",   setAdminPassword);
  //rest.function("rebootOurWeather",   rebootOurWeather);
  rest.function("setDateTime",   setDateTime);
  rest.function("resetToDefaults",   resetToDefaults);

  rest.function("resetWiFiAccessPoint", resetWiFiAccessPoint);
  rest.function("updateOurWeather", updateOurWeather);



  // external interfaces

  rest.function("enableCWOP", enableCWOPControl);
  rest.function("enableTwitter", enableTwitterControl);

  // weather functions

  rest.function("WeatherSmall",   weatherSmallControl);
  rest.function("WeatherMedium",   weatherMediumControl);
  rest.function("WeatherLarge",   weatherLargeControl);
  rest.function("WeatherDemo",   weatherDemoControl);

  rest.function("EnglishUnits",   englishUnitControl);

  rest.function("MetricUnits",   metricUnitControl);



  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("OurWeather");



  initialize60MinuteRain();



  Serial.println();
  Serial.println();
  Serial.println("--------------------");
  Serial.println("OurWeather ESP8266-12");
  Serial.println("--------------------");
  Serial.print("Version: ");
  Serial.println(WEATHERPLUSESP8266VERSION);

  Serial.print("Compiled at:");
  Serial.print (__TIME__);
  Serial.print(" ");
  Serial.println(__DATE__);

  Serial.print("Free Sketch Space on OurWeather:");
  Serial.println(ESP.getFreeSketchSpace());




  Serial.print("port number = ");
  Serial.println(WEB_SERVER_PORT);

  delay(2000);

  ads1015.begin();

  adsAirQuality.begin();


  int16_t ad0 = adsAirQuality.readADC_SingleEnded(0);

  currentAirQuality = -1;
  currentAirQualitySensor = 0;
  INTcurrentAirQualitySensor = 0;
  if (ad0 != -1)
  {
    AirQualityPresent = true;
    Serial.println("AirQuality Extension Present");
  }
  else
  {
    AirQualityPresent = false;
    Serial.println("AirQuality Extension Not Present");
  }


  randomSeed(analogRead(0));

  lastBoot = Rtc.GetDateTime();



  Serial.print("OurWeather IP Address:");
  Serial.println(WiFi.localIP());

  //blinkIPAddress();

  updateDisplay(DISPLAY_IPDISPLAY);


  timeElapsed = 0;

  // BMP180

  /* Initialise the sensor */
  if (!bmp.begin())
  {
    /* There was a problem detecting the BM180 ... check your connections */
    Serial.println("No BMP180 detected ");
    BMP180Found = false;

  }
  else
  {
    Serial.println("BMP180 Detected");
    BMP180Found = true;
  }

  // BMP280

  /* Initialise the sensor */
  if (!bme.begin())
  {

    Serial.println("No BMP280 detected ");
    BMP280Found = false;

  }
  else
  {
    Serial.println("BMP280 Detected");
    BMP280Found = true;
  }



  // AM2315

  // setup AM2315


  AOK = am2315.readData(dataAM2315);


  if (AOK) {

    Serial.println("AM2315 Detected...");

    //Serial.print("Hum: "); Serial.println(dataAM2315[1]);
    //Serial.print("TempF: "); Serial.println(dataAM2315[0]);


    AM2315_Temperature = dataAM2315[1];
    AM2315_Humidity = dataAM2315[0];

  }
  else
  {

    Serial.println("AM2315 Sensor not found, check wiring & pullups!");

  }


  AM2315_Temperature = 0.0;

  AM2315_Humidity = 0.0;




}


//
//
// loop()
//
//


void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Starting Main Loop");
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
  client.stop();




  if (timeElapsed > 5000)
  {
    Serial.println("5 second Loop executed");






    timeElapsed = 0;

    Serial.print("Free heap on ESP8266:");
    heapSize = ESP.getFreeHeap();
    Serial.println(heapSize, DEC);


    tmElements_t tm;
    Serial.println("---------------");
    Serial.println("DS3231 Clock");
    Serial.println("---------------");

    RtcDateTime now = Rtc.GetDateTime();

    String currentTimeString;
    currentTimeString = returnDateTime(now);


    Serial.println(currentTimeString);

    RestTimeStamp = currentTimeString;

    RestDataString = "";


    Serial.println("---------------");
    Serial.println("AM2315");
    Serial.println("---------------");




    AOK = am2315.readData(dataAM2315);
#ifdef DEBUGPRINT
    Serial.print("AOK=");
    Serial.println(AOK);
#endif


    AM2315_Temperature = dataAM2315[1];
    AM2315_Humidity = dataAM2315[0];
#ifdef DEBUGPRINT
    Serial.print("FBRi=");
    for (uint8_t i = 0; i < 10; i++)
    {

      Serial.print(FirstBadReply[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
#endif
    Serial.print("Temp: "); Serial.println(AM2315_Temperature);
    Serial.print("Hum: "); Serial.println(AM2315_Humidity);
#ifdef DEBUGPRINT
    Serial.print("AM2315TotalCount="); Serial.println(AM2315TotalCount);
    Serial.print("AM2315BadCount="); Serial.println(AM2315BadCount);
#endif



    RestDataString += String(AM2315_Temperature, 2) + ",";
    RestDataString += String(AM2315_Humidity, 2) + ",";

    Serial.println("---------------");
    if (BMP180Found)
      Serial.println("BMP180");
    else if (BMP280Found)
      Serial.println("BMP280");
    else
      Serial.println("No BMP180/BMP280 Found");
    Serial.println("---------------");


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

    RestDataString += String(BMP180_Temperature, 2) + ",";
    RestDataString += String(BMP180_Pressure, 2) + ",";
    RestDataString += String(BMP180_Altitude, 2) + ",";



    if (AirQualityPresent)
    {

      Serial.println("---------------");
      Serial.println("AirQualitySensor");
      Serial.println("---------------");


#ifdef DEBUGPRINT

      int16_t ad0 = adsAirQuality.readADC_SingleEnded(0);
      int16_t ad1 = adsAirQuality.readADC_SingleEnded(1);
      int16_t ad2 = adsAirQuality.readADC_SingleEnded(2);
      int16_t ad3 = adsAirQuality.readADC_SingleEnded(3);

      Serial.print("ad0=");
      Serial.println(ad0);
      Serial.print("ad1=");
      Serial.println(ad1);
      Serial.print("ad2=");
      Serial.println(ad2);
      Serial.print("ad3=");
      Serial.println(ad3);


      int16_t ad0_0x49 = ads1015.readADC_SingleEnded(0);
      int16_t ad1_0x49 = ads1015.readADC_SingleEnded(1);
      int16_t ad2_0x49 = ads1015.readADC_SingleEnded(2);
      int16_t ad3_0x49 = ads1015.readADC_SingleEnded(3);

      Serial.print("ad0_0x49=");
      Serial.println(ad0_0x49);
      Serial.print("ad1_0x49=");
      Serial.println(ad1_0x49);
      Serial.print("ad2_0x49=");
      Serial.println(ad2_0x49);
      Serial.print("ad3_0x49=");
      Serial.println(ad3_0x49);
#endif

      currentAirQuality = getAirQuality();

    }

    Serial.println("---------------");
    Serial.println("WeatherRack");
    Serial.println("---------------");

    currentWindSpeed = weatherStation.current_wind_speed();
    currentWindGust = weatherStation.get_wind_gust();

    currentWindDirection = weatherStation.current_wind_direction();

    float oldRain = rainTotal;
    rainTotal = rainTotal + weatherStation.get_current_rain_total();
    if (oldRain < rainTotal)
    {
      strcpy(bubbleStatus, "It is Raining\0");
    }

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




    Serial.print("windSpeedMin =");
    Serial.print(windSpeedMin);
    Serial.print(" windSpeedMax =");
    Serial.println(windSpeedMax);
#ifdef DEBUGPRINT
    Serial.print("windSpeedBuffer=");
    Serial.println(windSpeedBuffer);
#endif
    Serial.print("windGustMin =");
    Serial.print(windGustMin);
    Serial.print(" windGustMax =");
    Serial.println(windGustMax);
#ifdef DEBUGPRINT
    Serial.print("windGustBuffer=");
    Serial.println(windGustBuffer);
#endif
    Serial.print("windDirectionMin =");
    Serial.print(windDirectionMin);
    Serial.print(" windDirectionMax =");
    Serial.println(windDirectionMax);

#ifdef DEBUGPRINT
    Serial.print("windDirectionBuffer=");
    Serial.println(windDirectionBuffer);
#endif
    Serial.print("currentWindSpeed=");
    Serial.print(currentWindSpeed);

    Serial.print(" \tcurrentWindGust=");
    Serial.print (currentWindGust);

    Serial.print(" \tWind Direction=");
    Serial.print(currentWindDirection);


    Serial.print(" \t\tCumulative Rain = ");
    Serial.println(rainTotal);


    Serial.println(" ");



    RestDataString += String(currentWindSpeed, 2) + ",";
    RestDataString += String(currentWindGust, 2) + ",";
    RestDataString += String(currentWindDirection, 2) + ",";
    RestDataString += String(rainTotal, 2) + ",";
    RestDataString += String(windSpeedMin, 2) + ",";
    RestDataString += String(windSpeedMax, 2) + ",";
    RestDataString += String(windGustMin, 2) + ",";
    RestDataString += String(windGustMax, 2) + ",";

    RestDataString += String(windDirectionMin, 2) + ",";
    RestDataString += String(windDirectionMax, 2) + ",";
    RestDataString += String(EnglishOrMetric) + "," ;
    RestDataString += currentTimeString + "," ;
    RestDataString += stationName + ",";
    RestDataString += String(currentAirQualitySensor) + ",";
    RestDataString += String(currentAirQuality);

    if (timeElapsed300Seconds > 300000)
    {


      String lastBootTimeString;
      lastBootTimeString = returnDateTime(lastBoot);

      Serial.print("lastBoot = ");
      Serial.println(lastBootTimeString);
      
      timeElapsed300Seconds = 0;

      // update rain


      add60MinuteRainReading(rainTotal - lastRain);
      lastRain = rainTotal;

      RtcDateTime now = Rtc.GetDateTime();

      if (now.Day() == lastDay)
      {
        rainCalendarDay = rainTotal - startOfDayRain;

      }
      else
      {
        lastDay = now.Day();
        rainCalendarDay = 0.0;
        startOfDayRain = rainTotal;
      }

      Serial.println("Attempting to send data to WeatherUnderground");


      sendWeatherUndergroundData();
    }

    updateDisplay(WeatherDisplayMode);
    Serial.println("OutOfDisplay");

  }



  yield();

}
