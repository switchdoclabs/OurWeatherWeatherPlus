// PUBNUB Commands
//
// December 17, 2017




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

