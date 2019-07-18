// Filename WeatherPlus.ino
// Version 035 December 2018
// SwitchDoc Labs, LLC
//

// edit in Atom 0549 06232019..xx
//

#include "constants.h"

#define WEATHERPLUSESP8266VERSION "033"
#define WEATHERPLUSPUBNUBPROTOCOL "OURWEATHER035"

// define DEBUGPRINT to print out lots of debugging information for WeatherPlus.

#undef DEBUGPRINT
#undef PUBNUB_DEBUG
#undef DEBUGBLYNK
#define BLYNK_NO_BUILTIN
#define BLYNK_PRINT Serial // Defines the object that is used for printing
#undef BLYNK_DEBUG
#define BLYNK_USE_128_VPINS
#include <BlynkSimpleEsp8266.h>

// Change this to undef if you don't have the OLED present
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

bool WiFiPresent = false;

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

int pubNubEnabled;

String SDL2PubNubCode  = "";
String SDL2PubNubCode_Sub = "";

// Blynk Codes
String BlynkAuthCode = "";
bool UseBlynk = false;


BlynkTimer Btimer;


// Attach virtual serial terminal to Virtual Pin
WidgetTerminal statusTerminal(V32);

#define PUBLISHINTERVALSECONDS 30


#define PubNub_BASE_CLIENT WiFiClient

#define PUBNUB_DEFINE_STRSPN_AND_STRNCASECMP

#include "PubNub.h"




// parsing function
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



//

char channel1[]  = "OWIOT1";
char uuid[]   = WEATHERPLUSPUBNUBPROTOCOL;








#include <Wire.h>

#include <Arduino.h> //needed for Serial.println

// debug the REST library

#define DEBUG_MODE 1

#include "MaREST.h"

#include <string.h>





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
#define DISPLAY_SUNAIRPLUS 16
#define DISPLAY_WXLINK 17
#define DISPLAY_SDL2PUBNUBCODE 18
#define DISPLAY_FAILED_RECONNECT 19
#define DISPLAY_LIGHTNING_STATUS 20
#define DISPLAY_LIGHTNING_DISPLAY 21

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

IPAddress myConnectedIp;
IPAddress myConnectedGateWay;
IPAddress myConnectedMask;


//----------------------------------------------------------------------


int blinkPin = 0;                // pin to blink led at each reading
// Create an instance of the server


// Create aREST instance
aREST rest = aREST();
// commands are functions that get called by the webserver framework
// they can read any posted data from client, and they output to server




#include "elapsedmillis.h"

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
float dewpoint;
double heatIndex{};
bool heatIndexValid{ false };
String heatIndexLevel[4]{ "Caution", "Extreme caution", "Danger", "Extreme danger" };

#include "SDL_ESP8266_HR_AM2315.h"


SDL_ESP8266_HR_AM2315 am2315;
float dataAM2315[2];  //Array to hold data returned by sensor.  [0,1] => [Humidity, Temperature]

boolean AOK;  // 1=successful read

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#include "AS3935.h"

// ThunderBoard AS3935 from SwitchDoc Labs
AS3935 as3935(0x02, 3);


// lightning state variables as3935

String as3935_LastLightning = "";
int as3935_LastLightningDistance = 0;
String as3935_LastEvent = "";
int as3935_LastReturnIRQ = 0;
String as3935_LastLightningTimeStamp = "";
String as3935_LastEventTimeStamp = "";
int as3835_LightningCountSinceBootup = 0;

String as3935_FullString = "";
String as3935_Params = "";

int as3935_NoiseFloor = 2;

bool as3935_Indoor = true;

int as3935_TuneCap = 7;
bool as3935_DisturberDetection = false;
int as3935_WatchdogThreshold = 3;
int as3935_SpikeDetection = 3;

bool AS3935Present = false;

void printAS3935Registers()
{
  int noiseFloor = as3935.getNoiseFloor();
  int spikeRejection = as3935.getSpikeRejection();
  int watchdogThreshold = as3935.getWatchdogThreshold();
  Serial.print("Noise floor is: ");
  Serial.println(noiseFloor, DEC);
  Serial.print("Spike rejection is: ");
  Serial.println(spikeRejection, DEC);
  Serial.print("Watchdog threshold is: ");
  Serial.println(watchdogThreshold, DEC);
}

