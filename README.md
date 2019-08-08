
OurWeather Software for the SwitchDoc Labs Board WeatherPlus

Arduino IDE software designed for the ESP8266 on the WeatherPlus Board and the OurWeather Weather Kit

www.switchdoc.com<BR>
August 2019 

Current Version 036 - August 8, 2019

- Added SHT30 support to OurWeather

Current Version 035 - December 10, 2018

- Added Blynk App support, Changed Air Quality Sensor to A0 of the ADC converter to match manual.

Current Version 034 - September 28, 2018

- Merged frightanic changes for WPS support.  Updated Version Number

Current Version 033 - August 13, 2018

- Update aREST.h to latest version.  Potential lock fix. 

Current Version 032 - June 18, 2018

- Address fix for ADS1115 and Air Quality Sensor

Current Version 031 - March 30, 2018

- Added support for the OurWeather ThunderBoard Lightning Detection Extender Pack

Current Version 030 - March 24, 2018

- Adding WiFi Reconnect code - only after success on an AP 

Current Version 029 - March 15, 2018

- Changed Alexa to PubNub to be compatible with March 1, 2018 PubNub change

Current Version 028 - February 12, 2018

- Fixed AM2315 corruption bug - improved reliablity

Current Version 027 - January 5, 2018

- Fixed WeatherUnderGround EEPROM Corruption bug introduced in V026 

Current Version 026 - January 3, 2018

- Added MQTT to PubNub for OurWeather to Alexa interface 

Current Version 023 - August 1, 2017

- Fixed Wind direction bug - changed to Internal ADC1015

Current Version 022 - July 20, 2017

-Improved accuracy for Wind Speed and Wind Gust<BR>


Current Version 021 - February 8, 2017

- Added support for WXLink and SolarWXLink - SwitchDoc Labs Wireless Links<BR>
- Added dewpoint to WeatherUnderground<BR>


Current Version 019 - October 20, 2016

- Added support for Solar Power System and SunAirPlus<BR>
- Improved Reliability of AM2315 readings on Solar Power<BR>

Current Version 018 - September 12, 2016

- Added support for WeatherUnderground Display<BR>
- Minor Reliablity Improvements<BR>

Current Version 017 - June 17, 2016

- No functional change for OurWeather Users - Version 017 is NOT released for auto update.<BR>
- Fixed Demo mode bug when not using the OLED display.   Demo mode is what the system goes into after the 10 minute time our of not setting up OurWeather with a local network.<BR>

Current Version 016 - May 20, 2016

- No functional change.  Delete Time directory for compilation on Arduino IDE 1.6.8  <BR>
- No functional change.  Renamed ESG_AM2315 files to avoid library class conflicts and match Github archive<BR>
- Fixed infinite reboot sequence with WiFi FLASH corruption (timing of reset button)<BR>

Current Version 015 - April 17, 2016

- Added full support for Air Quality Sensor Extender Kit<BR>
- Finished Reliablity Additions for AM2315 Driver<BR>
- Support both WeatherPlus Boards with the BMP180 and BMP280<BR>

Version 014 - April 3, 2016

- Initial Release for OurWeather



