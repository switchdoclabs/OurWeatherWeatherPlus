//
//
//
// Rain Functions
//
// SwitchDoc Labs September, 2016
//


// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 20;

float Rainreadings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float rain60Minutes = 0;                  // the running total
float rain60MinutesAverage = 0; // the average


void initialize60MinuteRain() {

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Rainreadings[thisReading] = 0.0;
  }
}

void add60MinuteRainReading(float incrRain) {
  // subtract the last reading:
  rain60Minutes = rain60Minutes - Rainreadings[readIndex];
  // read from the sensor:
  Rainreadings[readIndex] = incrRain;
  // add the reading to the total:
  rain60Minutes = rain60Minutes + Rainreadings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }


  // calculate the average:
  rain60MinutesAverage = rain60Minutes / numReadings;

}
