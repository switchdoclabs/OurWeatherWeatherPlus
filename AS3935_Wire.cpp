#include <Wire.h>
#include "AS3935.h"

/*
   a line-by-line port of https://github.com/pcfens/particle-as3935/
   an exercise for me to write a library.
*/

/**
   Constructor.
   @param address I2C address of AS3935.
   @param interruptPin pin that is tied to IRQ pin of AS3935.
*/
AS3935::AS3935(uint8_t address, uint8_t interruptPin)
{
  _address = address;
  _interruptPin = interruptPin;
}

AS3935::AS3935::~AS3935()
{
}

/**
   Begin using the object with default SDA, and SCL pin numbers.
*/
void AS3935::begin()
{
  begin((int) _defaultSDA, (int) _defaultSCL);
}

/**
   Begin using the object

   - Begin wire
   - Enable interrupt pin as INPUT
   - Disable Oscillators on interrupt pin.

   @param sda SDA pin
   @param scl SCL pin
*/
void AS3935::begin(int sda, int scl)
{
  Wire.begin(sda, scl);
  //pinMode(_interruptPin, INPUT);
  disableOscillators();
}

/**
   Find the shift required to make the mask use the LSB.
   @param mask The mask to find the shift of
   @return The number of bit positions to shift the mask
*/
uint8_t AS3935::_getShift(uint8_t mask)
{
  uint8_t i = 0;
  for (i = 0; ~mask & 1; i++)
    mask >>= 1;
  return i;
}

/**
   Read a byte from a register.
   @param reg The register address
   @return The value in the register
*/
uint8_t AS3935::readRegister(uint8_t reg)
{
  uint8_t v;
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((int)_address, 1);
  v = Wire.read();
  return v;
}

/**
   Read a byte from a register, return a masked and shifted value
   @param reg The register address
   @param mask The mask to use when shifting contents
   @return An uint8_t with the right most bits containing the masked and
   shifted contents of the requested register
*/
uint8_t AS3935::readRegisterWithMask(uint8_t reg, uint8_t mask)
{
  uint8_t v;
  v = readRegister(reg) & mask;
  return (v >> _getShift(mask));
}

/**
   Write a masked value to register reg, preserving other bits
   @param reg The register address
   @param mask The bitmask to mask
   @param value The value to write to the register
*/
void AS3935::writeRegisterWithMask(uint8_t reg, uint8_t mask, uint8_t value)
{
  uint8_t registerValue;
  registerValue = readRegister(reg);
  registerValue &= ~(mask);
  registerValue |= ((value << (_getShift(mask))) & mask);
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(registerValue);
  Wire.endTransmission();
}

/**
   Write value to register reg.
   @param reg the register address to write value to.
   @param value the value to write to the register.
*/
void AS3935::writeRegister(uint8_t reg, uint8_t value)
{
  writeRegisterWithMask(reg, 0xff, value);
}

/**
   Sets all registers in default mode
*/
void AS3935::setDefault(void)
{
  writeRegister(0x3c, 0x96);
}

/**
   Calibrates the internal RC Oscillators automatically
*/
void AS3935::calibrateRCO(void)
{
  writeRegister(0x3D, 0x96);
}

/**
   Disable LCO/SRCO/TRCO on IRQ pin.
*/
void AS3935::disableOscillators(void)
{
  writeRegisterWithMask(0x08, 0xE0, 0x00);
}

/**
   Get interrupt reason
   @return one of AS3935_INT_STRIKE, AS3935_INT_DISTURBER, AS3935_INT_NOISE
*/
uint8_t AS3935::getInterruptReason(void)
{
  return readRegisterWithMask(0x03, 0b00001111);
}

/**
   Return the estimated distance in km to the head of an approaching storm.
   @return int8_t value of the estimated distance in km,
   AS3935_DISTANCE_OUT_OF_RANGE when out of range, or -1 when the register
   value is invalid. See also: 8.9.3 Statistical Distance Estimation
*/
int8_t AS3935::getDistance(void)
{
  uint8_t v;
  int8_t d;
  v = readRegisterWithMask(0x07, 0b00111111);
  switch (v) {
    case 0b111111:
      d = AS3935_DISTANCE_OUT_OF_RANGE;
      break;
    case 0b101000:
      d = 40;
      break;
    case 0b100101:
      d = 37;
      break;
    case 0b100010:
      d = 34;
      break;
    case 0b011111:
      d = 31;
      break;
    case 0b011011:
      d = 27;
      break;
    case 0b011000:
      d = 24;
      break;
    case 0b010100:
      d = 20;
      break;
    case 0b010001:
      d = 17;
      break;
    case 0b001110:
      d = 14;
      break;
    case 0b001100:
      d = 12;
      break;
    case 0b001010:
      d = 10;
      break;
    case 0b001000:
      d = 8;
      break;
    case 0b000110:
      d = 6;
      break;
    case 0b000101:
      d = 5;
      break;
    case 0b000001:
      d = 0;
      break;
    default:
      d = -1;
      break;
  }
  return d;
}

/**
   Returns bool whether or not current AFE setting is indoor.
   @return true if the setting is indoor, false if not
*/
bool AS3935::isIndoor()
{
  return readRegisterWithMask(0x00, 0b11000001) == AS3935_AFE_INDOOR;
}

/**
   Set AFE setting to indoor mode
   @return true or false whether if setting to indoor mode succeeded.
*/
bool AS3935::setIndoor()
{
  writeRegisterWithMask(0x00, 0b11000001, AS3935_AFE_INDOOR);
  return isIndoor();
}

