// PUBNUB Commands
//
// December 17, 2017

// March 2018 - chnaged to http client


String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}
String myURLEncode(String urlChars)
{
  urlChars.replace("%","%25");
  urlChars.replace(" ", "%20");
  urlChars.replace(String(char('\"')), "%22");

  
  urlChars.replace("&", "%26");
  urlChars.replace(String(char(39)), "%27");

  urlChars.replace( "+", "%2B");

  return urlChars;

  
}

void GETpublishPubNubMessage(String message)
{
  // test a send to PubNub
  Serial.println("Before WL_CONNECTED");
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient
    Serial.println("Ready to send");
  Serial.print("Pub Key=");
  Serial.println(SDL2PubNubCode);
  Serial.print("Sub Key=");
  Serial.println(SDL2PubNubCode_Sub);
     String sendString;

     sendString = "http://pubsub.pubnub.com/publish/";

     sendString = sendString + SDL2PubNubCode;
     sendString = sendString + "/";
     sendString = sendString + SDL2PubNubCode_Sub;
     sendString = sendString + "/";
     sendString = sendString + "0/";
     sendString = sendString + channel1;
     sendString = sendString + "/0/";
     sendString = sendString + myURLEncode(message);
     //sendString = sendString + "{%22FullDataString%22:%20%225.80,99%22}";
     //sendString = sendString + message;
    //sendString = sendString + urlencode(message);

     Serial.println(sendString);


    http.begin(sendString);  //Specify request destination


    int httpCode = http.GET();                                                                  //Send the request
    Serial.print("httpCode = ");
    Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code

      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload

    }

    http.end();   //Close connection

  }
}

// send our JSON message to PubNub

void publishPubNubMessage(String message)
{


  WiFiClient *client;




  //Publish

  Serial.println("publishing a message");


  // Message 2


#ifdef DEBUG
  Serial.println(message);
  Serial.print("Size=");
  Serial.println(message.length());
  Serial.print("channel=");
  Serial.println(channel1);

  Serial.print("Pub Key=");
  Serial.println(SDL2PubNubCode);
  Serial.print("Sub Key=");
  Serial.println(SDL2PubNubCode_Sub);

#endif

message= myURLEncode(message);



  client = PubNub.publish(channel1, message.c_str());

  delay(5000);

  if (!client) {
    Serial.println("publishing error");
    delay(1000);
    // bad conenction for whatever reason.  Redo the connection
    return;
  }


  if (PubNub.get_last_http_status_code_class() != PubNub::http_scc_success) {
    Serial.print("Got HTTP status code error from PubNub, class: ");
    Serial.print(PubNub.get_last_http_status_code_class(), DEC);
  }
  while (client->available()) {
    Serial.write(client->read());
  }
  client->stop();
  Serial.println("---");
  //client->flush();
  //client->stop();


  //Subscribe
  //returnMessage = myBridge.connect(channel);

  //Serial.print("returnMessage=");
  //Serial.println(returnMessage);


}



int sendStateSDL2PubNub(String command)
{
  String sentPassword;

  if (pubNubEnabled == 1)
  {
    sentPassword = getValue(command, ',', 0);



    if (sentPassword == adminPassword)
    {

      String SendString = "{\"FullDataString\": \"" + RestDataString + "\"}"; //Send the request

      // publish it

      //GETpublishPubNubMessage(SendString);
      publishPubNubMessage(SendString);
    }
    else
    {

      return 0;
    }
    return 1;




  }

  return 0;

}

// REST Functions

// Enable PubNub


int enableDisableSDL2PubNub(String command)
{

  Serial.print("Command =");
  Serial.println(command);

  String sentPassword;
  String setValue;


  sentPassword = getValue(command, ',', 0);
  setValue = getValue(command, ',', 1);
  SDL2PubNubCode = getValue(command, ',', 2);
  SDL2PubNubCode_Sub = getValue(command, ',', 3);


  if (sentPassword == adminPassword)
  {
    pubNubEnabled = setValue.toInt();
    Serial.print("pubNubEnabled=");
    Serial.print(pubNubEnabled);
    if (pubNubEnabled == 1)
    {


      writeEEPROMState();



    }
    return 1;
  }
  else
    return 0;

  // Get state from command


  return 1;
}

