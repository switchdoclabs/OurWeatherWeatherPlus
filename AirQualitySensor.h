int getAirQuality(void)
{

  // Values in this routine are for 16 bit ADC

  int16_t sensor_value = adsAirQuality.readADC_SingleEnded(0);
  Serial.print("Sensor_Value=");
  Serial.print(sensor_value);
  Serial.print("--->");

  currentAirQualitySensor = sensor_value;


  // limit the conversion for the INT value
  
  if (currentAirQualitySensor < 32000)
  {
    INTcurrentAirQualitySensor = currentAirQualitySensor;
  }
  else
  {
    INTcurrentAirQualitySensor = 32000;
  }
  
  if (sensor_value > 11200)
  {
    //Serial.println("Very High Pollution Detected");
    return 0;
  }
  else if (sensor_value > 6400)
  {
    //Serial.println("High Pollution");
    return 1;
  }
  else if (sensor_value > 4800)
  {
    //Serial.println("Medium Pollution");
    return 2;
  }
  else if (sensor_value > 3200)
  {
    //Serial.println("Low Pollution");
    return 3;
  }
  else
  {
    //Serial.println("Fresh Air");
    return 4;

  }

  return -1;

}

String reportAirQuality(int latest)
{

switch (latest)
  {
    case (0):
    {

      return "VHigh Pollu";

      break;
    }

    case (1):
    {

        return "High Pollu";

        break;
    }

    case (2):
    {

        return "Med Pollu";

        break;
    }

    case (3):
    {

        return "Low Pollu";

        break;
    }

    case (4):
    {

        return "Fresh Air";

        break;
    }


  default:

    {
      return "NP";


    }
  }


        return "UNKWN";
}