/**
   Set or unset AFE setting to indoor mode.
   @param enable True of false whether to set AFE to indoor mode.
   @return true or false whether if setting to indoor mode succeeded.
*/
bool AS3935::setIndoor(bool enable)
{
  return enable ? setIndoor() : setOutdoor();
}

/**
   Returns bool whether or not current AFE setting is outdoor.
   @return true if the setting is outdoor, false if not
*/
bool AS3935::isOutdoor()
{
  return readRegisterWithMask(0x00, 0b11000001) == AS3935_AFE_OUTDOOR;
}

/**
   Set the AFE setting to outdoor mode.
   @return true or false whether if setting to outdoor mode succeeded.
*/
bool AS3935::setOutdoor()
{
  writeRegisterWithMask(0x00, 0b11000001, AS3935_AFE_OUTDOOR);
  return isOutdoor();
}

/**
   Set or unset AFE setting to outdoor mode.
   @param enable True of false whether to set AFE to outdoor mode.
   @return true or false whether if setting to outdoor mode succeeded.
*/
bool AS3935::setOutdoor(bool enable)
{
  return enable ? setOutdoor() : setIndoor();
}


void AS3935::disableDisturbers()
{
  writeRegisterWithMask(AS3935_MASK_DIST, 1);
}

void AS3935::enableDisturbers()
{
  writeRegisterWithMask(AS3935_MASK_DIST, 0);
}
/**
   Get minimum number of lightning
   @return uint8_t number of minimum number of lightning, one of 1, 5, 9, or
   16.
*/
uint8_t AS3935::getMinimumLightning(void)
{
  switch (readRegisterWithMask(0x02, 0b11001111)) {
    case 0:
      return 1;
      break;
    case 1:
      return 5;
      break;
    case 2:
      return 9;
      break;
    case 3:
      return 16;
      break;
    default:
      return 255;
      break;
  }
}

/**
   Set minimum number of lightning to trigger an event
   @param n Minimum number of lightnings, one of 1, 5, 9, or 16.
   @return bool whether or not setting the value succeeded.
*/
bool AS3935::setMinimumLightning(uint8_t n)
{
  if (n == 1 || n == 5 || n == 9 || n == 16) {
    writeRegisterWithMask(0x02, 0b11001111, n);
    return getMinimumLightning();
  }
  return false;
}

/**
   Clear the statistics built up by the lightning distance estimation algorithm
   block.
*/
void AS3935::clearStats(void)
{
  writeRegisterWithMask(0x02, 0b10111111, 1);
  delay(2);
  writeRegisterWithMask(0x02, 0b10111111, 0);
  delay(2);
  writeRegisterWithMask(0x02, 0b10111111, 1);
  delay(2);
}

/**
   Get noise floor level from AS3935.
   @return The current noise floor level from the register
*/
uint8_t AS3935::getNoiseFloor(void)
{
  return readRegisterWithMask(0x01, 0b01110000);
}

/**
   Set noise floor level from AS3935.
   @param level The noise floor level, from 0 to 7, to set.
   @return true or false whether if setting the level is succeeded
*/
bool AS3935::setNoiseFloor(int level)
{
  if (level < 0 || level > 7)
    return false;
  writeRegisterWithMask(0x01, 0b01110000, level);
  return getNoiseFloor() == level;
}


uint16_t AS3935::getSpikeRejection()
{
  return readRegisterWithMask(AS3935_SREJ);
}

bool AS3935::setSpikeRejection(int level)
{
  if (level < 0 || level > 15)
    return false;
  writeRegisterWithMask(AS3935_SREJ, level);
  return getSpikeRejection() == level;
}

/**
   Increase noise floor level by one. When the level raeches to the maximum
   value, 7, further call will not increase the level.
   @return The noise floor level after the change.
*/
uint8_t AS3935::increaseNoiseFloor(void)
{
  int level = getNoiseFloor();
  setNoiseFloor(level + 1);
  return getNoiseFloor();
}

/**
   Decrease noise floor level by one. When the level raeches to the minimum
   value, 0, further call will not decrease the level.
   @return The noise floor level after the change.
*/
uint8_t AS3935::descreseNoiseFloor(void)
{
  int level = getNoiseFloor();
  setNoiseFloor(level - 1);
  return getNoiseFloor();
}



uint16_t AS3935::getWatchdogThreshold(void)
{
  return readRegisterWithMask(AS3935_WDTH);
}

uint16_t AS3935::setWatchdogThreshold(uint16_t wdth)
{
  writeRegisterWithMask(AS3935_WDTH, wdth);
  return getWatchdogThreshold();
}

void AS3935::reset()
{
  //write to 0x3c, value 0x96
  Serial.println("-------Reset-------");
  writeRegister ((uint8_t)0x3c, (uint8_t)0x96);

  delay(2);
}

/**
   Set internal capacitor values, from 0 to 120pF in steps of 8pf. Interrupts
   are disabled while calibrating.
   @param cap Integer, from 0 to 15.
   @return the value of the internal capacitor
*/
uint8_t AS3935::setTuningCapacitor(uint8_t cap)
{
  if (cap <= 15 || cap >= 0) {
    noInterrupts();
    writeRegisterWithMask(0x08, 0b00001111, cap);
    delay(2);
    calibrateRCO();
    writeRegisterWithMask(0x08, 0b00100000, 1);
    delay(2);
    writeRegisterWithMask(0x08, 0b00100000, 0);
    interrupts();
  }
  return readRegisterWithMask(0x08, 0b00001111);
}
/**
   Compatibility
   @param cap Integer, from 0 to 15.
   @sa AS3935::setTuningCapacitor(uint8_t)
*/
void AS3935::calibrate(uint8_t cap)
{
  setTuningCapacitor(cap);
}