int parseOutAS3935Parameters()
{

  // check for bad string

  if (as3935_Params.indexOf(",") == -1)
    as3935_Params = "2,1,7,0,3,3";

  String Value;

  Value = getValue(as3935_Params, ',', 0);
  if ((Value.toInt() < 0) || (Value.toInt() > 7))
    return 2;


  Value = getValue(as3935_Params, ',', 1);
  if ((Value.toInt() < 0) || (Value.toInt() > 1))
    return 2;

  Value = getValue(as3935_Params, ',', 2);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;

  Value = getValue(as3935_Params, ',', 3);
  if ((Value.toInt() < 0) || (Value.toInt() > 1))
    return 2;

  Value = getValue(as3935_Params, ',', 4);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;

  Value = getValue(as3935_Params, ',', 5);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;


  // OK, if we are here then all data is good
  Value = getValue(as3935_Params, ',', 0);
  as3935_NoiseFloor = Value.toInt();
  Value = getValue(as3935_Params, ',', 1);
  as3935_Indoor = Value.toInt();
  Value = getValue(as3935_Params, ',', 2);
  as3935_TuneCap = Value.toInt();
  Value = getValue(as3935_Params, ',', 3);
  as3935_DisturberDetection = Value.toInt();
  Value = getValue(as3935_Params, ',', 4);
  as3935_WatchdogThreshold = Value.toInt();
  Value = getValue(as3935_Params, ',', 5);
  as3935_SpikeDetection = Value.toInt();

  return 1;
}

void setAS3935Parameters()
{


  as3935.setTuningCapacitor(as3935_TuneCap);   // set to 1/2 - middle - you can calibrate on an Arduino UNO and use the value from there (pf/8)








  // lightning state variables as3935



  // first let's turn on disturber indication and print some register values from AS3935
  // tell AS3935 we are indoors, for outdoors use setOutdoors() function
  if (as3935_Indoor == true)
  {
    as3935.setIndoor();
  }
  else
  {
    as3935.setOutdoor();
  }

  as3935.setNoiseFloor(as3935_NoiseFloor);

#ifdef DEBUGPRINT
  Serial.print("NoiseFloor=");
  Serial.println(as3935_NoiseFloor);
#endif

  //AS3935.calibrate(); // can't calibrate because IRQ is polled and not through an Interrupt line on ESP8266

  // turn on indication of distrubers, once you have AS3935 all tuned, you can turn those off with disableDisturbers()

  if (as3935_DisturberDetection == true)
  {
    as3935.enableDisturbers();
  }
  else
  {
    as3935.disableDisturbers();
  }

  uint16_t getWatchdogThreshold(void);
  uint16_t setWatchdogThreshold(uint16_t wdth);


  as3935.setSpikeRejection(as3935_SpikeDetection);
  as3935.setWatchdogThreshold(as3935_WatchdogThreshold);

  // end set parameters


  // set up as3935 REST variable
  as3935_Params = String(as3935_NoiseFloor) + ",";
  as3935_Params += String(as3935_Indoor) + ",";
  as3935_Params += String(as3935_TuneCap) + ",";
  as3935_Params += String(as3935_DisturberDetection) + ",";
  as3935_Params += String(as3935_WatchdogThreshold) + ",";
  as3935_Params += String(as3935_SpikeDetection) ;


  printAS3935Registers();
}


// Station Name

String stationName;
String adminPassword;

// Health Indications for WeatherPlus

int heapSize;

// WeatherUnderground

String WeatherUnderground_StationID;
String WeatherUnderground_StationKey;
int lastMessageID;



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


//SDL_Weather_80422 weatherStation(pinAnem, pinRain, 0, 0, A0, SDL_MODE_INTERNAL_AD );
SDL_Weather_80422 weatherStation(pinAnem, pinRain, 0, 0, A0, SDL_MODE_I2C_ADS1015 );


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


#include "SDL2PubNub.h"



// SunAirPlus

bool SunAirPlus_Present;

float BatteryVoltage;
float BatteryCurrent;
float LoadVoltage;
float LoadCurrent;
float SolarPanelVoltage;
float SolarPanelCurrent;

// WXLink Support


#include "crc16.h"

//Crc 16 library (XModem)
Crc16 crc;

bool WXLink_Present;


float WXBatteryVoltage;
float WXBatteryCurrent;

