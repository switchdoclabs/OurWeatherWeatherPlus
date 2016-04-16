// Filename: Config.h
// Version 1.5 08/06/13 BP MiloCreek

// your local function include file
// ----------
// Note:  This program is designed for you to supply your own functions in a file called Local.h
// Out of the box, the program uses LocalExample.h included in ExecuteJSONCommand.h
// 
// If you change LocalExample.h and update the software, LocalExample.h WILL BE OVERWRITTEN!
// 
// Change #define LOCALFILE "LocalExample.h" to #define LOCALFILE  "Local.h" to #include "Local.h" in ExecuteJSONCommand.h
// ----------
#define LOCALFILE "Local.h"

//define if you want debug on, otherwise #undef
#define DEBUG

//define if you want to only debug local files, otherwise #undef
#define LOCALDEBUG 

//WEB_SERVER_PORT is the port that the Arduino Webserver will be responding to requests from the RasPiConnect/ArduinoConnect App
#define WEB_SERVER_PORT 80

//LOCALIP is the address of your device. If you poke a hole through your firewall and expose it to the Internet, insert that address here
//Usually, the port in the URL (i.e. 9600) will match the WEB_SERVER_PORT above but can be remapped in most routers/firewalls
//Note that the address for your SystemURL in the RasPiConnectApp/ArduinoConnectApp should be of the form:  http://192.168.1.120:9600/arduino (see manual for more information)

#define LOCALIP  { 192, 168, 1, 140 }

// LOCALMAC must be a unique value on your network.  Don't have two Arduinos as the same MAC on a network
#define LOCALMAC { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }


//USERNAME is the username that you have entered in the RasPiConnect App.  It must match and is case sensitive
#define USERNAME "RasPiConnect"

//PASSWORD is the password that you have entered in the RasPiConnect App.  It must match and is case sensitive
#define PASSWORD "RasPiConnectPassword"

// set to True if you have an I2C bus set up and has an AdaFruit BMP085 and two BlinkM modules (addresses 0xC and 0xB) False if not
#define I2CDEMO FALSE

//ARDUINOCONNECTSERVER Version Number.  Do not change!
#define VERSIONNUMBER  "1.6"
