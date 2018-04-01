#ifndef AS3935_h
#define AS3935_h
#include "Arduino.h"

static const uint8_t AS3935_INT_STRIKE = 0x08;
static const uint8_t AS3935_INT_DISTURBER = 0x04;
static const uint8_t AS3935_INT_NOISE = 0x01;
static const int8_t  AS3935_DISTANCE_OUT_OF_RANGE = -2;
static const uint8_t AS3935_AFE_INDOOR =  0b10010;
static const uint8_t AS3935_AFE_OUTDOOR = 0b01110;

// register access macros - register address, bitmask
#define AS3935_AFE_GB    0x00, 0x3E
#define AS3935_PWD        0x00, 0x01
#define AS3935_NF_LEV   0x01, 0x70
#define AS3935_WDTH       0x01, 0x0F
#define AS3935_CL_STAT    0x02, 0x40
#define AS3935_MIN_NUM_LIGH 0x02, 0x30
#define AS3935_SREJ       0x02, 0x0F
#define AS3935_LCO_FDIV     0x03, 0xC0
#define AS3935_MASK_DIST  0x03, 0x20
#define AS3935_INT        0x03, 0x0F
#define AS3935_DISTANCE     0x07, 0x1F
#define AS3935_DISP_LCO     0x08, 0x80
#define AS3935_DISP_SRCO  0x08, 0x40
#define AS3935_DISP_TRCO  0x08, 0x20
#define AS3935_TUN_CAP    0x08, 0x0F



class AS3935
{
  public:
    AS3935(uint8_t address, uint8_t interruptPin);
    ~AS3935(void);
    void begin(void);
    void begin(int sda, int scl);
    uint8_t readRegister(uint8_t reg);
    uint8_t readRegisterWithMask(uint8_t reg, uint8_t mask);
    void writeRegisterWithMask(uint8_t reg, uint8_t mask, uint8_t value);
    void writeRegister(uint8_t reg, uint8_t value);
    void setDefault(void);
    void calibrateRCO(void);
    void disableOscillators(void);
    uint8_t getInterruptReason(void);
    int8_t getDistance(void);
    bool isIndoor(void);
    bool setIndoor(void);
    bool setIndoor(bool enable);
    bool isOutdoor(void);
    bool setOutdoor(void);
    bool setOutdoor(bool enable);
    uint8_t getMinimumLightning(void);
    bool setMinimumLightning(uint8_t);
    void clearStats(void);
    uint8_t getNoiseFloor(void);
    bool setNoiseFloor(int level);
    uint8_t increaseNoiseFloor(void);
    uint8_t descreseNoiseFloor(void);
    uint8_t setTuningCapacitor(uint8_t);
    void calibrate(uint8_t);

    uint16_t getSpikeRejection(void);
    bool setSpikeRejection(int level);
    uint16_t getWatchdogThreshold(void);
    uint16_t setWatchdogThreshold(uint16_t wdth);

    void reset();

    void disableDisturbers();
    void enableDisturbers();



  private:
    uint8_t _address;
    uint8_t _interruptPin;
    const uint8_t _defaultSDA = SDA; // D4
    const uint8_t _defaultSCL = SCL; // D5
    uint8_t _getShift(uint8_t mask);
};

#endif