float WXLoadCurrent;
float WXSolarPanelVoltage;
float WXSolarPanelCurrent;
long WXMessageID;
bool WXLastMessageGood;


#include "WXLink.h"





#include "SDL_Arduino_INA3221.h"

SDL_Arduino_INA3221 SunAirPlus;

// the three channels of the INA3221 named for SunAirPlus Solar Power Controller channels (www.switchdoc.com)
#define LIPO_BATTERY_CHANNEL 1
#define SOLAR_CELL_CHANNEL 2
#define OUTPUT_CHANNEL 3

// OLED Display

#include "OWMAdafruit_GFX.h"
#include "ESP_SSD1306.h"


#define min(a,b) ((a)<(b)?(a):(b))

#define OLED_RESET 4



ESP_SSD1306 display(OLED_RESET);

#include "OLEDDisplay.h"


//
//

//
//


// validate temperature from AM2315 - Fixes the rare +16 degrees C issue
bool invalidTemperatureFound;

float validateTemperature(float incomingTemperature)
{
  if (incomingTemperature > AM2315_Temperature + 15.0) // check for large jump in temperature
  {
    // OK, we may have an invalid temperature.  Make sure this is not a startup (current humidity will be 0.0 if startup)
    if (AM2315_Humidity < 0.1)
    {
      // we are in startup phase, so accept temperature
      invalidTemperatureFound = false;
      return incomingTemperature;
    }
    else
    {
      // we have an issue with a bad read (typically a +32 degrees C increase)
      // so send last good temperature back and flag a bad temperature
      invalidTemperatureFound = true;
      return AM2315_Temperature;
    }
  }
  if (incomingTemperature < AM2315_Temperature - 15.0) // check for large decrease in temperature
  {
    // OK, we may have an invalid temperature.  Make sure this is not a startup (current humidity will be 0.0 if startup)
    if (AM2315_Humidity < 0.1)
    {
      // we are in startup phase, so accept temperature
      invalidTemperatureFound = false;
      return incomingTemperature;
    }
    else
    {
      // we have an issue with a bad read (typically a +32 degrees C increase)
      // so send last good temperature back and flag a bad temperature
      invalidTemperatureFound = true;
      return AM2315_Temperature;
    }
  }
  invalidTemperatureFound = false;
  return incomingTemperature; // good temperature

}


//scan for I2C Addresses

bool scanAddressForI2CBus(byte from_addr)
{
  byte error;

  // The i2c_scanner uses the return value of
  // the Write.endTransmisstion to see if
  // a device did acknowledge to the address.
  Wire.beginTransmission(from_addr);
  error = Wire.endTransmission();

  if (error == 0)
  {
    return true;
  }
  else if (error == 4)
  {

  }
  return false;
}


RtcDateTime lastBoot;

#include "BlynkRoutines.h"

