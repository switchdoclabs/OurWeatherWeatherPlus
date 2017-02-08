//
// WeatherUnderground Interface
//
// SwitchDoc Labs, Spetember 2016
//
//


int sendWeatherUndergroundData()
{


  WiFiClient client;

  const char* host = "weatherstation.wunderground.com";

  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return 1;
  }

  String myURL;
  // = "/weatherstation/updateweatherstation.php?ID=KWALIBER14&PASSWORD=20mqkah7&dateutc=now&winddir=180&windspeedmph=3.45&humidity=14&tempf=91.04&rainin=0.00&baromin=27.83&indoortempf=32.00&indoorhumidity0.00=&software=OurWeather";


  myURL = "/weatherstation/updateweatherstation.php?";
  myURL += "ID=" + WeatherUnderground_StationID;
  myURL += "&PASSWORD=" + WeatherUnderground_StationKey;
  myURL += "&dateutc=now";

  // now weather station variables

  myURL += "&winddir=" + String(currentWindDirection);

  myURL += "&windspeedmph=" + String(currentWindSpeed / 1.6);
  myURL += "&windgustmph=" + String(currentWindGust / 1.6);

  myURL += "&humidity=" + String(AM2315_Humidity);
  myURL += "&tempf=" + String((AM2315_Temperature * 9.0 / 5.0) + 32.0);
  
  myURL += "&dewptf=" + String((dewpoint * 9.0 / 5.0) + 32.0);

  myURL += "&rainin=" + String((rain60Minutes) / 25.4);
  myURL += "&dailyrainin=" + String(( rainCalendarDay) / 25.4);

 
  myURL += "&baromin=" + String((BMP180_Pressure / 1000.0) * 0.2953, 4);

  myURL += "&indoortempf=" + String((BMP180_Temperature * 9.0 / 5.0) + 32.0);
  //myURL += "&indoorhumidity%0.2f=" % HTUhumidity

  myURL += "&software=OurWeather";





  Serial.print("Requesting URL: ");
  Serial.println(myURL);

  // This will send the request to the server
  client.print(String("GET ") + myURL + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

String WUResult;

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    WUResult = line;
    yield();
  }
  Serial.print("WUResult=");
  Serial.println(WUResult);

  if (WUResult.indexOf("success") != -1)
  {
     Serial.println("WU Successful Write");
     return 1;
  }
  else
  {
    Serial.println("WU NOT Successful Write");
  }
  client.stop();
  return 0;




}

