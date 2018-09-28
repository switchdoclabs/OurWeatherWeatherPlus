

1. Download the Arduino IDE (henceforth called the AIDE) from  <https://www.arduino.cc/en/Main/Software>, version 1.6.8 at the time of writing. Be careful to not download from www.arduino.org! See <http://arduino.stackexchange.com/questions/11824/difference-between-arduino-cc-and-arduino-org> for the soppy details

1. Install the ESP8266 addon in the AIDE   <https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon>.   
<http://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/>.  
Open up Arduino, then go to the Preferences (File > Preferences). Then, towards the bottom of the window, copy this URL into the 'Additional Board Manager URLs' text box:

    http://arduino.esp8266.com/stable/package_esp8266com_index.json

    Here are some introductory tips on using the AIDE: <https://www.arduino.cc/en/Guide/Environment>       

1. Make a fork of the ourWeather repo at <https://github.com/switchdoclabs/OurWeatherWeatherPlus.git> in your own github repository

1. Clone the forked version of OurWeatherWeatherPlus from your github repo to your local PC.

1. Open the file `SDL_ESP8266_WeatherPlus.ino` in the AIDE.

1. In the AIDE menu system select the 'Adafruit Huzzah ESP8266' in the Tools/Board menu. SDL advises that this board setting best fit the design of the WeatherPlus board.

1. In the AIDE menu system select 'Sketch / Verify/compile' to compile the code.  

1. The binary can be exported using the AIDE menu system select 'Sketch / Export Compiled Binary'.