void setup() {


  invalidTemperatureFound = false;

  // WiFi reset loop fix - erase the WiFi saved area

  WiFi.persistent(false);




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


  // now set up thunderboard AS3935



  // reset all internal register values to defaults
  as3935.reset();


  int noiseFloor = as3935.getNoiseFloor();

  Serial.print("noiseFloor=");
  Serial.println(noiseFloor);

  if (noiseFloor == 2)
  {
    Serial.println("AS3935 Present");
    AS3935Present = true;
  }
  else
  {
    Serial.println("AS3935 Not Present");
    AS3935Present = false;
  }

  if (AS3935Present == true)
  {

    parseOutAS3935Parameters();
    setAS3935Parameters();



  }


  // Set up Wifi

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

  if (WiFi.status()  == WL_CONNECTED)

    WiFiPresent = true;


  writeEEPROMState();

  Serial.print("WiFi Channel= ");
  Serial.println(WiFi.channel());

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

  // as3935 rest variables


  rest.variable("ThunderBoardLast", &as3935_FullString);
  rest.variable("ThunderBoardParams", &as3935_Params);




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
  rest.function("setBAKEY",   setBAKEY);

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

  // PubNub

  rest.function("EnablePubNub", enableDisableSDL2PubNub);

  rest.function("SendPubNubState", sendStateSDL2PubNub);


  // Thunderboard functions AS3935



  rest.function("setThunderBoardParams", setThunderBoardParams);





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

  // test for SunAirPlus_Present
  SunAirPlus_Present = false;

  LoadVoltage = SunAirPlus.getBusVoltage_V(OUTPUT_CHANNEL);

  if (LoadVoltage < 0.1)
  {
    SunAirPlus_Present = false;
    Serial.println("SunAirPlus Not Present");
  }
  else
  {
    SunAirPlus_Present = true;
    Serial.println("SunAirPlus Present");
  }

  // test for WXLink Present

  WXLink_Present = false;

  WXLink_Present = scanAddressForI2CBus(0x08);

  WXLastMessageGood = false;

  WXMessageID = 0;
  WXLoadCurrent = 0.0;


  WXBatteryVoltage = 0.0;
  WXBatteryCurrent = 0.0;

  WXSolarPanelVoltage = 0.0;
  WXSolarPanelCurrent = 0.0;
  lastMessageID = -1;

  if (WXLink_Present == false)
  {

    Serial.println("WXLink Not Present");
  }
  else
  {

    Serial.println("WXLink Present");
  }


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
  myConnectedIp = WiFi.localIP();

  Serial.print("OurWeather Gateway Address:");
  myConnectedGateWay = WiFi.gatewayIP();
  Serial.println(WiFi.gatewayIP());

  Serial.print("OurWeather subnet Mask:");
  myConnectedMask = WiFi.subnetMask();
  Serial.println(WiFi.subnetMask());


  //blinkIPAddress();

  updateDisplay(DISPLAY_IPDISPLAY);

  // Now put PUBNUB Code up there

  updateDisplay(DISPLAY_SDL2PUBNUBCODE);


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

  bool AM2315_Present = false;

  AOK = am2315.readData(dataAM2315);


  if (AOK) {

    Serial.println("AM2315 Detected...");

    //Serial.print("Hum: "); Serial.println(dataAM2315[1]);
    //Serial.print("TempF: "); Serial.println(dataAM2315[0]);


    AM2315_Temperature = dataAM2315[1];
    AM2315_Humidity = dataAM2315[0];
    dewpoint =  AM2315_Temperature - ((100.0 - AM2315_Humidity) / 5.0);
    AM2315_Present = true;

  }
  else
  {

    Serial.println("AM2315 Sensor not found, check wiring & pullups!");

  }


  AM2315_Temperature = 0.0;

  AM2315_Humidity = 0.0;
  dewpoint = 0.0;


  if (WiFiPresent == true)
  {

    PubNub.begin(SDL2PubNubCode.c_str(), SDL2PubNubCode_Sub.c_str());

    Serial.println("PubNub set up");
  }




  if (UseBlynk == true)
  {



    Blynk.config(BlynkAuthCode.c_str());

    // Setup a function to be called every 10 seconds
    Btimer.setInterval(10000L, myBTimerEvent);
    // Every second


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
    // Clear the terminal content
    //statusTerminal.clear();
    writeToStatusLine((String)"OurWeather Version V" + (String)WEATHERPLUSESP8266VERSION + " Started");

    writeToBlynkStatusTerminal((String)"OurWeather Version V" + (String)WEATHERPLUSESP8266VERSION + " Started");
    // Print out the presents
    if (SunAirPlus_Present)
    {
      writeToBlynkStatusTerminal("SunAirPlus Present");

    }
    else
    {
      writeToBlynkStatusTerminal("SunAirPlus Not Present");
    }

    if (WXLink_Present)
    {
      writeToBlynkStatusTerminal("WXLink Present");

    }
    else
    {
      writeToBlynkStatusTerminal("WXLink Not Present");
    }

    if (AirQualityPresent)
    {
      writeToBlynkStatusTerminal("Air Quality Sensor Present");

    }
    else
    {
      writeToBlynkStatusTerminal("Air Quality Sensor Not Present");
    }

    if (BMP280Found)
    {
      writeToBlynkStatusTerminal("BMP280 Present");

    }
    else
    {
      writeToBlynkStatusTerminal("BMP280 Not Present");
    }

    if (AM2315_Present)
    {
      writeToBlynkStatusTerminal("AM2315 Present");

    }
    else
    {
      writeToBlynkStatusTerminal("AM2315 Not Present");
    }

    if (AS3935Present)
    {
      writeToBlynkStatusTerminal("AS3935 ThunderBoard Present");

    }
    else
    {
      writeToBlynkStatusTerminal("AS3935 ThunderBoard Not Present");
    }
  }

  if (EnglishOrMetric == 0)
  {
    Blynk.virtualWrite(V8,  "English");
    writeToBlynkStatusTerminal("Units set to English");
  }
  else
  {
    Blynk.virtualWrite(V8,  "Metric");
    writeToBlynkStatusTerminal("Units set to Metric ");




  }


} // end setup

float calcHeatIndex(float AM2315_Humidity, float AM2315_Temperature) {
	float heatIndex{};
	heatIndex = constants::hIc1C + constants::hIc2C * AM2315_Temperature + constants::hIc3C * AM2315_Humidity + constants::hIc4C * AM2315_Temperature * AM2315_Humidity
		+ constants::hIc5C * AM2315_Temperature * AM2315_Temperature * AM2315_Temperature + constants::hIc6C * AM2315_Humidity * AM2315_Humidity
		+ constants::hIc7C * AM2315_Temperature * AM2315_Temperature * AM2315_Humidity + constants::hIc8C * AM2315_Temperature * AM2315_Humidity * AM2315_Humidity
		+ constants::hIc9C * AM2315_Temperature * AM2315_Temperature * AM2315_Humidity * AM2315_Humidity;
	return heatIndex;
	}
//
//
// loop() ***********************************************
// ******************************************************
// ******************************************************


void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Starting Main Loop");
  // Handle REST calls
  WiFiClient client = server.available();

  int timeout;
  timeout = 0;
  if (client)
  {

    // Thank you to MAKA69 for this suggestion
    while (!client.available()) {
      Serial.print(".");
      delay(1);
      timeout++;
      if (timeout > 1000) {
        Serial.print("INFINITE LOOP BREAK!");
        break;
      }
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


    if (!WXLink_Present)
    {

      AOK = am2315.readData(dataAM2315);
#ifdef DEBUGPRINT
      Serial.print("AOK=");
      Serial.println(AOK);
#endif
      AM2315_Temperature = dataAM2315[1];
      AM2315_Humidity = dataAM2315[0];
      dewpoint =  AM2315_Temperature - ((100.0 - AM2315_Humidity) / 5.0);
	  if (AM2315_Temperature > 27) {
		  heatIndex = calcHeatIndex(AM2315_Humidity, AM2315_Temperature);
		  heatIndexValid = true;
		  }

      Serial.print("Temp: "); Serial.println(AM2315_Temperature);
      Serial.print("Hum: "); Serial.println(AM2315_Humidity);
      Serial.print("DwPt: "); Serial.println(dewpoint);
#ifdef DEBUGPRINT
      am2315.printStatistics();
#endif
    }
    else
    {
      Serial.println("WXLink Present - AM2315 local read overruled");
    }


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
    if (SunAirPlus_Present)
      Serial.println("SunAirPlus");
    else
      Serial.println("SunAirPlus Not Present");
    Serial.println("---------------");

    // if SunAirPlus present, read charge data

    if (SunAirPlus_Present)
    {

      LoadVoltage = SunAirPlus.getBusVoltage_V(OUTPUT_CHANNEL);
      LoadCurrent = SunAirPlus.getCurrent_mA(OUTPUT_CHANNEL);


      BatteryVoltage = SunAirPlus.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
      BatteryCurrent = SunAirPlus.getCurrent_mA(LIPO_BATTERY_CHANNEL);

      SolarPanelVoltage = SunAirPlus.getBusVoltage_V(SOLAR_CELL_CHANNEL);
      SolarPanelCurrent = -SunAirPlus.getCurrent_mA(SOLAR_CELL_CHANNEL);

#ifdef DEBUGPRINT
      Serial.println("");
      Serial.print("LIPO_Battery Load Voltage:  "); Serial.print(BatteryVoltage); Serial.println(" V");
      Serial.print("LIPO_Battery Current:       "); Serial.print(BatteryCurrent); Serial.println(" mA");
      Serial.println("");

      Serial.print("Solar Panel Voltage:   "); Serial.print(SolarPanelVoltage); Serial.println(" V");
      Serial.print("Solar Panel Current:   "); Serial.print(SolarPanelCurrent); Serial.println(" mA");
      Serial.println("");

      Serial.print("Load Voltage:   "); Serial.print(LoadVoltage); Serial.println(" V");
      Serial.print("Load Current:   "); Serial.print(LoadCurrent); Serial.println(" mA");
      Serial.println("");
#endif

    }
    else
    {

      LoadVoltage = 0.0;
      LoadCurrent = 0.0;


      BatteryVoltage = 0.0;
      BatteryCurrent = 0.0;

      SolarPanelVoltage = 0.0;
      SolarPanelCurrent = 0.0;


    }

    Serial.println("---------------");
    if (WXLink_Present)
      Serial.println("WXLink");
    else
      Serial.println("WXLink Not Present");
    Serial.println("---------------");

    // read variables from WXLink

    if (WXLink_Present)
    {
      if (readWXLink() == true)
      {
        WXLastMessageGood = true;
        blinkLED(2, 200);  // blink 2 for good message


      }
      else
      {

        WXLastMessageGood = false;



      }
    }




    Serial.println("---------------");
    Serial.println("WeatherRack");
    Serial.println("---------------");

    if (WXLink_Present == false)
    {

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
    }
    else
    {
      // WXLink is PRESENT, take from WXLink

      if (WXLastMessageGood == true)  // if bad WX Message, don't change
      {

        currentWindSpeed = convert4BytesToFloat(buffer, 9);
        currentWindGust = convert4BytesToFloat(buffer, 21);

        currentWindDirection = convert2BytesToInt(buffer, 7);

        float oldRain = rainTotal;
        rainTotal = convert4BytesToLong(buffer, 17);

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

        // Now overwrite outside temp/humidity

        AM2315_Temperature = validateTemperature(convert4BytesToFloat(buffer, 25));
        AM2315_Humidity = convert4BytesToFloat(buffer, 29);

        // calculate dewpoint
        dewpoint =  AM2315_Temperature - ((100.0 - AM2315_Humidity) / 5.0);
		if (AM2315_Temperature > 27) {
			heatIndex = calcHeatIndex(AM2315_Humidity, AM2315_Temperature);
			heatIndexValid = true;
			}

        // set up solar status and message ID for screen

        // if WXLINK present, read charge data



        WXLoadCurrent = convert4BytesToFloat(buffer, 41);


        WXBatteryVoltage = convert4BytesToFloat(buffer, 33);
        WXBatteryCurrent = convert4BytesToFloat(buffer, 37);

        WXSolarPanelVoltage = convert4BytesToFloat(buffer, 45);
        WXSolarPanelCurrent = convert4BytesToFloat(buffer, 49);

        WXMessageID = convert4BytesToLong(buffer, 57);

        /*   Serial.println("");
           Serial.print("WXLIPO_Battery Load Voltage:  "); Serial.print(WXBatteryVoltage); Serial.println(" V");
           Serial.print("WXLIPO_Battery Current:       "); Serial.print(WXBatteryCurrent); Serial.println(" mA");
           Serial.println("");

           Serial.print("WXSolar Panel Voltage:   "); Serial.print(WXSolarPanelVoltage); Serial.println(" V");
           Serial.print("WXSolar Panel Current:   "); Serial.print(WXSolarPanelCurrent); Serial.println(" mA");
           Serial.println("");

           Serial.print("WXLoad Current:   "); Serial.print(WXLoadCurrent); Serial.println(" mA");
           Serial.println("");
        */

      }

    }

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
    RestDataString += String(currentAirQuality) + ",";

    RestDataString += String(BatteryVoltage, 2) + ",";
    RestDataString += String(BatteryCurrent, 2) + ",";
    RestDataString += String(SolarPanelVoltage, 2) + ",";
    RestDataString += String(SolarPanelCurrent, 2) + ",";
    RestDataString += String(LoadVoltage, 2) + ",";
    RestDataString += String(LoadCurrent, 2) + ",";

    RestDataString += String(WXBatteryVoltage, 2) + ",";
    RestDataString += String(WXBatteryCurrent, 2) + ",";
    RestDataString += String(WXSolarPanelVoltage, 2) + ",";
    RestDataString += String(WXSolarPanelCurrent, 2) + ",";
    RestDataString += "0.00,";
    RestDataString += String(WXLoadCurrent, 2) + ",";

    if (invalidTemperatureFound == true)
    {
      RestDataString += "IVF:" + String(AOK) + ",";
    }
    else
    {
      RestDataString += "V:" + String(AOK) + ",";
    }
    invalidTemperatureFound = false;

    // Restart WiFi in case of connected, then lost connection
    if (WiFiPresent == true)
    {
      if (WiFi.status()  != WL_CONNECTED)
      {
        //Restart Access Point with the specified name
        WiFiManager wifiManager;
        wifiManager.setDebugOutput(true);
        Serial.println("--->Restarting Connection connect and setting");
        wifiManager.setTimeout(600);

        Serial.print("OurWeather IP Address:");
        Serial.println(myConnectedIp);

        Serial.print("OurWeather Gateway Address:");
        Serial.println(myConnectedGateWay);

        Serial.print("OurWeather subnet Mask:");
        Serial.println(myConnectedMask);
        wifiManager.setSTAStaticIPConfig( myConnectedIp, myConnectedGateWay, myConnectedMask);
        //and goes into a blocking loop awaiting configuration
        if (!wifiManager.justConnect(APssid.c_str())) {
          Serial.println("->Restarting Connection but hit timeout");
          Serial.println("->Failed Restarting Connection but hit timeout");
          blinkLED(4, 300);  // blink 4, failed to connect

        }
        else
        {
          Serial.println("->Connection Restarted");

        }
      }
    }
    if (WXLastMessageGood == true)
    {
      RestDataString += "WXLMG ,";
    }
    else
    {
      RestDataString += "WXLMB ,";
    }

    RestDataString += String(pubNubEnabled) + ",";  // index # 34

    if (AS3935Present == true)
    {
      // Now check for Lightning ThunderBoard AS3935

      Serial.println("---------------");
      Serial.println("ThunderBoard AS3935 Lightning Detector");
      Serial.println("---------------");

      // first step is to find out what caused interrupt
      int strokeDistance = 0.0;
      int irqSource = 0;

      irqSource = as3935.getInterruptReason();
      Serial.print("as3935 irqSource: ");
      Serial.println(irqSource, HEX);

      Serial.print("as3935 irqSource: ");
      Serial.println(irqSource, BIN);

      if (irqSource > 0)
      {
        printAS3935Registers();
        as3935_LastReturnIRQ = irqSource;
        // returned value is bitmap field, bit 0 - noise level too high, bit 2 - disturber detected, and finally bit 3 - lightning!
        if (irqSource & 0b0001)
        {
          Serial.println("INT_NH Interrupt: Noise level too high, try adjusting noise floor");
          writeToBlynkStatusTerminal("ThunderBoard-Noise level too high");
          as3935_LastEvent = "Noise Level too high";
          RtcDateTime now = Rtc.GetDateTime();
          as3935_LastEventTimeStamp = returnDateTime(now);
        }
        if (irqSource & 0b0100)
        {
          Serial.println("INT_D Interrupt: Disturber detected");
          writeToBlynkStatusTerminal("ThunderBoard-Disturber detected");
          as3935_LastEvent = "Disturber detected";
          RtcDateTime now = Rtc.GetDateTime();
          as3935_LastEventTimeStamp = returnDateTime(now);
        }
        if (irqSource & 0b1000)
        {
          // need to find how far that lightning stroke, function returns approximate distance in kilometers,
          // where value 1 represents storm in detector's near victinity, and 63 - very distant, out of range stroke
          // everything in between is just distance in kilometers

          strokeDistance = as3935.getDistance();

          as3935_LastEvent = "Lightning detected";
          RtcDateTime now = Rtc.GetDateTime();
          as3935_LastEventTimeStamp = returnDateTime(now);

          as3935_LastLightning  = String(strokeDistance) + " km" ;
          as3935_LastLightningTimeStamp = returnDateTime(now);
          as3935_LastLightningDistance = strokeDistance;
          as3835_LightningCountSinceBootup++;

          Serial.print("INT_L Interrupt: Lightning Detected.  Stroke Distance:");
          Serial.print(strokeDistance);
          Serial.println(" km");
          writeToBlynkStatusTerminal((String)"ThunderBoard-Lightning! Distance=" + String(strokeDistance));
          writeToStatusLine((String)"ThunderBoard-Lightning! Distance=" + String(strokeDistance));

          if (strokeDistance == 1)
            Serial.println("Storm overhead");
          if (strokeDistance == 63)
            Serial.println("Out of range lightning detected.");

          delay(3000);
          updateDisplay(DISPLAY_LIGHTNING_DISPLAY);
          delay(3000);
        }
      }
    }
    //  Lightning REST variable
    as3935_FullString = "";
    as3935_FullString += as3935_LastLightning + ",";
    as3935_FullString += as3935_LastLightningTimeStamp + ",";
    as3935_FullString += String(as3935_LastLightningDistance) + ",";
    as3935_FullString += as3935_LastEvent + ",";
    as3935_FullString += as3935_LastEventTimeStamp + ",";
    as3935_FullString += String(as3835_LightningCountSinceBootup);

    // Lighting Rest
    RestDataString += as3935_LastLightning + ",";
    RestDataString += as3935_LastLightningTimeStamp + ",";
    RestDataString += String(as3935_LastLightningDistance) + ",";
    RestDataString += as3935_LastEvent + ",";
    RestDataString += as3935_LastEventTimeStamp + ",";
    RestDataString += String(as3835_LightningCountSinceBootup) + ",";


    if (timeElapsed300Seconds > 300000)   // 5 minutes
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
    RestDataString += String(rainCalendarDay, 2) + ","; // index # 41
    RestDataString += lastBootTimeString + ","; // index # 42
	RestDataString += String(heatIndex, 0) + ",";  // index # 43
	RestDataString += String(heatIndexValid) + ","; // index  #44
	if (heatIndex > 27 && heatIndex <= 32) {
		RestDataString += heatIndexLevel[0] + ",";
		}
	else if (heatIndex > 32 && heatIndex <= 41) {
		RestDataString += heatIndexLevel[1] + ",";
		}
	else if (heatIndex > 41 && heatIndex <= 54) {
		RestDataString += heatIndexLevel[2] + ",";
		}
	else if (heatIndex > 54) {
		RestDataString += heatIndexLevel[3] + ",";
		}
	else RestDataString += "  "; //  index # 45

      bool dataStale;
      dataStale = false;
      // check for stale data from WXLink


      if (WXLink_Present)
      {
        if (lastMessageID != WXMessageID)
        {
          dataStale = false;
          lastMessageID = WXMessageID;
        }
        else
        {
          dataStale = true;
        }
      }


      if ((WeatherUnderground_StationID.length() == 0) || (WeatherUnderground_StationKey.length() == 0) )
      {
        Serial.println("-----------");
        Serial.println(" WeatherUnderground Disabled");
        Serial.println("-----------");
      }
      else // Check next and the go for it.
        if ((WeatherUnderground_StationID == "XXXX") || (WeatherUnderground_StationKey == "YYYY") )
        {
          Serial.println("-----------");
          Serial.println(" WeatherUnderground Disabled");
          Serial.println("-----------");

        }
        else
        {
          Serial.println("Attempting to send data to WeatherUnderground");
          if (dataStale == false)
            Serial.println("WeatherUnderground Data New - sent");
          else
            Serial.println("WeatherUnderground Data Stale - Not sent");

          if (dataStale == false)
          {
            if (sendWeatherUndergroundData() == 0)
            {
              // Failed - try again
              sendWeatherUndergroundData();
            }


          }
        }

      delay(2000);

      // send data up to PubNub

      if (pubNubEnabled == 1)
      {

        String SendString = "{\"FullDataString\": \"" + RestDataString + "\"}"; //Send the request

        // publish it

        publishPubNubMessage(SendString);
      }
    }


    updateDisplay(WeatherDisplayMode);


    if (AS3935Present == true)
    {
      delay(3000);
      updateDisplay(DISPLAY_LIGHTNING_STATUS);
      delay(3000);
    }

    if (SunAirPlus_Present)
    {

      delay(3000);
      updateDisplay(DISPLAY_SUNAIRPLUS);
      delay(3000);
    }

    if (WXLink_Present)
    {

      delay(3000);
      updateDisplay(DISPLAY_WXLINK);
      delay(3000);


      // check to see if pin 5 is stuck high (SCL is at 0) - then we are hung.

      int SCL, SDA;

      SCL = digitalRead(4);
      SDA = digitalRead(5);
      Serial.print("SCL/SDA=");
      Serial.print(SCL);
      Serial.print("/");
      Serial.println(SDA);

      if ((SCL == 0) || (SDA == 0))
      {
        resetWXLink();

      }
    }


    Serial.println("OutOfDisplay");

  }

  if (UseBlynk)
  {

    Blynk.run();
    Btimer.run(); // Initiates BlynkTimer
  }

  yield();

}
